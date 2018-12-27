/*
 *      Wireless Version1 Function Implementation  
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */
#include "act_wireless.h"
#include "utility.h"

#ifdef ACTIONTEC_MULTIACCESS
#include "act_session.h"
#endif

extern struct gconfig gcfg;

extern int mssid_idx;
extern int wlan_idx;
extern int vwlan_idx;
extern char WLAN_IF[20];
extern char *BRIDGE_IF;

static int mssididx = 0;
static SS_STATUS_Tp pStatus = NULL;

int client_select_id = 0;
char *client_ssid = NULL;
int client_channel = 0;
int client_encrypt = 0;
int client_wpa_tkip_aes = 0;
int client_wpa2_tkip_aes = 0;
int both_freq = 1;
int curr_ssid = 0;

static int channel5g[20]=
	    { 36, 40, 44, 48, 52, 56, 60, 64,100,104,
	     108,112,116,120,124,128,132,136,140,149};

int ClientConnected()
{
  FILE *file;
  char *buffer;
  unsigned long fileLen;
  char *p,*p1;
  char command[100];

  sprintf(command,"iwconfig > /tmp/client_stat.txt ");
  //system(command);
  
  //Open file
  file = fopen("/tmp/client_stat.txt", "rb");
  if (!file)
  {
      //printf("Unable to open config file.");
      return 0;
  }
  
  //Get file length
  fseek(file, 0, SEEK_END);
  fileLen=ftell(file);
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

  p=strstr(buffer,"Access Point:");
  if (p) 
  {
    p[100]=0;
    p1=strstr(p,"Not-Associated");
    if (p1)
    {
		free(buffer);
		return 0;   //disconnected
    }
    else 
	{
        free(buffer);
        return 1; //connected
    }
  }

  free(buffer);
  return 0;     //disconnected
}



void getWlValue(char *type, char *retvalue)
{
  if (!safe_strcmp(type, "apenb"))
  {
    sprintf(retvalue, "%s", gcfg.wlan_on_boot);
  }
  else if (!safe_strcmp(type, "ssid"))
  {
  	sprintf(retvalue,"%s", gcfg.ap_ssid);
  }
  else if (!safe_strcmp(type, "mode")) 
  {
    if (!safe_strcmp(gcfg.ap_startmode, "standard") 
		|| !safe_strcmp(gcfg.ap_startmode, "multi")) 
      strcpy(retvalue, "ap");
    else
      strcpy(retvalue, "client");
  }
  else if (!safe_strcmp(type, "freq")) 
  {
    if (!safe_strcmp(gcfg.ap_chmode, "11NGHT20"))  //11NAHT40MINUS
      strcpy(retvalue, "2");
    else
      strcpy(retvalue, "5");
  }   
  else if (!safe_strcmp(type, "channel")) 
  {
    if (!safe_strcmp(gcfg.ap_primary_ch, "11ng"))
      strcpy(retvalue, "Automatic");
    else if (!safe_strcmp(gcfg.ap_primary_ch, "11na"))
      strcpy(retvalue, "Automatic");
    else
      sprintf(retvalue, "%s", gcfg.ap_primary_ch);
  }
  else if (!safe_strcmp(type, "channelidx")) 
  {
    int channel,idx,i;

    idx=0;
    if (!safe_strcmp(gcfg.ap_primary_ch, "11ng"))
      idx=0;
    else if (!safe_strcmp(gcfg.ap_primary_ch, "11na"))
      idx=0;
    else 
	{
      channel = strtol(gcfg.ap_primary_ch, NULL, 10);
       
      if (channel <=11)
		idx = channel;
      else {
		for (i=0; i<20; i++) 
		  if (channel == channel5g[i]) {
		    idx = i+1;
		    break;
		  }
	  }
    }
    sprintf(retvalue, "%d", idx);
  }  
  /*advanced setting */
  else if (!safe_strcmp(type, "secmode"))
  {
      switch (mssididx) 
	  {
	      case 0:
	        sprintf(retvalue, "%s", gcfg.ap_secmode);
	        break;
	      case 1:
	        sprintf(retvalue, "%s", gcfg.ap_secmode2);
	        break;
	      case 2:
	        sprintf(retvalue, "%s", gcfg.ap_secmode3);
	        break;
	      case 3:
	        sprintf(retvalue, "%s", gcfg.ap_secmode4);
	        break;
      }
  }
  else if (!safe_strcmp(type, "secmode1"))
  {
    sprintf(retvalue, "%s", gcfg.ap_secmode);
  }
  else if (!safe_strcmp(type, "sectype")) 
  {
    if (!safe_strcmp(gcfg.ap_secmode, "None"))
      strcpy(retvalue, "0");
    else if (!safe_strcmp(gcfg.ap_secmode, "WEP"))
      strcpy(retvalue, "1");
    else {
      if (gcfg.ap_wpa == 1)
		strcpy(retvalue, "2");
      else
		strcpy(retvalue, "3");
    }
  }
  else if (!safe_strcmp(type, "wepmode"))
  {
    sprintf(retvalue, "%d", gcfg.ap_wep_mode_0);
  }
  else if (!safe_strcmp(type, "wepindex")) 
  {
  	sprintf(retvalue, "%d", gcfg.ap_primary_key_0);
  }
  else if (!safe_strcmp(type, "wepvalue")) 
  {
  	int index = gcfg.ap_primary_key_0;

    if (index==1)
      strcpy(retvalue, gcfg.wep_radio_num0_key_1);
    else if (index==2)
      strcpy(retvalue, gcfg.wep_radio_num0_key_2);
    else if (index==3)
      strcpy(retvalue, gcfg.wep_radio_num0_key_3);
    else if (index==4)
      strcpy(retvalue, gcfg.wep_radio_num0_key_4);
  }
  else if (!safe_strcmp(type, "wpamode")) 
  {
    switch (mssididx) 
	{
	    case 0:
	      sprintf(retvalue, "%d", gcfg.ap_wpa);
	      break;
	    case 1:
	      sprintf(retvalue, "%d", gcfg.ap_wpa2);
	      break;
	    case 2:
	      sprintf(retvalue, "%d", gcfg.ap_wpa3);
	      break;
	    case 3:
	      sprintf(retvalue, "%d", gcfg.ap_wpa4);
	      break;
    }
  }
  else if (!safe_strcmp(type, "cypher")) 
  {
    switch (mssididx) 
	{
	    case 0:
	      sprintf(retvalue, "%s", gcfg.ap_cypher);
	      break;
	    case 1:
	      sprintf(retvalue, "%s", gcfg.ap_cypher2);
	      break;
	    case 2:
	      sprintf(retvalue, "%s", gcfg.ap_cypher3);
	      break;
	    case 3:
	      sprintf(retvalue, "%s", gcfg.ap_cypher4);
	      break;
    }
  }
  else if (!safe_strcmp(type, "psk")) 
  {
   switch (mssididx) 
   {
	   case 0:
	     sprintf(retvalue, "%s", gcfg.psk_key);
	     break;
	   case 1:
	     sprintf(retvalue, "%s", gcfg.psk_key2);
	     break;
	   case 2:
	     sprintf(retvalue, "%s", gcfg.psk_key3);
	     break;
	   case 3:
	     sprintf(retvalue, "%s", gcfg.psk_key4);
	     break;
   }
  }
  else if (!safe_strcmp(type, "filter"))
  {
    sprintf(retvalue, "%d", gcfg.n_filter);
  }
  else if (!safe_strcmp(type, "filtermac"))
  {
  	sprintf(retvalue, "%s", gcfg.n_filtermac);
  }
  else if (!safe_strcmp(type, "broadcast"))
  {
  	sprintf(retvalue, "%d", gcfg.n_broadcast);
  }
  else if (!safe_strcmp(type, "beacon"))
  {
  	sprintf(retvalue, "%d", gcfg.n_beacon);
  }
  else if (!safe_strcmp(type, "dtm"))
  {
  	sprintf(retvalue, "%d", gcfg.n_dtm);
  }
  else if (!safe_strcmp(type, "rts"))
  {
  	sprintf(retvalue, "%d", gcfg.n_rts);
  }
  else if (!safe_strcmp(type, "frag"))
  {
  	sprintf(retvalue, "%d", gcfg.n_frag);
  }
  else if (!safe_strcmp(type, "wmm"))
  {
  	sprintf(retvalue, "%d", gcfg.n_wmm);
  }
  else if (!safe_strcmp(type, "gi"))
  {
  	sprintf(retvalue, "%d", gcfg.n_gi);
  }
  else if (!safe_strcmp(type, "igmp"))
  {
  	sprintf(retvalue, "%d", gcfg.n_igmp);
  }
  //multiple ssid
  else if (!safe_strcmp(type, "mssididx")) 
  {
    sprintf(retvalue, "%d", mssididx);
  }
  else if (!safe_strcmp(type, "mssid2")) 
  {
    sprintf(retvalue, "%s", gcfg.ap_mssid2);
  }
  else if (!safe_strcmp(type, "mssid3")) 
  {
    sprintf(retvalue, "%s", gcfg.ap_mssid3);
  }
  else if (!safe_strcmp(type, "mssid4")) 
  {
    sprintf(retvalue, "%s", gcfg.ap_mssid4);
  }
  else if (!safe_strcmp(type, "ssid1"))
  {
    sprintf(retvalue, "%s", gcfg.ap_ssid);
  }
  else if (!safe_strcmp(type, "ssid2"))
  {
  	sprintf(retvalue, "%s", gcfg.ap_ssid2);
  }
  else if (!safe_strcmp(type, "ssid3"))
  {
  	sprintf(retvalue, "%s", gcfg.ap_ssid3);
  }
  else if (!safe_strcmp(type, "ssid4"))
  {
  	sprintf(retvalue, "%s", gcfg.ap_ssid4);
  }
  //connect
  else if (!safe_strcmp(type, "cssid"))
  {
    sprintf(retvalue, "%s", getValue("cssid"));
  }
  else if (!safe_strcmp(type, "csec"))
  {
  	sprintf(retvalue, "%s", getValue("csec"));
  }
  //wps
  else if (!safe_strcmp(type, "wps"))
  {
   sprintf(retvalue, "%d", gcfg.wps_enable);
  }
  else if (!safe_strcmp(type, "wscpin"))
  {
  	sprintf(retvalue, "%s", gcfg.wsc_pin);
  }
  else if (!safe_strcmp(type, "wscname"))
  {
  	sprintf(retvalue, "%s", gcfg.wsc_name);
  }
  else if (!safe_strcmp(type, "apenrollee"))
  {
  	sprintf(retvalue, "%s", gcfg.ap_enrollee);
  }
  else if (!safe_strcmp(type, "clientconn"))
  {
      if(ClientConnected())
		sprintf(retvalue, "<font color=#008000>Connected</font>");
      else 
		sprintf(retvalue, "<font color=#800000>Disconnected</font>");
  }
  else 
    strcpy(retvalue," ");

  kinfo("%s -> %s", type, retvalue);
}

