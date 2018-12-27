/*
 *      Moca Function Implementation 
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_moca.h"

extern struct gconfig gcfg;

extern char error_buf[];

char chdesp[9][30]=
{
  	"10 0x15554000",  
  	"6 0x4000",   
  	"7 0x10000",  
  	"7 0x40000",  
  	"8 0x100000", 
  	"8 0x400000",  
  	"9 0x1000000",   
  	"9 0x4000000",   
  	"10 0x10000000"
};

int mocaConnected()
{
  FILE *file;
  char *buffer;
  unsigned long fileLen;
  char *p,*p1;
  char command[100];

  sprintf(command,"clnkstat > /tmp/moca_stat.txt ");
  system(command);
  
  //Open file
  file = fopen("/tmp/moca_stat.txt", "rb");
  if (!file)
  {
      printf("Unable to open config file.");
      return 0;
  }
  
  //Get file length
  fseek(file, 0, SEEK_END);
  fileLen = ftell(file);
  fseek(file, 0, SEEK_SET);
  
  //Allocate memory
  buffer=(char *)malloc(fileLen+1);
  if (!buffer)
  {
      printf("Memory error!");
      return 0;
  }
  
  //Read file contents into buffer
  fread(buffer, 1,fileLen, file);
  fclose(file);

  p = strstr(buffer, "LINK");
  if(p) 
  {
    p[100]=0;
    p1=strstr(p, "UP");
    if (p1)
    {
		free(buffer);
		return 1;   //connected
    }
  }

  free(buffer);
  return 0;     //disconnected
 }



void getMocaValue(char *type, char *retvalue)
{
	char moca_channel[20] = {0};
	char moca_privacy[10] = {0};
	char moca_password[64] = {0};
	char moca_status[32] = {0};
	char mdmoid_moca_object[256] = {0};

	sprintf(mdmoid_moca_object, "%s.%d", "Device.MoCA.Interface", 1);
	
  if (!safe_strcmp(type, "channel"))
  {
      //sprintf(retvalue, "%d", gcfg.moca_channel);
	  
	  tr69GetUnfreshLeafData(moca_channel, mdmoid_moca_object, "FreqCurrentMaskSetting");
	  cutGetTR69(moca_channel);

	  if(moca_channel && safe_strcmp(moca_channel, "NULL"))
	 	sprintf(retvalue, "%s", moca_channel);	
	  else
	  	sprintf(retvalue, "%s", "0x0000000015554000");
  }
  else if (!safe_strcmp(type, "privacy"))
  {
  	  //sprintf(retvalue, "%d", gcfg.moca_privacy);

	  tr69GetUnfreshLeafData(moca_privacy, mdmoid_moca_object, "PrivacyEnabledSetting");
	  cutGetTR69(moca_privacy);

	  if(moca_privacy && safe_strcmp(moca_privacy, "NULL"))
	 	sprintf(retvalue, "%d", atoi(moca_privacy));	
	  else
	  	sprintf(retvalue, "%d", 0);
  }
  else if (!safe_strcmp(type, "password"))
  {
      //sprintf(retvalue, "%s", gcfg.moca_password);

	  tr69GetUnfreshLeafData(moca_password, mdmoid_moca_object, "KeyPassphrase");
	  cutGetTR69(moca_password);

	  if(moca_password && safe_strcmp(moca_password, "NULL"))
	 	sprintf(retvalue, "%s", moca_password);	
	  else
	  	sprintf(retvalue, "%s", "");
  }
  else if (!safe_strcmp(type, "status"))
  {
  	  /*
      if (mocaConnected())
		sprintf(retvalue, "<font color=#008000>Connected</font>");
      else 
		sprintf(retvalue, "<font color=#800000>Disconnected</font>");
		*/

	  tr69GetFreshLeafData(moca_status, mdmoid_moca_object, "Status");
	  cutGetTR69(moca_status);

	  if(!safe_strcmp(moca_status, "Up"))
	 	sprintf(retvalue, "%s", "<font color=\"#008000\">Connected</font>");	
	  else if(!safe_strcmp(moca_status, "Down"))
	  	sprintf(retvalue, "%s", "<font color=\"#800000\">Disconnected</font>");
	  else if(moca_status && safe_strcmp(moca_status, "NULL"))
	  	sprintf(retvalue, "%s", moca_status);
	  else 
	  	sprintf(retvalue, "%s", "");
  }
  else 
    strcpy(retvalue, "empty");

  kinfo("%s -> %s", type, retvalue);
}

