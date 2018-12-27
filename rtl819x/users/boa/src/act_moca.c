/*
 *      Moca Function Implementation 
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_moca.h"

extern char error_buf[];
int moca_pwdshow = 0;

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
	  if(tr69GetUnfreshLeafData(moca_channel, mdmoid_moca_object, "FreqCurrentMaskSetting"))
      {
         sprintf(error_buf, "Get %s.FreqCurrentMaskSetting error!", mdmoid_moca_object);
         sprintf(retvalue, "%s", "0x0000000015554000");
      }
	 
	  if(safe_strcmp(moca_channel, "") && safe_strcmp(moca_channel, "NULL"))
	 	sprintf(retvalue, "%s", moca_channel);	
	  else
	  	sprintf(retvalue, "%s", "0x0000000015554000");
  }
  else if (!safe_strcmp(type, "privacy"))
  {
	  if(tr69GetUnfreshLeafData(moca_privacy, mdmoid_moca_object, "PrivacyEnabledSetting"))
      {
         sprintf(error_buf, "Get %s.PrivacyEnabledSetting error!", mdmoid_moca_object);
         sprintf(retvalue, "%d", 0);
      }   
	 
	  if(safe_strcmp(moca_privacy, "") && safe_strcmp(moca_privacy, "NULL"))
	 	sprintf(retvalue, "%d", atoi(moca_privacy));	
	  else
	  	sprintf(retvalue, "%d", 0);
  }
  else if (!safe_strcmp(type, "password"))
  {
	  if(tr69GetUnfreshLeafData(moca_password, mdmoid_moca_object, "KeyPassphrase"))
      {
         sprintf(error_buf, "Get %s.KeyPassphrase error!", mdmoid_moca_object);
         sprintf(retvalue, "%s", "");
      } 

	  if(safe_strcmp(moca_password, "") && safe_strcmp(moca_password, "NULL"))
	 	sprintf(retvalue, "%s", moca_password);	
	  else
	  	sprintf(retvalue, "%s", "");
  }
  else if (!safe_strcmp(type, "status"))
  {
	  if(tr69GetFreshLeafData(moca_status, mdmoid_moca_object, "Status"))
      {
         sprintf(error_buf, "Get %s.Status error!", mdmoid_moca_object);
         sprintf(retvalue, "%s", "");
      } 

	  if(!safe_strcmp(moca_status, "Up"))
	 	sprintf(retvalue, "%s", "<font color=\"#008000\">Connected</font>");	
	  else if(!safe_strcmp(moca_status, "Down"))
	  	sprintf(retvalue, "%s", "<font color=\"#800000\">Disconnected</font>");
	  else if(moca_status && safe_strcmp(moca_status, "NULL"))
	  	sprintf(retvalue, "%s", moca_status);
	  else 
	  	sprintf(retvalue, "%s", "");
  }
  else if (!safe_strcmp(type, "NcMac"))
  {
    do{
      FILE *fp;
      char line[128] = {0};
      char cmd[128] = {0};
      char *p = NULL;

      snprintf(cmd, sizeof(cmd),"clnkstat -a > %s",CLNKSTAT_FILE);
      system(cmd);

      fp = fopen(CLNKSTAT_FILE, "r");
      if(fp == NULL)
      {
        break;
      }

      while(fgets(line,sizeof(line),fp) != NULL)
      {   
        p = strstr(line, "MAC Address:   ");
        if(p != NULL)
        {   
          sprintf(retvalue, p + strlen("MAC Address:   "));
        }

        p = strstr(line, "Node State:");
        if(p != NULL)
        {
          if(strtol(p+strlen("Node State:"), NULL, 10) & (1 << 0)) 
          {
            break;
          }
          else
          {
	    sprintf(retvalue, "%s", "");
          }
        }
      }

      fclose(fp);
    } while(0);
  }
  else 
    strcpy(retvalue, "empty");

  kinfo("%s -> %s", type, retvalue);
}

int getMocaList(request *wp, char *type)
{
  FILE *fp;
  char line[500] = {0};
  char buff[30] = {0}, buff2[30] = {0};
  char mac[8][40] = {{0}}, tx[8][20] = {{0}}, rx[8][20] = {{0}};
  char *p;
  int i, j, step, macnode, NC_node_id = -1;
  int failed_counter=0;
  int max_try=10;
  char moca_status[32] = {0};
  char sys_buf[64]={0};
  int  c_moca=0;

  while(1)
  {
	  if(tr69GetFreshLeafData(moca_status, "Device.MoCA.Interface.1", "Status"))
	  {
	     strcpy(error_buf, "Get Device.MoCA.Interface.1.Status error!");
	     kerror("error_buf: %s,moca_status:%s@@", error_buf,moca_status);
	  }
      
      if(strlen(moca_status)>0)
      {
        failed_counter=0;
        break;
      }
      else
      {
        failed_counter++;
          if(failed_counter>max_try)
            return 1;
        sleep(1);
      }
  }

   //fprintf(stdout,"moca_status=%s@1@\n",moca_status);
  if(safe_strcmp(moca_status, "Up"))
  {
    snprintf(sys_buf,sizeof(sys_buf),"echo > %s",CLNKSTAT_FILE);
    system(sys_buf);
    macnode = 0;
  }
  else
  {
    snprintf(sys_buf,sizeof(sys_buf),"clnkstat -a > %s",CLNKSTAT_FILE);
    system(sys_buf);
    macnode = -1;
  }

  fp = fopen(CLNKSTAT_FILE, "r");
  
  if (fp == NULL)
    return 1;
  
  step = 0;

  for (;;) {
    //fprintf(stdout,"moca_status=%s@2@\n",moca_status);
      if(fgets(line,sizeof(line),fp) == NULL)
      {
        if((c_moca=fgetc(fp))==EOF&&strlen(mac[0])>0)
            break;
        if(safe_strcmp(moca_status, "Up")==0)
         {
           snprintf(sys_buf,sizeof(sys_buf),"clnkstat -a > %s",CLNKSTAT_FILE);
           system(sys_buf);
           if(step==0&&strlen(mac[0])==0)
           {
              macnode = -1;
           }
           fclose(fp);
           fp = fopen(CLNKSTAT_FILE, "r");
           if (fp == NULL)
            return 1;

           continue;
         }
         else
           break;
        
	  }
   
    
	kinfo("line: %s", line);

    if (macnode == MAC_NODE_NUM) {
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

    if(step == 1)
    {
        p = strstr(line, "Node State:");
        if(p != NULL)
        {
            if(strtol(p+strlen("Node State:"), NULL, 10) & (1 << 0))
            {
                NC_node_id = macnode;
            }
        }

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
        //fprintf(stdout,"rx[macnode]=%s@9@\n",rx[macnode]);
      }
      continue;
    }
  }   

  kinfo("macnode: %d",macnode);

  fclose(fp);

    
  //if((macnode == -1) && !safe_strcmp(moca_status, "Up"))
  //   return 1;

  //Connection Speed 
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\" width=\"200\"><span id=\"connection_speed\">Connection Speed</span></td>\n");
  if(NC_node_id == 0)
  {
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\"><span id=\"status_adapter\">Adapter*</span></td>\n");
  }
  else
  {
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\"><span id=\"status_adapter\">Adapter</span></td>\n");
  }
  for (i=1; i<MAC_NODE_NUM; i++)
  {
    if(NC_node_id == i)
    {
      req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\" width=\"70\"><span name=\"status_device\">Device</span> %d*</td>\n",i);
    }
    else
    {
      req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\" width=\"70\"><span name=\"status_device\">Device</span> %d</td>\n",i);
    }
  }
  req_format_write(wp, "</tr>\n");

  //MAC Address
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\"><span id=\"status_mac\">MAC Address</span></td>\n");  
  
  if(safe_strcmp(moca_status, "Up"))
  {
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  }
  else 
  {
    for (i=0; i<=macnode; i++)
        req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",mac[i]);
  }
  
  for (i=macnode+1; i<MAC_NODE_NUM; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  
  //IP Address
  req_format_write(wp, "<tr bgcolor=\"#f1f3f9\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\"><span id=\"status_ip\">IP Address</span></td>\n");
  for (i=0; i<MAC_NODE_NUM; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Adapater
  req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\"><span id=\"stat_adapter2\">Adapter</span></td>\n");
  req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  for (i=1; i<=macnode; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",rx[i]);
  for (i=macnode+1; i<MAC_NODE_NUM; i++)
    req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
  req_format_write(wp, "</tr>\n");

  //Device 
  for(j=1;j<MAC_NODE_NUM;j++)
   {
      if(j%2==0)
       {
	 req_format_write(wp, "<tr bgcolor=\"#e0e5f1\">\n");
       }
      else
       {
         req_format_write(wp, "<tr bgcolor=\"#f1f3f9\">\n");
       }
      req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\"><span name=\"stat_device2\">Device</span> %d</td>\n",j);
      if (macnode>=j)
	 req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">%s</td>\n",tx[j]);
      else
	 req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n"); 
      for (i=1; i<MAC_NODE_NUM; i++)
	 req_format_write(wp, "<td class=\"GRID\" align=\"left\" valign=\"top\">N/A</td>\n");
	 req_format_write(wp, "</tr>\n");
   }
  return 0;
}

void actMocasetup(request *wp, char *path, char *query)
{			
	char moca_channel[20] = {0};
	char moca_privacy[10] = {0};
	char moca_password[64] = {0};	
	char moca_taboofre[64] = {0};
	char moca_controller[4] = {0};
	char moca_beacon[4] = {0};	
	char mdmoid_moca_object[256] = {0};
	char *strVal = NULL;
	char *command = NULL;
    char *newpassword = NULL;
	char *newchannel = NULL;
	int newprivacy = 0;
	

	sprintf(mdmoid_moca_object, "%s.%d", "Device.MoCA.Interface", 1);
	
	kinfo("Enter");

	command = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", command);

	if(!safe_strcmp(command, "apply"))
	{
		newchannel = req_get_cstream_var(wp, ("moca_channel"), "");
		if(newchannel[0]) {
		  	kinfo("moca_channel: %s", newchannel);	

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

		strVal = req_get_cstream_var(wp, ("moca_privacy"), "");
		if(strVal[0]) {			
			kinfo("moca_privacy: %s", strVal);

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

		newpassword = req_get_cstream_var(wp, ("moca_pwd"), "");
		if(newpassword[0])
		{
			kinfo("moca_pwd: %s", newpassword);	

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
		
		strVal = req_get_cstream_var(wp, ("moca_taboobit"), "");
		if(strVal[0]) {			
			kinfo("moca_taboobit: %s", strVal);

			if(!safe_strcmp(strVal, "0"))
			{
				if(tr69CommitSetLeafDataString("Device.MoCA.Interface", 1, "FreqCapabilityMask", "0"))
				{
					strcpy(error_buf, ("Set Device.MoCA.Interface.1.FreqCapabilityMask error"));
					goto setErr_moca;
				}
			}
			else 
			{
				strVal = req_get_cstream_var(wp, ("moca_taboofre"), "");
				if(strVal[0]) {			
					kinfo("moca_taboofre: %s", strVal);

					if(tr69CommitSetLeafDataString("Device.MoCA.Interface", 1, "FreqCapabilityMask", strVal))
					{
						strcpy(error_buf, ("Set Device.MoCA.Interface.1.FreqCapabilityMask error"));
						goto setErr_moca;
					}
				}
			}
		}

		strVal = req_get_cstream_var(wp, ("moca_controller"), "");
		if(strVal[0]) {			
			kinfo("moca_controller: %s", strVal);

			if(tr69CommitSetLeafDataInt("Device.MoCA.Interface", 1, "PreferredNC", atoi(strVal)))
			{
				strcpy(error_buf, ("Set Device.MoCA.Interface.1.PreferredNC error"));
				goto setErr_moca;
			}
		}

		strVal = req_get_cstream_var(wp, ("moca_beacon"), "");
		if(strVal[0]) {			
			kinfo("moca_beacon: %s", strVal);

			if(tr69CommitSetLeafDataInt("Device.MoCA.Interface", 1, "BeaconPowerLimit", atoi(strVal)))
			{
				strcpy(error_buf, ("Set Device.MoCA.Interface.1.BeaconPowerLimit error"));
				goto setErr_moca;
			}
		}
		
		strVal = req_get_cstream_var(wp, ("moca_pwdshow"), "");
		if(strVal[0]) {			
			kinfo("moca_pwdshow: %s", strVal);
			moca_pwdshow = atoi(strVal);
		}

		if(tr69CommitSetLeafDataEnd("Device.MoCA.Interface", 1))
		{
			strcpy(error_buf, ("Set Device.MoCA.Interface.1 error"));
			goto setErr_moca;
		}
		if(tr69SaveNow())
        {
            strcpy(error_buf, "tr69SaveNow error!");
            goto setErr_moca;
        }      
		thankyouPage(wp);
	}
	else if(!safe_strcmp(command, "view"))
	{
		nothankyouPage(wp);
	}
	else if(!safe_strcmp(command, "getvalue"))
	{
		if(tr69GetUnfreshLeafData(moca_taboofre, mdmoid_moca_object, "FreqCapabilityMask"))
        {
			sprintf(error_buf, "Get %s.FreqCapabilityMask error", mdmoid_moca_object);
			goto getErr_moca;
		}
		changeString(moca_taboofre);

		if(tr69GetUnfreshLeafData(moca_controller, mdmoid_moca_object, "PreferredNC"))
        {
			sprintf(error_buf, "Get %s.PreferredNC error", mdmoid_moca_object);
			goto getErr_moca;
		}
		changeString(moca_controller);
		
		if(tr69GetUnfreshLeafData(moca_beacon, mdmoid_moca_object, "BeaconPowerLimit"))
        {
			sprintf(error_buf, "Get %s.BeaconPowerLimit error", mdmoid_moca_object);
			goto getErr_moca;
		}
		changeString(moca_beacon);
	
		if(tr69GetUnfreshLeafData(moca_channel, mdmoid_moca_object, "FreqCurrentMaskSetting"))
        {
			sprintf(error_buf, "Get %s.FreqCurrentMaskSetting error", mdmoid_moca_object);
			goto getErr_moca;
		}
		changeString(moca_channel);
		
  	    if(tr69GetUnfreshLeafData(moca_privacy, mdmoid_moca_object, "PrivacyEnabledSetting"))
        {
			sprintf(error_buf, "Get %s.PrivacyEnabledSetting error", mdmoid_moca_object);
			goto getErr_moca;
		}
		changeString(moca_privacy);

	    if(tr69GetUnfreshLeafData(moca_password, mdmoid_moca_object, "KeyPassphrase"))
        {
            sprintf(error_buf, "Get %s.KeyPassphrase error", mdmoid_moca_object);
			goto getErr_moca;
        }   
		changeString(moca_password);

		req_format_write(wp, ("%s|%s|%s|%s|%s|%s|%d||"), 
			moca_taboofre, moca_controller, moca_beacon, moca_channel, moca_privacy, moca_password, moca_pwdshow);				
	}
	
	return;  

setErr_moca:
	kerror("error_buf: %s", error_buf);
	backPage(wp);
getErr_moca:
    kerror("error_buf: %s", error_buf);
}