int getWlList(request *wp, char *type)
{
	int nBytesSent=0, i;
	BssDscr *pBss;
	char tmpBuf[100], ssidbuf[40], tmp1Buf[10], tmp2Buf[20], wpa_tkip_aes[20],wpa2_tkip_aes[20];
	WLAN_MODE_T mode;
	bss_info bss;	

	kinfo("Enter");

	if (pStatus==NULL) {
		pStatus = calloc(1, sizeof(SS_STATUS_T));
		if ( pStatus == NULL ) {
			printf("Allocate buffer failed!\n");
			return 0;
		}
	}

	pStatus->number = 0; // request BSS DB

	if ( getWlSiteSurveyResult(WLAN_IF, pStatus) < 0 ) {
		req_format_write(wp, "Read site-survey status failed!");
		free(pStatus); //sc_yang
		pStatus = NULL;
		return 0;
	}

	if ( !apmib_get( MIB_WLAN_MODE, (void *)&mode) ) {
		printf("Get MIB_WLAN_MODE MIB failed!");
		return 0;
	}

	if ( getWlBssInfo(WLAN_IF, &bss) < 0) {
		printf("Get bssinfo failed!");
		return 0;
	}

	int rptEnabled=0;
#if defined(CONFIG_SMART_REPEATER)
	int opmode=0;
	apmib_get(MIB_OP_MODE, (void *)&opmode);
#endif

	if(wlan_idx == 0)
		apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
	else
		apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);
		
	nBytesSent += req_format_write(wp, ("<tr>"
	"<td align=center width=\"30%%\" bgcolor=\"#808080\"><font size=\"2\"><b>SSID</b></font></td>\n"
	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>BSSID</b></font></td>\n"
	"<td align=center width=\"10%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Channel</b></font></td>\n"
      	"<td align=center width=\"20%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Type</b></font></td>\n"
      	"<td align=center width=\"10%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Encrypt</b></font></td>\n"
	"<td align=center width=\"10%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Signal</b></font></td>\n"));
	
	if( (mode == CLIENT_MODE )
#if defined(CONFIG_RTL_ULINKER)
		|| (mode == AP_MODE && rptEnabled == 1)
#endif
#if defined(CONFIG_SMART_REPEATER)
		||( (mode == AP_MODE) && (rptEnabled == 1) 		
	)
#endif
	)
	{
		nBytesSent += req_format_write(wp, ("<td align=center width=\"10%%\" bgcolor=\"#808080\"><font size=\"2\"><b>Select</b></font></td></tr>\n"));
	}
	else
		nBytesSent += req_format_write(wp, ("</tr>\n"));

	for (i=0; i<pStatus->number && pStatus->number!=0xff; i++) {
		pBss = &pStatus->bssdb[i];
		snprintf(tmpBuf, 200, ("%02x:%02x:%02x:%02x:%02x:%02x"),
			pBss->bdBssId[0], pBss->bdBssId[1], pBss->bdBssId[2],
			pBss->bdBssId[3], pBss->bdBssId[4], pBss->bdBssId[5]);

		memcpy(ssidbuf, pBss->bdSsIdBuf, pBss->bdSsId.Length);
		ssidbuf[pBss->bdSsId.Length] = '\0';

		if (pBss->network==BAND_11B)
			strcpy(tmp1Buf, (" (B)"));
		else if (pBss->network==BAND_11G)
			strcpy(tmp1Buf, (" (G)"));	
		else if (pBss->network==(BAND_11G|BAND_11B))
			strcpy(tmp1Buf, (" (B+G)"));
		else if (pBss->network==(BAND_11N))
			strcpy(tmp1Buf, (" (N)"));		
		else if (pBss->network==(BAND_11G|BAND_11N))
			strcpy(tmp1Buf, (" (G+N)"));	
		else if (pBss->network==(BAND_11G|BAND_11B | BAND_11N))
			strcpy(tmp1Buf, (" (B+G+N)"));	
		else if(pBss->network== BAND_11A)
			strcpy(tmp1Buf, (" (A)"));
		else if(pBss->network== (BAND_11A | BAND_11N))
			strcpy(tmp1Buf, (" (A+N)"));	
		else
			sprintf(tmp1Buf, (" -%d-"),pBss->network);

		memset(wpa_tkip_aes,0x00,sizeof(wpa_tkip_aes));
		memset(wpa2_tkip_aes,0x00,sizeof(wpa2_tkip_aes));
		
		if ((pBss->bdCap & cPrivacy) == 0)
			sprintf(tmp2Buf, "no");
		else {
			if (pBss->bdTstamp[0] == 0)
				sprintf(tmp2Buf, "WEP");
#if defined(CONFIG_RTL_WAPI_SUPPORT)
			else if (pBss->bdTstamp[0] == SECURITY_INFO_WAPI)
				sprintf(tmp2Buf, "WAPI");
#endif
			else {
				int wpa_exist = 0, idx = 0;
				if (pBss->bdTstamp[0] & 0x0000ffff) {
					idx = sprintf(tmp2Buf, "WPA");
					if (((pBss->bdTstamp[0] & 0x0000f000) >> 12) == 0x4)
						idx += sprintf(tmp2Buf+idx, "-PSK");
					else if(((pBss->bdTstamp[0] & 0x0000f000) >> 12) == 0x2)
						idx += sprintf(tmp2Buf+idx, "-1X");
					wpa_exist = 1;

					if (((pBss->bdTstamp[0] & 0x00000f00) >> 8) == 0x5)
						sprintf(wpa_tkip_aes,"%s","aes/tkip");
					else if (((pBss->bdTstamp[0] & 0x00000f00) >> 8) == 0x4)
						sprintf(wpa_tkip_aes,"%s","aes");
					else if (((pBss->bdTstamp[0] & 0x00000f00) >> 8) == 0x1)
						sprintf(wpa_tkip_aes,"%s","tkip");
				}
				if (pBss->bdTstamp[0] & 0xffff0000) {
					if (wpa_exist)
						idx += sprintf(tmp2Buf+idx, "/");
					idx += sprintf(tmp2Buf+idx, "WPA2");
					if (((pBss->bdTstamp[0] & 0xf0000000) >> 28) == 0x4)
						idx += sprintf(tmp2Buf+idx, "-PSK");
					else if (((pBss->bdTstamp[0] & 0xf0000000) >> 28) == 0x2)
						idx += sprintf(tmp2Buf+idx, "-1X");

					if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x5)
						sprintf(wpa2_tkip_aes,"%s","aes/tkip");
					else if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x4)
						sprintf(wpa2_tkip_aes,"%s","aes");
					else if (((pBss->bdTstamp[0] & 0x0f000000) >> 24) == 0x1)
						sprintf(wpa2_tkip_aes,"%s","tkip");
				}
			}
		}
		{
			nBytesSent += req_format_write(wp, ("<tr>"
			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><pre><font size=\"2\">%s</td>\n"
			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
			"<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%d%s</td>\n"     
      			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
			"<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%s</td>\n"
			"<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\">%d</td>\n"),
			ssidbuf, tmpBuf, pBss->ChannelNumber, tmp1Buf,
			((pBss->bdCap & cIBSS) ? "Ad hoc" : "AP"), tmp2Buf, pBss->rssi);
		}

		if( ( mode == CLIENT_MODE )
#if defined(CONFIG_RTL_ULINKER)
		|| (mode == AP_MODE && rptEnabled == 1)
#endif
#if defined(CONFIG_SMART_REPEATER)
		||( (mode == AP_MODE) && (rptEnabled == 1) 		
		)
#endif
		)
		{
			nBytesSent += req_format_write(wp,
			("<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"> <input type=\"hidden\" id=\"selSSID_%d\" value=\"%s\" > <input type=\"hidden\" id=\"selChannel_%d\" value=\"%d\" > <input type=\"hidden\" id=\"selEncrypt_%d\" value=\"%s\" > <input type=\"hidden\" id=\"wpa_tkip_aes_%d\" value=\"%s\" > <input type=\"hidden\" id=\"wpa2_tkip_aes_%d\" value=\"%s\" > <input type=\"radio\" name="
			"\"select\" value=\"sel%d\" onClick=\"enableConnect(%d)\"></td></tr>\n"), i,ssidbuf,i,pBss->ChannelNumber,i,tmp2Buf,i,wpa_tkip_aes,i,wpa2_tkip_aes ,i,i);
		}
		else
			nBytesSent += req_format_write(wp, ("</tr>\n"));
	}

	if( pStatus->number == 0 )
	{
		if (( mode == CLIENT_MODE )
#if defined(CONFIG_RTL_ULINKER)
		|| (mode == AP_MODE && rptEnabled == 1)
#endif
#if defined(CONFIG_SMART_REPEATER)
		||( (mode == AP_MODE) && (rptEnabled == 1) 		
		)
#endif
		)
		{
			nBytesSent += req_format_write(wp, ("<tr>"
	                "<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><pre><font size=\"2\">None</td>\n"
	                "<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
	                "<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
	                "<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
	                "<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
	                "<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
	                "<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
	                "</tr>\n"));
		}
		else
		{
			nBytesSent += req_format_write(wp, ("<tr>"
			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><pre><font size=\"2\">None</td>\n"
			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
			"<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
			"<td align=center width=\"20%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
			"<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
			"<td align=center width=\"10%%\" bgcolor=\"#C0C0C0\"><font size=\"2\"></td>\n"
			"</tr>\n"));
		}
	}
	
	nBytesSent += req_format_write(wp, ("</table>\n"));
	return nBytesSent;
}