int getMocaList(request *wp, char *type)
{
  FILE *fp;
  char line[500] = {0};
  char buff[30] = {0}, buff2[10] = {0};
  char mac[8][40] = {0}, tx[8][20] = {0}, rx[8][20] = {0};
  char *p;
  int i, step, macnode;

  system("clnkstat -a > /tmp/clnkstat.txt");

  fp = fopen("/tmp/clnkstat.txt", "r");
  
  if (fp == NULL)
    return;
  
  macnode = -1;
  step = 0;

  for (;;) {
    if (agets(fp, line) == -1)
      break;

	kinfo("line: %s", line);

    if (macnode == 8) {
      macnode--;
      break;
    }
    
    if (step == 0) {
      p = strstr(line,"MAC Address:");
      if (p) {
		macnode++;
		sscanf(p+strlen("MAC Address:"), "%s", mac[macnode]);
		step++;

		kinfo("set macs address: %s", mac[macnode]);
      }
      continue;
    }
    
    if (step == 1) {
      p = strstr(line, "TxBitRate:");
      if (p) {
		sscanf(p + strlen("TxBitRate:"), "%s", buff);
		buff[5] = 0;
		strcpy(buff2, buff);
		buff2[3] = 0;
		sprintf(tx[macnode], "%s.%s", buff2, buff+3);		
		step = 0;
      }
	  
      p = strstr(line,"RxBitRate:");
      if (p) {
		sscanf(p + strlen("RxBitRate:"), "%s", buff);
		buff[5] = 0;
		strcpy(buff2, buff);
		buff2[3] = 0;
		sprintf(rx[macnode], "%s.%s", buff2, buff+3);
      }
      continue;
    }
  }   

  kinfo("macnode: %d",macnode);

  fclose(fp);

  //Connection Speed 
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\" width=\"200\">Connection Speed</td>\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Adapter</td>\n");
  for (i=1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\" width=\"70\">Device %d</td>\n",i);
  req_format_write(wp, "</tr>\n");

  //MAC Address
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">MAC Address</td>\n");  
  for (i=0; i<=macnode; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",mac[i]);
  for (i=macnode+1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  
  //IP Address
  req_format_write(wp, "<tr bgcolor=\"#f1f3f9\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">IP Address</td>\n");
  for (i=0; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Adapater
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Adapter</td>\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<=macnode; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",rx[i]);
  for (i=macnode+1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Device 1
  req_format_write(wp, "<tr bgcolor=\"#f1f3f9\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Device 1</td>\n");
  if (macnode>=1)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",tx[1]);
  else
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Device 2
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Device 2</td>\n");
  if (macnode>=2)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",tx[2]);
  else
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Device 3
  req_format_write(wp, "<tr bgcolor=\"#f1f3f9\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Device 3</td>\n");
  if (macnode>=3)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",tx[3]);
  else
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Device 4
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Device 4</td>\n");
  if (macnode>=4)
    req_format_write(wp,"<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",tx[4]);
  else
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Device 5
  req_format_write(wp, "<tr bgcolor=\"#f1f3f9\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Device 5</td>\n");
  if (macnode>=5)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",tx[5]);
  else
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Device 6
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Device 6</td>\n");
  if (macnode>=6)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",tx[6]);
  else
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Device 7
  req_format_write(wp, "<tr bgcolor=\"#f1f3f9\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">Device 7</td>\n");
  if (macnode>=7)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",tx[7]);
  else
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<8; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  return 0;
}

static unsigned int my_strlen(const SYS_UCHAR  *str)
{
  const SYS_UCHAR *str0 = str;
  while(*str) str++;
  return str - str0;
}

static unsigned long int ft(
	int t,
	unsigned long int x,
	unsigned long int y,
	unsigned long int z )
{
    unsigned long int a,b,c;

    c = 0;

    if (t < 20)
    {
        a = x & y;
        b = (~x) & z;
        c = a ^ b;
    }
    else if (t < 40)
    {
        c = x ^ y ^ z;
    }
    else if (t < 60)
    {
        a = x & y;
        b = a ^ (x & z);
        c = b ^ (y & z);
    }
    else if (t < 80)
    {
        c = x ^ y ^ z;
    }

    return c;
}

static unsigned long int k(int t)
{
    unsigned long int c;

    c = 0;

    if (t < 20)
    {
        c = 0x5a827999;
    }
    else if (t < 40)
    {
        c = 0x6ed9eba1;
    }
    else if (t < 60)
    {
        c = 0x8f1bbcdc;
    }
    else if (t < 80)
    {
        c = 0xca62c1d6;
    }

    return c;
}


static inline SYS_UINT32 rotl (SYS_INT32 bits, SYS_UINT32 a)
{   
	// Rotate Left - Generates single instruction on ARM.
    return (a<<(bits)) | (a>>(32-bits));
}

static void sha1 (	unsigned char *message,		// NOTE: input, not const, extended
					int message_length,
					unsigned char *digest )		// 20 bytes
{
	int i;
	int padded_length;

	unsigned long int l;
	unsigned long int t;
	unsigned long int h[5];
	unsigned long int a,b,c,d,e;
	unsigned long int w[80];
	unsigned long int temp;

    /* Calculate the number of 512 bit blocks */

    padded_length = message_length + 8; /* Add length for l */
    padded_length = padded_length + 1; /* Add the 0x01 bit postfix */

    l = message_length * 8;

    /* Round up to multiple of 64 */
    padded_length = (padded_length + 63) & ~63;

    /* clear the padding field */
    memset((char *)(message+message_length), 0, padded_length - message_length);

    /* Insert b1 padding bit */
    message[message_length] = 0x80;

    /* Insert length */
    message[padded_length-1] = (unsigned char)( l        & 0xff);
    message[padded_length-2] = (unsigned char)((l >> 8)  & 0xff);
    message[padded_length-3] = (unsigned char)((l >> 16) & 0xff);
    message[padded_length-4] = (unsigned char)((l >> 24) & 0xff);

    /* Set initial hash state */
    h[0] = 0x67452301;
    h[1] = 0xefcdab89;
    h[2] = 0x98badcfe;
    h[3] = 0x10325476;
    h[4] = 0xc3d2e1f0;

    for (i = 0; i < padded_length; i += 64)
    {
        /* Prepare the message schedule */
        for (t=0; t < 16; t++)
        {
            w[t]  = (256*256*256) * message[i + (t*4)    ];
            w[t] += (256*256    ) * message[i + (t*4) + 1];
            w[t] += (256        ) * message[i + (t*4) + 2];
            w[t] +=                 message[i + (t*4) + 3];
        }
        for ( ; t < 80; t++)
        {
            w[t] = rotl(1,(w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16]));
        }

        /* Initialize the five working variables */
        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        e = h[4];

        /* iterate a-e 80 times */

		// NOTE: split into 4 loops by 20 to simplify ft() and k()
        for (t = 0; t < 80; t++)
        {
            temp = (rotl(5,a) + ft(t,b,c,d)) & 0xffffffff;
            temp = (temp + e) & 0xffffffff;
            temp = (temp + k(t)) & 0xffffffff;
            temp = (temp + w[t]) & 0xffffffff;
            e = d;
            d = c;
            c = rotl(30,b);
            b = a;
            a = temp;
        }

        /* compute the ith intermediate hash value */
        h[0] = (a + h[0]) & 0xffffffff;
        h[1] = (b + h[1]) & 0xffffffff;
        h[2] = (c + h[2]) & 0xffffffff;
        h[3] = (d + h[3]) & 0xffffffff;
        h[4] = (e + h[4]) & 0xffffffff;
    }

    digest[3]  = (unsigned char) ((h[0]      ) & 0xff);
    digest[2]  = (unsigned char) ((h[0] >>  8) & 0xff);
    digest[1]  = (unsigned char) ((h[0] >> 16) & 0xff);
    digest[0]  = (unsigned char) ((h[0] >> 24) & 0xff);

    digest[7]  = (unsigned char) ((h[1]      ) & 0xff);
    digest[6]  = (unsigned char) ((h[1] >>  8) & 0xff);
    digest[5]  = (unsigned char) ((h[1] >> 16) & 0xff);
    digest[4]  = (unsigned char) ((h[1] >> 24) & 0xff);

    digest[11] = (unsigned char) ((h[2]      ) & 0xff);
    digest[10] = (unsigned char) ((h[2] >>  8) & 0xff);
    digest[9]  = (unsigned char) ((h[2] >> 16) & 0xff);
    digest[8]  = (unsigned char) ((h[2] >> 24) & 0xff);

    digest[15] = (unsigned char) ((h[3]      ) & 0xff);
    digest[14] = (unsigned char) ((h[3] >>  8) & 0xff);
    digest[13] = (unsigned char) ((h[3] >> 16) & 0xff);
    digest[12] = (unsigned char) ((h[3] >> 24) & 0xff);

    digest[19] = (unsigned char) ((h[4]      ) & 0xff);
    digest[18] = (unsigned char) ((h[4] >>  8) & 0xff);
    digest[17] = (unsigned char) ((h[4] >> 16) & 0xff);
    digest[16] = (unsigned char) ((h[4] >> 24) & 0xff);

}


/*******************************************************************************
*
* Public method:      InitSecurityKey(char*password, SYS_UCHAR* seed[20])
*
********************************************************************************
*
* Description:	Generate binary seed for storage, from the incoming ascii pwd.
*               
* Inputs:    17 byte ascii password
*
* Outputs:   20 byte binary seed
*      
*
********************************************************************************/
void InitSecurityKey(SYS_CHAR password[MAX_PASSWORD_LENGTH],
                     SYS_UCHAR binary_seed[PASSWD_SEED_LENGTH])
{
	SYS_INT32  len = my_strlen((SYS_UCHAR *)password);
	SYS_CHAR   ascii_key[64];  // sha1() extends 17 to 64

	// Restrict Key to 17 characters
	if (len > MAX_PASSWORD_LENGTH)
	{
		password += len - MAX_PASSWORD_LENGTH;
		len = MAX_PASSWORD_LENGTH;
	}
	// If less than 17 Chars, prefix with leading ascii zeros.
	memset((char *)ascii_key, '0', MAX_PASSWORD_LENGTH);
	/* null terminate string so it can be printed later */
	ascii_key[MAX_PASSWORD_LENGTH] = '\0';
	memcpy((char *)(ascii_key + MAX_PASSWORD_LENGTH - len), (char *)password, len);

	// Hash ascii_key to get binary_seed
	memset((char *)binary_seed, 0, PASSWD_SEED_LENGTH);
	sha1((SYS_UCHAR *)ascii_key, MAX_PASSWORD_LENGTH, binary_seed );
}

int getclinkpid()
{
  FILE *fp;
  char buff[100] = {0};
 
  sprintf(buff,"ps | grep clinkd > /tmp/moca_pid.txt ");
  system(buff);
  
  //Open file
  fp = fopen("/tmp/moca_pid.txt", "rb");
  if(!fp)
  {
      printf("Unable to open config file.");
      return 0;
  }
  
  buff[0] = 0;
  fscanf(fp,"%s", buff);
  fclose(fp);

  return strtol(buff, NULL, 10);
}


int updateMocaSetting(int newchannel, int newprivacy, char *password)
{
  FILE *fp;
  int mocapid = 0;
  int pwflag = 0;
  int i = 0;
  unsigned char newseed[20] = {0};
 
  fp = fopen("/etc/clink.conf","w");
  if(fp == NULL)
    return;

  if ((newprivacy == 1) && (strlen(password) == 17))
    pwflag=1;
  else
    pwflag=0;

  fprintf(fp,"hostId 1 1\n");
  fprintf(fp,"ipaddr 9 10.1.1.21\n");
  fprintf(fp,"netmask 13 255.255.255.0\n");
  fprintf(fp,"gateWay 8 10.1.1.1\n");

  if (pwflag==0) {
  	fprintf(fp,"password 40 c1f6ea8523102b1ba3628c81b64a5371ed372b74\n");
  	fprintf(fp,"mocapassword 0 \n");
  }
  else
  {      
      // convert 12 to 17 digit string mocapassword to 20 byte password seed
      InitSecurityKey((SYS_UCHAR *)password, (SYS_UCHAR *)newseed);
      fprintf(fp,"password 40 ");
      for (i = 0; i < PASSWD_SEED_LENGTH; i++)
      {
	  	fprintf(fp,"%02x", newseed[i]);
      }
      fprintf(fp,"\n");
      fprintf(fp,"mocapassword 17 %s\n",password);
  }

  fprintf(fp,"CMRatio 2 20\n");
  fprintf(fp,"DistanceMode 1 0\n");
  fprintf(fp,"TxPower 2 10\n");
  fprintf(fp,"phyMargin 1 8\n");
  fprintf(fp,"phyMBitMask 3 127\n");

  if (pwflag==0)
    fprintf(fp,"SwConfig 5 0x7ff\n");
  else
    fprintf(fp,"SwConfig 5 0x17ff\n");

  fprintf(fp,"dhcp 1 0\n");
  fprintf(fp,"admPswd 8 entropic\n");
  fprintf(fp,"channelPlan 1 2\n");

  if (newchannel==0)
    fprintf(fp,"scanMask 10 0x155555f8\n");
  else
    fprintf(fp,"scanMask %s\n",chdesp[newchannel]);

  fprintf(fp,"productMask 10 0x155541f8\n");
  
  if (newchannel==0)
    fprintf(fp,"channelMask 10 0x15554000\n");
  else
    fprintf(fp,"channelMask %s\n",chdesp[newchannel]);

  fprintf(fp,"lof 10 1450000000\n");
  fprintf(fp,"bias 1 1\n");
  fprintf(fp,"TargetPhyRate 3 180\n");
  fprintf(fp,"PowerCtlPhyRate 3 235\n");
  fprintf(fp,"BeaconPwrLevel 2 10\n");
  fprintf(fp,"MiiPausePriLevel 2 -1\n");
  fprintf(fp,"PQoSClassifyMode 1 0\n");
  fprintf(fp,"mfrVendorId 1 0\n");
  fprintf(fp,"mfrHwVer 1 0\n");
  fprintf(fp,"mfrSwVer 1 0\n");
  fprintf(fp,"personality 1 0\n");
  fprintf(fp,"feicProfileId 2 -1\n");
  fprintf(fp,"dbgMask 3 0x0\n");
  
  fclose(fp);
  
  mocapid = getclinkpid();
  if(mocapid > 0)
    kill(mocapid, SIGHUP);
 
  return 1;
}


void actMocasetup(request *wp, char *path, char *query)
{		
	char tmpBuf[100] = {0};	
	char moca_channel[20] = {0};
	char moca_privacy[10] = {0};
	char moca_password[64] = {0};
	char mdmoid_moca_object[256] = {0};
	char *strVal = NULL;
    char *newpassword = NULL;
	char *newchannel = NULL;
	int newprivacy = 0;	

	sprintf(mdmoid_moca_object, "%s.%d", "Device.MoCA.Interface", 1);
	
	kinfo("Enter");

	strVal = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", strVal);

	if(!safe_strcmp(strVal, "apply"))
	{
		newchannel = req_get_cstream_var(wp, ("mocachannel"), "");
		if(newchannel[0]) {
		  	kinfo("mocachannel: %s", newchannel);	

			if(strchr(newchannel, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! Channel is invalid."));
				goto setErr_moca;
			}
			
			if(tr69CommitSetLeafDataString("Device.MoCA.Interface", 1, "FreqCurrentMaskSetting", newchannel))
			{
				strcpy(error_buf, ("Set Device.MoCA.Interface.1.FreqCurrentMaskSetting error"));
				goto setErr_moca;
			}
		}

		strVal = req_get_cstream_var(wp, ("mocaonoff"), "");
		if(strVal[0]) {			
			kinfo("mocaonoff: %s", strVal);

			if (!safe_strcmp(strVal, "on"))
				newprivacy = 1;
			else
				newprivacy = 0;

			if(tr69CommitSetLeafDataInt("Device.MoCA.Interface", 1, "PrivacyEnabledSetting", newprivacy))
			{
				strcpy(error_buf, ("Set Device.MoCA.Interface.1.PrivacyEnabledSetting error"));
				goto setErr_moca;
			}
		}

		newpassword = req_get_cstream_var(wp, ("mocapwd"), "");
		if(newpassword[0])
		{
			kinfo("mocapwd: %s", newpassword);	

			if(strchr(newpassword, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! Password is invalid."));
				goto setErr_moca;
			}
			
			if(tr69CommitSetLeafDataString("Device.MoCA.Interface", 1, "KeyPassphrase", newpassword))
			{
				strcpy(error_buf, ("Set Device.MoCA.Interface.1.KeyPassphrase error"));
				goto setErr_moca;
			}
		}

		if(tr69CommitSetLeafDataEnd("Device.MoCA.Interface", 1))
		{
			strcpy(error_buf, ("Set Device.MoCA.Interface.1 error"));
			goto setErr_moca;
		}
		tr69_save_now();

		thankyouPage(wp);
	}
	else if(!safe_strcmp(strVal, "view"))
	{
		nothankyouPage(wp);
	}
	else if(!safe_strcmp(strVal, "getvalue"))
	{
		tr69GetUnfreshLeafData(moca_channel, mdmoid_moca_object, "FreqCurrentMaskSetting");		
	    cutGetTR69(moca_channel);
		changeString(moca_channel);
		
  	    tr69GetUnfreshLeafData(moca_privacy, mdmoid_moca_object, "PrivacyEnabledSetting");		
	    cutGetTR69(moca_privacy);
		changeString(moca_privacy);

	    tr69GetUnfreshLeafData(moca_password, mdmoid_moca_object, "KeyPassphrase");		
	    cutGetTR69(moca_password);
		changeString(moca_password);

		req_format_write(wp, ("%s|%s|%s||"), moca_channel, moca_privacy, moca_password);				
	}
	
	return;  

setErr_moca:
	kerror("error_buf: %s", error_buf);
	backPage(wp);
}