void scanWirelessAP(request *wp)
{	
	unsigned char res;		
	int wait_time = 0;
	int status = 0;	
	char tmpBuf[100] = {0};	

	wlan_idx = 0;

	while (1) {
		sprintf(WLAN_IF, "wlan%d", wlan_idx);						 
			
		switch(getWlSiteSurveyRequest(WLAN_IF, &status)) 
		{ 
			case -2: 
				kdebug("-2"); 
				strcpy(tmpBuf, ("Auto scan running!!please wait...")); 
				goto setErr_wlan; 
				break; 
			case -1: 
				kdebug("-1"); 
				strcpy(tmpBuf, ("Site-survey request failed!")); 
				goto setErr_wlan; 
				break; 
			default: 
				break; 
		} 

		if (status != 0) {	// not ready
			if (wait_time++ > 5) {
				strcpy(tmpBuf, ("scan request timeout!"));
				goto setErr_wlan;
			}
			sleep(1);
		}
		else
			break;
	}		

	// wait until scan completely
	wait_time = 0;
	while (1) {
		res = 1;	// only request request status
		if ( getWlSiteSurveyResult(WLAN_IF, (SS_STATUS_Tp)&res) < 0 ) {
			strcpy(tmpBuf, ("Read site-survey status failed!"));
			free(pStatus);
			pStatus = NULL;
			goto setErr_wlan;
		}
		if (res == 0xff) {	 // in progress
			if (wait_time++ > 10) 			
			{
				strcpy(tmpBuf, ("scan timeout!"));
				free(pStatus);
				pStatus = NULL;
				goto setErr_wlan;
			}
			sleep(1);
		}
		else
			break;
	}	

setErr_wlan:
	ERR_MSG(tmpBuf);
}

void applyPage(request *wp)
{
	char *strVal = NULL;

#ifdef ACTIONTEC_MULTIACCESS
	struct http_session *hs = NULL; 
	hs = er_http_session_lookup(wp->session_id);

	if(hs != NULL)
	{
		hs->both_freq = both_freq;
		hs->wlan_idx = wlan_idx;
		hs->curr_ssid = curr_ssid;
	}
#endif

	run_init_script_flag = 1;			
#ifndef NO_ACTION
	run_init_script("all");
#endif	
	needReboot = 0;
	
	countDownTime = WIRELESS_SETTING_COUNTDOWN_TIME;
	
	strVal = req_get_cstream_var(wp, ("mypage"), "");	
	if(strVal[0])	
		sprintf(lastUrl, "%s", strVal);
	else
		strncpy(lastUrl, "/wcb_main.html", 100);
	
	send_redirect_perm(wp, COUNTDOWN_PAGE);	
}

int clientConnect(request *wp, char *buf)
{
	char *strSSID = NULL;
	char wlanifp[16] = {0}; 
	char wlanifp_bak[32] = {0};
	char command[50] = {0};
	int rptEnabled=0;
	int wlan_mode=0;		
	int opmode=0;
	int vxd_wisp_wan=0;	
	int wait_time = 0, max_wait_time = 5;
	NETWORK_TYPE_T net;
	int chan = 0;
	int is_40m_bw = 0;
	int status = 0;
	unsigned char res = 0;
	ENCRYPT_T encrypt = ENCRYPT_DISABLED;

	kinfo("Enter");	

	if(wlan_idx == 0)
		apmib_get(MIB_REPEATER_ENABLED1, (void *)&rptEnabled);
	else
		apmib_get(MIB_REPEATER_ENABLED2, (void *)&rptEnabled);		
		
	apmib_get(MIB_WLAN_MODE, (void *)&wlan_mode);
	apmib_get(MIB_OP_MODE, (void *)&opmode);

	if( (wlan_mode == AP_MODE) && (rptEnabled == 1) )
		vxd_wisp_wan = 1;

	sprintf(wlanifp, "wlan%d", wlan_idx);

	if(vxd_wisp_wan == 1)
	{
		sprintf(wlanifp_bak, "%s", wlanifp);
		sprintf(wlanifp, "%s-vxd", wlanifp_bak);
	}	

	SetWlan_idx(wlanifp);

	strSSID = req_get_cstream_var(wp, ("wl_ssid"), "");
	apmib_set(MIB_WLAN_SSID, (void *)strSSID);

	if(vxd_wisp_wan == 1)
		if(wlan_idx == 0)
			apmib_set(MIB_REPEATER_SSID1, (void *)strSSID);
		else
			apmib_set(MIB_REPEATER_SSID2, (void *)strSSID);

	if (client_select_id) {	
		if (pStatus == NULL) {
			strcpy(buf, ("Please refresh again!"));
			goto setErr_wlan;
		}
		
		if ( client_select_id >= pStatus->number ) { // invalid index
			strcpy(buf, ("Connect failed 1!"));
			goto setErr_wlan;
		}	

		if ( pStatus->bssdb[client_select_id].bdCap & cESS )
			net = INFRASTRUCTURE;
		else
			net = ADHOC;
				
		if ( apmib_set(MIB_WLAN_NETWORK_TYPE, (void *)&net) == 0) {
			strcpy(buf, ("Set MIB_WLAN_NETWORK_TYPE failed!"));
			goto setErr_wlan;
		}
				
		if (net == ADHOC) {
			chan = pStatus->bssdb[client_select_id].ChannelNumber;
			if ( apmib_set( MIB_WLAN_CHANNEL, (void *)&chan) == 0) {
				strcpy(buf, ("Set channel number error!"));
				goto setErr_wlan;
			}

			is_40m_bw = (pStatus->bssdb[client_select_id].bdTstamp[1] & 2) ? 1 : 0; 			
			apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&is_40m_bw);				
		}
	}
		
	apmib_update_web(CURRENT_SETTING);		

	countDownTime = WIRELESS_CLIENTCONNECT_COUNTDOWN_TIME;
	send_redirect_perm(wp, COUNTDOWN_PAGE);

	if(vxd_wisp_wan){
		sprintf(command,"ifconfig %s-vxd down", wlanifp);			
		system(command);
	}
	
	sprintf(command,"ifconfig %s down", wlanifp);		
	system(command);
	sprintf(command,"flash set_mib %s", wlanifp);		\
	system(command);
	sprintf(command,"ifconfig %s up", wlanifp); 	
	system(command);
	
	if(vxd_wisp_wan){
		sprintf(command,"flash set_mib %s-vxd", wlanifp); 
		system(command);
		sprintf(command,"ifconfig %s-vxd up", wlanifp); 		
		system(command);
	}
	
	sleep(3);
	system("sysconf wlanapp start wlan0 br0");		
	sleep(1);

	/*sysconf upnpd 1(isgateway) 1(opmode is bridge)*/
	system("sysconf upnpd 1 1");		
	sleep(1);
	
	res = client_select_id;
	wait_time = 0;
	
	while (1) 
	{
		if(vxd_wisp_wan && !strstr(WLAN_IF, "-vxd"))
			strcat(WLAN_IF,"-vxd"); 					

		if ( getWlJoinResult(WLAN_IF, &res) < 0 ) {
			strcpy(buf, ("Get Join result failed!"));
			goto setErr_wlan;
		}
	
		if(res==STATE_Bss  || res==STATE_Ibss_Idle || res==STATE_Ibss_Active) { // completed 
			break;
		}
		else
		{
			if (wait_time++ > 10) {
				strcpy(buf, ("connect timeout!"));
				goto setErr_wlan;
			}
		}
	
		sleep(1);
	}
	
	if ( res!=STATE_Bss && res!=STATE_Ibss_Idle && res!=STATE_Ibss_Active )
	{
		strcpy(buf, ("Connect failed 2!"));
		goto setErr_wlan;
	}
	else {					
		status = 0; 		
		apmib_get( MIB_WLAN_ENCRYPT, (void *)&encrypt);
			
		if (encrypt == ENCRYPT_WPA || encrypt == ENCRYPT_WPA2 || encrypt == ENCRYPT_WAPI) {
			bss_info bss;
			wait_time = 0;
						
			max_wait_time = 15; //Need more test, especially for 802.1x client mode
						
			while (wait_time++ < max_wait_time) {
				getWlBssInfo(WLAN_IF, &bss);
				if (bss.state == STATE_CONNECTED){
					break;
				}
				sleep(1);
			}
	
			if (wait_time > max_wait_time)						
				status = 1; //fail
		}
	
		if (status)
		{				
			strcpy(buf, ("Connect failed 3!"));
			goto setErr_wlan;
		}
		else
		{
			kinfo("Connect successfully!");
		}
	} 
	
#ifndef NO_ACTION
	run_init_script("all");
#endif

	if(vxd_wisp_wan)
	{
		SetWlan_idx(wlanifp_bak);			
	}	
	
	return 0;
	
setErr_wlan:
	return 1;
}

void actWlquicksetup(request *wp, char *path, char *query)
{
	char *strVal = NULL;
	char *wl_psk = NULL;
	char tmpBuf[100] = {0};	
	char strtmp[80] = {0};
	char key[30] = {0};
	
	int disabled = 0;	
	int band_no = 0;
	int val = 0;
	int mode = -1;	
	int chan = 0;
	int wsc_auth = 1;		
	int format = 0;
	int use1x = 0;
	int wlan_onoff_tkip = 0;	
	int wpaCipher = 0;
	int wpa2Cipher = 0;
	int wdsEncrypt = 0;
	int wlan_encrypt = 0;
	int ret = 0;	
	int wsc_configured = 1;
	int keyType = 1;
	int coexist = 0; 
	ENCRYPT_T encrypt = ENCRYPT_DISABLED;
	int wsc_enc = WSC_ENCRYPT_TKIP;
	int wpa_cipher = WPA_CIPHER_TKIP;
	int intVal = WEP64;
	int authType = AUTH_OPEN;	
	int keyLen = WEP64_KEY_LEN * 2;	

	wlan_idx = 0;
	vwlan_idx = 0;

	kinfo("Enter");

	strVal = req_get_cstream_var(wp, ("wl_mode"), "");
	if(strVal[0] == 0)
	{		
		apmib_get( MIB_WLAN_MODE, (void *)&val);
		sprintf(strtmp, "%d", val);
		strVal = strtmp;		
	}
		
	if(strVal[0]) 
	{		
		kinfo("wl_mode: %s", strVal);
		if (strVal[0]!= '0' && strVal[0]!= '1' && strVal[0]!= '2' &&  strVal[0]!= '3')
		{
			strcpy(tmpBuf, ("Invalid mode value!"));
			goto setErr_wlan;
		}
			
		mode = strVal[0] - '0';
		kinfo("mode: %d", mode);
			
		if (mode == CLIENT_MODE) {
	  		apmib_get( MIB_WLAN_ENCRYPT, (void *)&encrypt);
			if (encrypt &  ENCRYPT_WPA2_MIXED) {
				apmib_get( MIB_WLAN_WPA_AUTH, (void *)&format);
				if (format & 1) { // radius
					strcpy(tmpBuf, ("You cannot set client mode with Enterprise (RADIUS) !<br><br>Please change the encryption method in security page first."));
					goto setErr_wlan;
				}
			}
			else if (encrypt == ENCRYPT_WEP || encrypt == 0) {
				apmib_get( MIB_WLAN_ENABLE_1X, (void *)&use1x);
				if (use1x & 1) {
					strcpy(tmpBuf, ("You cannot set client mode with 802.1x enabled!<br><br>Please change the encryption method in security page first."));
					goto setErr_wlan;
				}
			}			
		}

		if ( apmib_set( MIB_WLAN_MODE, (void *)&mode) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_MODE error!"));
			goto setErr_wlan;
		}
	}	

	strVal = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", strVal);
	
	if(!safe_strcmp(strVal, "apply"))
	{
		strVal = req_get_cstream_var(wp, ("wl_enb"), "");	
		if (strVal[0]) 
		{
			kinfo("wl_enb: %s", strVal);
			
			if ( !safe_strcmp(strVal, "ON"))
				disabled = 0;
			else
				disabled = 1;

			if ( apmib_set( MIB_WLAN_WLAN_DISABLED, (void *)&disabled) == 0) {
		  		strcpy(tmpBuf, ("Set disabled flag error!"));
				goto setErr_wlan;
			}		
		}	

		strVal = req_get_cstream_var(wp, ("wl_freq"), "");	
		if (strVal[0]) 
		{
			kinfo("wl_freq: %s", strVal);

			band_no = strtol(strVal, (char **)NULL, 10);
			if (band_no < 0 || band_no > 19) {
	  			strcpy(tmpBuf, ("Invalid band value!"));
				goto setErr_wlan;
			}
			
			apmib_get( MIB_WLAN_11N_ONOFF_TKIP, (void *)&wlan_onoff_tkip);

			kinfo("wlan_onoff_tkip: %d", wlan_onoff_tkip);
			
			if(wlan_onoff_tkip == 0) //Wifi request
			{			
				apmib_get( MIB_WLAN_ENCRYPT, (void *)&wlan_encrypt);
				apmib_get( MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpaCipher);
				apmib_get( MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa2Cipher);
				apmib_get( MIB_WLAN_WDS_ENCRYPT, (void *)&wdsEncrypt);
				
				if(mode != CLIENT_MODE && (band_no == 7 || band_no == 9 || band_no == 10 || band_no == 11)) //7:n; 9:gn; 10:bgn 11:5g_an
				{				
					if(wlan_encrypt == ENCRYPT_WPA || wlan_encrypt == ENCRYPT_WPA2)
					{
						wpaCipher &= ~WPA_CIPHER_TKIP;
						if(wpaCipher == 0)
							wpaCipher =  WPA_CIPHER_AES;
						apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpaCipher);
					
						wpa2Cipher &= ~WPA_CIPHER_TKIP;
						if(wpa2Cipher == 0)
							wpa2Cipher =  WPA_CIPHER_AES;
						apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa2Cipher);
					}
					
					if(wdsEncrypt == WDS_ENCRYPT_TKIP)
					{
						wdsEncrypt = WDS_ENCRYPT_DISABLED;
						apmib_set( MIB_WLAN_WDS_ENCRYPT, (void *)&wdsEncrypt);
					}
				}		
			}

			if(band_no == 10)
				wlan_idx = 1;
			
			val = (band_no + 1);
			if ( apmib_set( MIB_WLAN_BAND, (void *)&val) == 0) {
	   			strcpy(tmpBuf, ("Set band error!"));
				goto setErr_wlan;
			}
		}

		strVal = req_get_cstream_var(wp, ("wl_ssid"), "");
		if (strVal[0]) {
			kinfo("wl_ssid: %s", strVal);
			if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	   	 		strcpy(tmpBuf, ("Set SSID error!"));
				goto setErr_wlan;
			}
			if(apmib_set(MIB_WLAN_WSC_SSID, (void *)&strVal) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WSC_SSID mib error!"));
				goto setErr_wlan;
			}
		}
		else if (mode == 1 && !strVal[0]) { // client and NULL SSID		
			if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	   	 		strcpy(tmpBuf, ("Set SSID error!"));
				goto setErr_wlan;
			}
			if(apmib_set(MIB_WLAN_WSC_SSID, (void *)&strVal) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WSC_SSID mib error!"));
				goto setErr_wlan;
			}
		}	
		
		strVal = req_get_cstream_var(wp, ("wl_chanwidth"), "");
		if (strVal[0]) {
			kinfo("wl_chanwidth: %s", strVal);
			if ( strVal[0] == '0')
				val = 0;
			else if (strVal[0] == '1')
				val = 1;
			else if (strVal[0] == '2')
				val = 2;
			else {
				strcpy(tmpBuf, ("Error! Invalid Channel Bonding."));
				goto setErr_wlan;
			}

			if(val != 2)
			{
				if (apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&val) == 0) {
					strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL_BONDING failed!"));
					goto setErr_wlan;
				}				
			}
			else if(val == 2)
			{
				coexist = 1;				
			}

			if (apmib_set(MIB_WLAN_COEXIST_ENABLED, (void *)&coexist) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_COEXIST_ENABLED failed!"));
				goto setErr_wlan;
			}			
		}

		wl_psk = req_get_cstream_var(wp, ("wl_psk"), "");
		if (wl_psk[0]) {
			kinfo("wl_psk: %s", wl_psk);		
			
			strVal = req_get_cstream_var(wp, ("wl_security"), "");
			if (strVal[0]) {
				kinfo("wl_security: %s", strVal);

				encrypt = (ENCRYPT_T) strVal[0] - '0';			
				kinfo("encrypt: %d", encrypt);
				
				if(encrypt==ENCRYPT_DISABLED){
					wsc_enc = WSC_ENCRYPT_NONE;
				} 
				else if (encrypt==ENCRYPT_WEP) {				
					wsc_enc = WSC_ENCRYPT_WEP;

					if (!apmib_set(MIB_WLAN_WEP, (void *)&intVal)) {
						strcpy(tmpBuf, ("Set MIB_WLAN_WEP error!"));
						goto setErr_wlan;
					}

					if (!apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType)) {
						strcpy(tmpBuf, ("Set MIB_WLAN_AUTH_TYPE error!"));
						goto setErr_wlan;
					}
					
					if (!apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&keyType)) {
						strcpy(tmpBuf, ("Set WEP key type error!"));
						goto setErr_wlan;
					}

					if ( !string_to_hex(wl_psk, key, keyLen)) {
						strcpy(tmpBuf, ("Invalid wep-key value!"));
						goto setErr_wlan;
					}
					
					ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
					ret=apmib_set(MIB_WLAN_WEP64_KEY2, (void *)key);
					ret=apmib_set(MIB_WLAN_WEP64_KEY3, (void *)key);
					ret=apmib_set(MIB_WLAN_WEP64_KEY4, (void *)key);

					if (!ret) {
			 			strcpy(tmpBuf, ("Set wep-key error!"));
						goto setErr_wlan;
					}
				}
				else if((encrypt==ENCRYPT_WPA) || (encrypt==ENCRYPT_WPA2)){

					if(encrypt==ENCRYPT_WPA)
					{
						wsc_auth = 2;						
						if (apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_cipher) == 0) {
							strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
							goto setErr_wlan;							
						}						
					}
					else if(encrypt==ENCRYPT_WPA2)
					{
						wsc_auth = 32;
						if (apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa_cipher) == 0) {
							strcpy(tmpBuf, ("Set MIB_WLAN_WPA2_CIPHER_SUITE failed!"));
							goto setErr_wlan;							
						}
					}
					
					if ( !apmib_set(MIB_WLAN_WPA_PSK, (void *)wl_psk)) {
						strcpy(tmpBuf, ("Set MIB_WLAN_WPA_PSK error!"));
						goto setErr_wlan;
					}

					if ( !apmib_set(MIB_WLAN_WSC_PSK, (void *)wl_psk)) {
						strcpy(tmpBuf, ("Set MIB_WLAN_WSC_PSK error!"));
						goto setErr_wlan;
					}				
				}			
				
				if (apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt) == 0) {
					strcpy(tmpBuf, ("Set MIB_WLAN_ENCRYPT mib error!"));
					goto setErr_wlan;
				}		
			
				if (apmib_set( MIB_WLAN_WSC_ENC, (void *)&wsc_enc) == 0) {
					strcpy(tmpBuf, ("Set MIB_WLAN_WSC_ENC mib error!"));
					goto setErr_wlan;
				}					
			}
			
			if(apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&wsc_configured) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WSC_CONFIGURED mib error!"));
				goto setErr_wlan;
			}

			if ( !apmib_set(MIB_WLAN_WSC_AUTH, (void *)&wsc_auth)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WSC_AUTH error!"));
				goto setErr_wlan;
			}
		}	
		
		strVal = req_get_cstream_var(wp, ("wl_channel"), "");
		if (strVal[0]) {
			kinfo("wl_channel: %s", strVal);
			errno = 0;
			chan = strtol( strVal, (char **)NULL, 10);
			if (errno) {
	   			strcpy(tmpBuf, ("Invalid channel number!"));
				goto setErr_wlan;
			}
			
			if ( apmib_set( MIB_WLAN_CHANNEL, (void *)&chan) == 0) {
	   			strcpy(tmpBuf, ("Set channel number error!"));
				goto setErr_wlan;
			}
		}	

		apmib_update_web(CURRENT_SETTING);

		if (mode == CLIENT_MODE) {			
			if(!clientConnect(wp, tmpBuf))
			{	
				return;
			}
			else
				goto setErr_wlan;			
		}	
		
		applyPage(wp);
		
	}
	else if(!safe_strcmp(strVal, "scan"))
	{		
		scanWirelessAP(wp);
		nothankyouPage(wp);		
	}

	return;

setErr_wlan:
	ERR_MSG(tmpBuf);
}

void actWllist(request *wp, char *path, char *query)
{
	char *strVal = NULL;	
	char tmpBuf[100] = {0};	

	kinfo("Enter");

	strVal = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", strVal);	

	if(!safe_strcmp(strVal, "refresh"))
	{
		scanWirelessAP(wp);			
	}
	else if(!safe_strcmp(strVal, "connect"))
	{		
		strVal = req_get_cstream_var(wp, ("selectID"), "");
		if(strVal[0]) {
			kinfo("selectID: %s", strVal);	
			client_select_id = atoi(strVal);
		}
	
		strVal = req_get_cstream_var(wp, ("SSID"), "");
		if(strVal[0]) {
			kinfo("SSID: %s", strVal);	

			if ((client_ssid = (char *) malloc(strlen(strVal) + 1)) != NULL)
			{
				memset(client_ssid, 0, strlen(strVal) + 1);
				strncpy(client_ssid, strVal, strlen(strVal)); 
			}
		}
		
		strVal = req_get_cstream_var(wp, ("Channel"), "");
		if(strVal[0]) {
			kinfo("Channel: %s", strVal);	
			client_channel = atoi(strVal);
		}
		
		strVal = req_get_cstream_var(wp, ("Encrypt"), "");		
		if(strVal[0]) {
			kinfo("Encrypt: %s", strVal);		
			client_encrypt = atoi(strVal);		
		}

		strVal = req_get_cstream_var(wp, ("Wpa_tkip_aes"), "");		
		if(strVal[0]) {
			kinfo("Wpa_tkip_aes: %s", strVal);		
			client_wpa_tkip_aes = atoi(strVal);			
		}

		strVal = req_get_cstream_var(wp, ("Wpa2_tkip_aes"), "");
		if(strVal[0]) {
			kinfo("Wpa2_tkip_aes: %s", strVal);	
			client_wpa2_tkip_aes = atoi(strVal);			
		}	
	}

	nothankyouPage(wp);	
	return;
	
setErr_wlan:
	ERR_MSG(tmpBuf);
}

void actWlconfig(request *wp, char *path, char *query)
{
	// ap
	char *strVal = NULL;
	char *action = NULL;
	char *wl_mssididx = NULL;
	char *wl_psk = NULL;	
	char buf[128] = {0};
	char strtmp[80] = {0};
	char tmpBuf[100] = {0};	
	char key[30] = {0};	
	int mode = -1;
	int ret = 0;	
	int disabled = 0;
	int val = 0;	
	int format = 0;
	int use1x = 0;
	int wsc_auth = 1;	
	int defaultKey = 0;
	int wsc_configured = 1;	
	int keyType = 1;
	ENCRYPT_T encrypt = ENCRYPT_DISABLED;
	int wsc_enc = WSC_ENCRYPT_TKIP;
	int wpa_cipher = WPA_CIPHER_TKIP;
	int intVal = WEP64;
	int keyLen = WEP64_KEY_LEN * 2;
	int authType = AUTH_OPEN;		

	kinfo("Enter");

	wl_mssididx = req_get_cstream_var(wp, ("wl_mssididx"), "");
	kinfo("wl_mssididx: %s", wl_mssididx);

	mssid_idx = atoi(wl_mssididx);	
	vwlan_idx = mssid_idx;

	strVal = req_get_cstream_var(wp, ("wl_mode"), "");		
	if(strVal[0] == 0)
	{			
		apmib_get( MIB_WLAN_MODE, (void *)&val);
		sprintf(strtmp, "%d", val);
		strVal = strtmp;		
	}
		
	if (strVal[0]) 
	{		
		kinfo("wl_mode: %s", strVal);
		if (strVal[0]!= '0' && strVal[0]!= '1' && strVal[0]!= '2' &&  strVal[0]!= '3')
		{
			strcpy(tmpBuf, ("Invalid mode value!"));
			goto setErr_wlan;
		}
			
		mode = strVal[0] - '0';
		kinfo("mode: %d", mode);
			
		if (mode == CLIENT_MODE) {
			apmib_get( MIB_WLAN_ENCRYPT,  (void *)&encrypt);
			if (encrypt &  ENCRYPT_WPA2_MIXED) {
				apmib_get( MIB_WLAN_WPA_AUTH, (void *)&format);
				if (format & 1) { // radius
					strcpy(tmpBuf, ("You cannot set client mode with Enterprise (RADIUS) !<br><br>Please change the encryption method in security page first."));
					goto setErr_wlan;
				}
			}
			else if (encrypt == ENCRYPT_WEP || encrypt == 0) {
				apmib_get( MIB_WLAN_ENABLE_1X, (void *)&use1x);
				if (use1x & 1) {
					strcpy(tmpBuf, ("You cannot set client mode with 802.1x enabled!<br><br>Please change the encryption method in security page first."));
					goto setErr_wlan;
				}
			}
		}
	
		if ( apmib_set( MIB_WLAN_MODE, (void *)&mode) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_MODE error!"));
			goto setErr_wlan;
		}
	}		
	
	action = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", action);

	if(!safe_strcmp(action, "changeSSID"))
	{	
		apmib_update_web(CURRENT_SETTING);

#ifndef NO_ACTION
		run_init_script("bridge");
#endif	
		snprintf(buf, 128, "/wl_config.html?ssid=%s", wl_mssididx);		
		send_redirect_perm(wp, buf);
		return;
	}
	else if(!safe_strcmp(action, "apply"))
	{		
		if ( apmib_set( MIB_WLAN_WLAN_DISABLED, (void *)&disabled) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED error!"));
			goto setErr_wlan;
		}			

		strVal = req_get_cstream_var(wp, ("wl_ssid"), "");
		if (strVal[0]) {
			kinfo("wl_ssid: %s", strVal);
			if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	   	 		strcpy(tmpBuf, ("Set SSID error!"));
				goto setErr_wlan;
			}
			if (apmib_set(MIB_WLAN_WSC_SSID, (void *)strVal) == 0) {
	   	 		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_SSID error!"));
				goto setErr_wlan;
			}
		}
		else if (mode == 1 && !strVal[0] ) { // client and NULL SSID		
			if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	   	 		strcpy(tmpBuf, ("Set SSID error!"));
				goto setErr_wlan;
			}
			if (apmib_set(MIB_WLAN_WSC_SSID, (void *)strVal) == 0) {
	   	 		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_SSID error!"));
				goto setErr_wlan;
			}
		}		
	}
	else if(!safe_strcmp(action, "scan"))
	{
		scanWirelessAP(wp);
		nothankyouPage(wp);
		return;
	}	

	if(!safe_strcmp(action, "apply") || (!safe_strcmp(action, "client_connect")))
	{		
		strVal = req_get_cstream_var(wp, ("AP_SECMODE"), "");
		if (strVal[0]) {
			kinfo("AP_SECMODE: %s", strVal);

			encrypt = (ENCRYPT_T) strVal[0] - '0';				
			kinfo("encrypt: %d", encrypt);
				
			if(encrypt == ENCRYPT_DISABLED){
				wsc_enc = WSC_ENCRYPT_NONE;
			} 
			else if (encrypt == ENCRYPT_WEP) {					
				wsc_enc = WSC_ENCRYPT_WEP;

				strVal = req_get_cstream_var(wp, ("AP_WEP_MODE_0"), "");
				if (strVal[0]) {
					kinfo("AP_WEP_MODE_0: %s", strVal);
					authType = atoi(strVal);
				}
				
				if (!apmib_set(MIB_WLAN_WEP, (void *)&intVal)) {
					strcpy(tmpBuf, ("Set MIB_WLAN_WEP error!"));
					goto setErr_wlan;
				}

				if (!apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType)) {
					strcpy(tmpBuf, ("Set MIB_WLAN_AUTH_TYPE error!"));
					goto setErr_wlan;
				}
					
				if (!apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&keyType)) {
					strcpy(tmpBuf, ("Set WEP key type error!"));
					goto setErr_wlan;
				}

				strVal = req_get_cstream_var(wp, ("AP_PRIMARY_KEY_0"), "");
				if(strVal[0]) {
					kinfo("AP_PRIMARY_KEY_0: %s", strVal);
					if(!safe_strcmp(strVal, "1"))
					{
						wl_psk = req_get_cstream_var(wp, ("WEP_RADIO_NUM0_KEY_1"), "");
						if(wl_psk[0]) {
							kinfo("WEP_RADIO_NUM0_KEY_1: %s", wl_psk);

							if ( !string_to_hex(wl_psk, key, keyLen)) {
				   				strcpy(tmpBuf, ("Invalid wep-key1 value!"));
								goto setErr_wlan;
							}
							
							ret=apmib_set(MIB_WLAN_WEP64_KEY1, (void *)key);
						}
					}
					else if(!safe_strcmp(strVal, "2"))
					{
						wl_psk = req_get_cstream_var(wp, ("WEP_RADIO_NUM0_KEY_2"), "");
						if(wl_psk[0]) {
							kinfo("WEP_RADIO_NUM0_KEY_2: %s", wl_psk);	

							if ( !string_to_hex(wl_psk, key, keyLen)) {
				   				strcpy(tmpBuf, ("Invalid wep-key2 value!"));
								goto setErr_wlan;
							}
							
							ret=apmib_set(MIB_WLAN_WEP64_KEY2, (void *)key);
						}
					}
					else if(!safe_strcmp(strVal, "3"))
					{
						wl_psk = req_get_cstream_var(wp, ("WEP_RADIO_NUM0_KEY_3"), "");
						if(wl_psk[0]) {
							kinfo("WEP_RADIO_NUM0_KEY_3: %s", wl_psk);	

							if ( !string_to_hex(wl_psk, key, keyLen)) {
				   				strcpy(tmpBuf, ("Invalid wep-key3 value!"));
								goto setErr_wlan;
							}
							
							ret=apmib_set(MIB_WLAN_WEP64_KEY3, (void *)key);
						}
					}
					else if(!safe_strcmp(strVal, "4"))
					{
						wl_psk = req_get_cstream_var(wp, ("WEP_RADIO_NUM0_KEY_4"), "");
						if(wl_psk[0]) {
							kinfo("WEP_RADIO_NUM0_KEY_4: %s", wl_psk);

							if ( !string_to_hex(wl_psk, key, keyLen)) {
				   				strcpy(tmpBuf, ("Invalid wep-key4 value!"));
								goto setErr_wlan;
							}
							
							ret=apmib_set(MIB_WLAN_WEP64_KEY4, (void *)key);
						}
					}

					defaultKey = atoi(strVal) - 1;
					
					if ( !apmib_set(MIB_WLAN_WEP_DEFAULT_KEY, (void *)&defaultKey)) {
						strcpy(tmpBuf, ("Set MIB_WLAN_WEP_DEFAULT_KEY error!"));
						goto setErr_wlan;
					}
				}
				
				if (!ret) {
		 			strcpy(tmpBuf, ("Set wep-key error!"));
					goto setErr_wlan;
				}
			}
			else if(encrypt == ENCRYPT_WPA){

				wl_psk = req_get_cstream_var(wp, ("PSK_KEY"), "");
				
				if(wl_psk[0]) {
					kinfo("PSK_KEY: %s", wl_psk);
					
					strVal = req_get_cstream_var(wp, ("AP_WPA"), "");
					if(strVal[0]) {
						
						kinfo("AP_WPA: %s", strVal);
						if(!safe_strcmp(strVal, "1"))
						{
							wsc_auth = 2;							
						}
						else if(!safe_strcmp(strVal, "2"))
						{
							encrypt = ENCRYPT_WPA2;
							wsc_auth = 32;							
						}
						else if(!safe_strcmp(strVal, "3"))
						{
							encrypt = ENCRYPT_WPA2_MIXED;
							wsc_auth = 34;							
						}

						if ( !apmib_set(MIB_WLAN_WPA_PSK, (void *)wl_psk)) {
							strcpy(tmpBuf, ("Set MIB_WLAN_WPA_PSK error!"));
							goto setErr_wlan;
						}

						if ( !apmib_set(MIB_WLAN_WSC_PSK, (void *)wl_psk)) {
							strcpy(tmpBuf, ("Set MIB_WLAN_WSC_PSK error!"));
							goto setErr_wlan;
						}
					}

					strVal = req_get_cstream_var(wp, ("AP_CYPHER"), "");
					if(strVal[0]) {
						kinfo("AP_CYPHER: %s", strVal);
						wpa_cipher = atoi(strVal);						
						if ( apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_cipher) == 0) {
							strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
							goto setErr_wlan;							
						}

						if ( apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa_cipher) == 0) {
							strcpy(tmpBuf, ("Set MIB_WLAN_WPA2_CIPHER_SUITE failed!"));
							goto setErr_wlan;							
						}

						if(wpa_cipher == 1)
							wsc_enc =  WSC_ENCRYPT_TKIP;
						else if(wpa_cipher == 2)
							wsc_enc =  WSC_ENCRYPT_AES;
						else if(wpa_cipher == 3)
							wsc_enc =  WSC_ENCRYPT_TKIPAES;
					}
				}
			}		

			if ( !apmib_set(MIB_WLAN_WSC_AUTH, (void *)&wsc_auth)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WSC_AUTH error!"));
				goto setErr_wlan;
			}
				
			if(apmib_set(MIB_WLAN_ENCRYPT, (void *)&encrypt) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_ENCRYPT mib error!"));
				goto setErr_wlan;
			}		

			if(apmib_set(MIB_WLAN_WSC_ENC, (void *)&wsc_enc) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WSC_ENC mib error!"));
				goto setErr_wlan;
			}
			
			if(apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&wsc_configured) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WSC_CONFIGURED mib error!"));
				goto setErr_wlan;
			}	
		}				
	}

	apmib_update_web(CURRENT_SETTING);

	if(!safe_strcmp(action, "apply")) 
	{		
		applyPage(wp);
		return;
	}
	else if(!safe_strcmp(action, "client_connect"))
	{			
		if(!clientConnect(wp, tmpBuf))
		{
			send_redirect_perm(wp, HOME_PAGE);		
			return;
		}
		else
			goto setErr_wlan;		
	}	
	
setErr_wlan:
	ERR_MSG(tmpBuf);
}

void actWlmultiplessid(request *wp, char *path, char *query)
{
	char *strVal = NULL;	
	char tmpBuf[100] = {0};	
	int mode = 0;
	int disabled = 0;

	kinfo("Enter");

	if(apmib_get(MIB_WLAN_MODE, (void *)&mode) == 0)
	{
		strcpy(tmpBuf, ("Get MIB_WLAN_MODE mib error!"));
		goto setErr_wlan;
	}

	vwlan_idx = 1;
	
	strVal = req_get_cstream_var(wp, ("ssid"), "");	
	if (strVal[0]) {
		kinfo("ssid: %s", strVal);
		if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	   		strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}		
	}
	else if (mode == 1 && !strVal[0] ) { // client and NULL SSID		
		if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	 		strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}		
	}
	
	vwlan_idx = 2;
	
	strVal = req_get_cstream_var(wp, ("mssid2"), "");
	if (strVal[0]) {
		kinfo("mssid2: %s", strVal);
		disabled = atoi(strVal);

		if (apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&disabled) == 0) {
		   	strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED error!"));
			goto setErr_wlan;
		}
	}

	strVal = req_get_cstream_var(wp, ("ssid2"), "");
	if (strVal[0]) {
		kinfo("ssid2: %s", strVal);
		if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	   		strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}
	}
	else if (mode == 1 && !strVal[0] ) { // client and NULL SSID		
		if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	 		strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}
	}

	vwlan_idx = 3;

	strVal = req_get_cstream_var(wp, ("mssid3"), "");
	if (strVal[0]) {
		kinfo("mssid3: %s", strVal);
		disabled = atoi(strVal);

		if (apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&disabled) == 0) {
		   	strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED error!"));
			goto setErr_wlan;
		}
	}

	strVal = req_get_cstream_var(wp, ("ssid3"), "");
	if (strVal[0]) {
		kinfo("ssid3: %s", strVal);
		if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	   		strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}
	}
	else if (mode == 1 && !strVal[0] ) { // client and NULL SSID		
		if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	 		strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}
	}

	vwlan_idx = 4;

	strVal = req_get_cstream_var(wp, ("mssid4"), "");
	if (strVal[0]) {
		kinfo("mssid4: %s", strVal);
		disabled = atoi(strVal);

		if (apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&disabled) == 0) {
		   	strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED error!"));
			goto setErr_wlan;
		}
	}

	strVal = req_get_cstream_var(wp, ("ssid4"), "");
	if (strVal[0]) {
		kinfo("ssid4: %s", strVal);
		if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	   		strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}
	}
	else if (mode == 1 && !strVal[0] ) { // client and NULL SSID		
		if (apmib_set(MIB_WLAN_SSID, (void *)strVal) == 0) {
	 		strcpy(tmpBuf, ("Set SSID error!"));
			goto setErr_wlan;
		}
	}

	apmib_update_web(CURRENT_SETTING);
	applyPage(wp);	
	return;

setErr_wlan:
	ERR_MSG(tmpBuf);
}

void actWlwps(request *wp, char *path, char *query)
{
	char *strVal = NULL;	
	char tmpBuf[100] = {0};	
	char pincodestr_b[20] = {0};	
	int intVal = 0;
	int idx = 0, idx2 = 0;	

	kinfo("Enter");

	strVal = req_get_cstream_var(wp, ("command"), "");	
	kinfo("command: %s", strVal);
	if(!safe_strcmp(strVal, "apply"))
	{	
		strVal = req_get_cstream_var(wp, ("WPS"), "");
		if(strVal[0]) {
			kinfo("WPS: %s", strVal);
			intVal = atoi(strVal);		
		
			if (apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&intVal) == 0) {
		 		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_DISABLE error!"));
				goto setErr_wlan;
			}
			
			updateVapWscDisable(wlan_idx, intVal);
		}		

		strVal = req_get_cstream_var(wp, ("WSC_PIN"), "");
		if(strVal[0]) {
			kinfo("WSC_PIN: %s", strVal);

			if (apmib_set(MIB_HW_WSC_PIN, (void *)strVal) == 0) {
		 		strcpy(tmpBuf, ("Set MIB_HW_WSC_PIN error!"));
				goto setErr_wlan;
			}		
		}

		strVal = req_get_cstream_var(wp, ("WSC_NAME"), "");
		if(strVal[0]) {
			kinfo("WSC_NAME: %s", strVal);
			
			if (apmib_set(MIB_DEVICE_NAME, (void *)strVal) == 0) {
		 		strcpy(tmpBuf, ("Set MIB_DEVICE_NAME error!"));
				goto setErr_wlan;
			}		
		}

		apmib_update_web(CURRENT_SETTING);
		
#ifndef NO_ACTION
		run_init_script("bridge");
#endif

	}
	else if(!safe_strcmp(strVal, "pinmethod"))
	{
		strVal = req_get_cstream_var(wp, ("AP_ENROLLEE"), "");
		if(strVal[0]) {
			kinfo("AP_ENROLLEE: %s, WLAN_IF: %s", strVal, WLAN_IF);

			apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&intVal);
			if (intVal) {
				intVal = 0;
				if (apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&intVal) == 0) {
			 		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_DISABLE error!"));
					goto setErr_wlan;
				}
				
				updateVapWscDisable(wlan_idx, intVal);
				apmib_update_web(CURRENT_SETTING);	

				sprintf(tmpBuf, "echo %s > /var/wps_peer_pin", strVal);
				system(tmpBuf);

#ifndef NO_ACTION
				run_init_script("bridge");
#endif					
			}
			else {	
				
#ifndef NO_ACTION				
				for(idx=0; idx < strlen(strVal); idx++){					
					if(strVal[idx] >= '0' && strVal[idx] <= '9'){
						pincodestr_b[idx2] = strVal[idx];	
						idx2++;
					}
				}
							
				sprintf(tmpBuf, "iwpriv %s set_mib pin=%s", WLAN_IF, pincodestr_b);	
				kinfo("%s", tmpBuf);
				system(tmpBuf);							
#endif
			}
		}
	}
	else if(!safe_strcmp(strVal, "pbc"))
	{
		apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&intVal);
		if (intVal) {
			intVal = 0;
			if (apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&intVal) == 0) {
			 	strcpy(tmpBuf, ("Set MIB_WLAN_WSC_DISABLE error!"));
				goto setErr_wlan;
			}
			
			updateVapWscDisable(wlan_idx, intVal);
			apmib_update_web(CURRENT_SETTING);	// update to flash	
			
			system("echo 1 > /var/wps_start_pbc");
			
#ifndef NO_ACTION
			run_init_script("bridge");
#endif			
		}
		else {		
#ifndef NO_ACTION	
			sprintf(tmpBuf, "%s -sig_pbc wlan%d", _WSC_DAEMON_PROG, wlan_idx);
			kinfo("%s", tmpBuf);
			system(tmpBuf);
#endif
		}
	}	

	thankyouPage(wp);
	return;

setErr_wlan:
	ERR_MSG(tmpBuf);
}

void actWladvanced(request *wp, char *path, char *query)
{
	char *strVal = NULL;
	char tmpBuf[100] = {0};	
	int enabled = 0;
	MACFILTER_T macEntry;
	int val = 0;
	int cur_band = 0;

	kinfo("Enter");
	
	strVal = req_get_cstream_var(wp, ("filter"), "");
	if(strVal[0]) {
		kinfo("filter: %s", strVal);
		enabled = strVal[0] - '0';
		
		if (apmib_set( MIB_WLAN_MACAC_ENABLED, (void *)&enabled) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_MACAC_ENABLED error!"));
			goto setErr_wlan;
		}
	}

	strVal = req_get_cstream_var(wp, ("filtermac"), "");
	if(strVal[0]) {
		kinfo("filtermac: %s", strVal);

		if (strlen(strVal) != 12 || !string_to_hex(strVal, macEntry.macAddr, 12)) {
			strcpy(tmpBuf, ("Error! Invalid MAC address."));
			goto setErr_wlan;
		}		
		
		macEntry.comment[0] = '\0';

		if ( !apmib_set(MIB_WLAN_AC_ADDR_DELALL, (void *)&macEntry)) {
			strcpy(tmpBuf, ("Delete all table error!"));
			goto setErr_wlan;
		}
		
		if (apmib_set(MIB_WLAN_AC_ADDR_ADD, (void *)&macEntry) == 0) {
			strcpy(tmpBuf, ("Add table entry error!"));
			goto setErr_wlan;
		}
	}	

	strVal = req_get_cstream_var(wp, ("broadcast"), "");	
	if (strVal[0]) {
		kinfo("broadcast: %s", strVal);
		if ( strVal[0] == '0')
			val = 0;
		else if (strVal[0] == '1')
			val = 1;
		else {
			strcpy(tmpBuf, ("Error! Invalid Channel Bonding."));
			goto setErr_wlan;
		}
		if ( apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&val) == 0) {
			strcpy(tmpBuf, ("Set hidden ssid failed!"));
			goto setErr_wlan;
		}
	}

	strVal = req_get_cstream_var(wp, ("wmm"), "");	
	if (strVal[0]) {
		kinfo("wmm: %s", strVal);
		if (strVal[0] == '0')
			val = 0;
		else if (strVal[0] == '1')
			val = 1;
		else {
			strcpy(tmpBuf, ("Error! Invalid WMM value."));
			goto setErr_wlan;
		}
		if ( apmib_set(MIB_WLAN_WMM_ENABLED, (void *)&val) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WMM_ENABLED failed!"));
			goto setErr_wlan;
		}
	} else {
		//enable wmm in 11N mode always
		apmib_get( MIB_WLAN_BAND, (void *)&cur_band);
		if(cur_band == 10 || cur_band == 11){
			val = 1;
			if ( apmib_set(MIB_WLAN_WMM_ENABLED, (void *)&val) == 0) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WMM_ENABLED failed!"));
				goto setErr_wlan;
			}
		}
	}
	
	strVal = req_get_cstream_var(wp, ("gi"), "");
	if (strVal[0]) {
		kinfo("gi: %s", strVal);
		if (strVal[0] == '0')
			val = 0;
		else if (strVal[0] == '1')
			val = 1;
		else {
			strcpy(tmpBuf, ("Error! Invalid short GI."));
			goto setErr_wlan;
		}
		if ( apmib_set(MIB_WLAN_SHORT_GI, (void *)&val) == 0) {
			strcpy(tmpBuf, ("Set MIB_WLAN_SHORT_GI failed!"));
			goto setErr_wlan;
		}
	}

	strVal = req_get_cstream_var(wp, ("igmp"), "");
	if (strVal[0]) {
		kinfo("igmp: %s", strVal);
		if (strVal[0] == '0')
			val = 0;
		else if (strVal[0] == '1')
			val = 1;
		else {
			strcpy(tmpBuf, ("Error! Invalid IGMP."));
			goto setErr_wlan;
		}
		
		if ( apmib_set(MIB_IGMP_PROXY_DISABLED, (void *)&val) == 0) {
			strcpy(tmpBuf, ("Set MIB_IGMP_PROXY_DISABLED failed!"));
			goto setErr_wlan;
		}		
	}	

	apmib_update_web(CURRENT_SETTING);
	applyPage(wp);
	return;

setErr_wlan:
	ERR_MSG(tmpBuf);
}


