/*
 *      Wireless Function Implementation  
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_wireless.h"
#include "utility.h"
#include "act_status.h"
#include <sys/time.h>	

#ifdef ACTIONTEC_TR69
#include "act_tr69.h"
#endif

#ifdef ACTIONTEC_MULTIACCESS
#include "act_session.h"
#endif

#include "dal_wifi.c"

#ifdef ACTIONTEC_CLIENT
int client_select_id = 0;
char *client_ssid = NULL;
int client_channel = 0;
int client_encrypt = 0;
int client_wpa_tkip_aes = 0;
int client_wpa2_tkip_aes = 0;
#endif

extern int wlan_idx;
extern int vwlan_idx;
int both_freq = 1;
int curr_ssid = 1;
char error_buf[100] = {0};	

static unsigned int IsValidHexadecimalString(char* key)
{
	unsigned int valid = 1;
	int i;

	do{
		if(NULL==key||0==strlen(key)){
			kerror("The key [%s] is invalid hexadecimal string.\n", key);
			valid = 0;
			break;
		}
		
		if(0!=(strlen(key)&0x01)){
			kerror("The key [%s] is invalid hexadecimal string.\n", key);
			valid = 0;
			break;
		}

		for(i=0; i<strlen(key); i++){
			if((key[i]<'0'||key[i]>'9')
				&&(key[i]<'a'||key[i]>'f')
				&&(key[i]<'A'||key[i]>'F')){
				kerror("The key [%s] is invalid hexadecimal string.\n", key);
				valid = 0;
				break;
			}
		}
	}while(0);

	return valid;
}


void applyPage(request *wp, const char *init_arg)
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

	apmib_update_web(CURRENT_SETTING);

	run_init_script_flag = 1;			
#ifndef NO_ACTION
	run_init_script(init_arg);
#endif	
	needReboot = 0;
	
	countDownTime = WIRELESS_SETTING_COUNTDOWN_TIME;
	
	strVal = req_get_cstream_var(wp, ("mypage"), "");	
	if(strVal[0])	
		sprintf(lastUrl, "%s", strVal);
	else
		strncpy(lastUrl, MAIN_PAGE, 100);
	
	send_redirect_perm(wp, COUNTDOWN_PAGE);	
}

static int SetWlan_idx(char * wlan_iface_name)
{
	int idx;
	
		idx = atoi(&wlan_iface_name[4]);
		if (idx >= NUM_WLAN_INTERFACE) {
				printf("invalid wlan interface index number!\n");
				return 0;
		}
		wlan_idx = idx;
		vwlan_idx = 0;
	
#ifdef MBSSID		
		
		if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
				wlan_iface_name[6] == 'v' && wlan_iface_name[7] == 'a') {
				idx = atoi(&wlan_iface_name[8]);
				if (idx >= NUM_VWLAN_INTERFACE) {
					printf("invalid virtual wlan interface index number!\n");
					return 0;
				}
				
				vwlan_idx = idx+1;
				idx = atoi(&wlan_iface_name[4]);
				wlan_idx = idx;
		}
#endif	
#ifdef UNIVERSAL_REPEATER
				if (strlen(wlan_iface_name) >= 9 && wlan_iface_name[5] == '-' &&
						!memcmp(&wlan_iface_name[6], "vxd", 3)) {
					vwlan_idx = NUM_VWLAN_INTERFACE;
					idx = atoi(&wlan_iface_name[4]);
					wlan_idx = idx;
				}
	
#endif				
				
return 1;		
}		


/*  Version 2:   2.4G, 5G, Both.
  *
  */
  
int applyWac(request *wp, char *tmpBuf)
{
	char *strVal = NULL;

	char *token = NULL;
	char mac_address[16] = {0};
	int enabled = 0;
	int entryNum = 0;
	MACFILTER_T entry;
	
	strVal = req_get_cstream_var(wp, ("wl_macenabled"), "");
	if(strVal[0])
	{
		kinfo("wl_macenabled: %s", strVal);		
		enabled = atoi(strVal);
				
		if (!apmib_set( MIB_WLAN_MACAC_ENABLED, (void *)&enabled)) {
			strcpy(tmpBuf, ("Set enabled flag error!"));
			goto setErr_wac;
		}
	}

	strVal = req_get_cstream_var(wp, ("wl_mac"), "");
	if(strVal[0])
	{
		kinfo("wl_mac: %s", strVal);
		
		token = strtok(strVal, ":");
		while(token != NULL)
		{			
			if(strlen(mac_address) + strlen(token) < 16)
				strcat(mac_address, token);
			token = strtok(NULL, ":");
		}
		
		if (strlen(mac_address) != 12 || !string_to_hex(mac_address, entry.macAddr, 12)) {
			strcpy(tmpBuf, ("Error! Invalid MAC address."));
			goto setErr_wac;
		}

		strVal = req_get_cstream_var(wp, ("wl_maccomment"), "");		
		if ( strVal[0] ) {
			kinfo("wl_maccomment: %s", strVal);	

			if (strlen(strVal) > COMMENT_LEN - 1) {
				strcpy(tmpBuf, ("Error! Comment length too long."));
				goto setErr_wac;
			}
			else if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(tmpBuf, ("Error! Comment is invalid."));
				goto setErr_wac;
			}
			
			strcpy((char *)entry.comment, strVal);
		}
		else
			entry.comment[0] = '\0';

		if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum)) {
			strcpy(tmpBuf, ("Get entry number error!"));
			goto setErr_wac;
		}
		if ( (entryNum + 1) > MAX_WLAN_AC_NUM) {
			strcpy(tmpBuf, ("Cannot add new entry because table is full!"));
			goto setErr_wac;
		}

		// set to MIB. try to delete it first to avoid duplicate case
		apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&entry);
				
		if (!apmib_set(MIB_WLAN_AC_ADDR_ADD, (void *)&entry)) {
			strcpy(tmpBuf, ("Add table entry error!"));
			goto setErr_wac;
		}
	}

	return 1;
	
setErr_wac:
	return 0;
}

int getlistWac(request *wp, char *tmpBuf, int both)
{
	int entryNum = 0;
	MACFILTER_T entry;
	char buf[100] = {0};
	int i = 0;
	int entry2Num = 0;
	int entry5Num = 0;
    int enabled = 0;
    int wl_freq=0;
	char comment[COMMENT_LEN] = {0};
        int enable24g = 0;
        int enable5g = 0;

	if(!both)
	{
		if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entryNum)) {				
			strcpy(tmpBuf, ("Get entry number error!"));
			goto setErr_wac;
		}
                if (!apmib_get( MIB_WLAN_MACAC_ENABLED, (void *)&enabled)) {
                                strcpy(error_buf, ("Set enabled flag error!"));
                                kerror("error_buf: %s", error_buf);
                                backPage(wp);
                }
	}
	else
	{
		wlan_idx = 1;
		if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entry2Num)) {				
			strcpy(tmpBuf, ("Get entry number2 error!"));
			goto setErr_wac;
		}
                if (!apmib_get( MIB_WLAN_MACAC_ENABLED, (void *)&enable24g)) {
                                strcpy(error_buf, ("Set enabled flag error!"));
                                kerror("error_buf: %s", error_buf);
                                backPage(wp);
                }
		
		wlan_idx = 0;
		if (!apmib_get(MIB_WLAN_MACAC_NUM, (void *)&entry5Num)) {				
			strcpy(tmpBuf, ("Get entry number5 error!"));
			goto setErr_wac;
		}
                if (!apmib_get( MIB_WLAN_MACAC_ENABLED, (void *)&enable5g)) {
                                strcpy(error_buf, ("Set enabled flag error!"));
                                kerror("error_buf: %s", error_buf);
                                backPage(wp);
                }

		entryNum = entry2Num + entry5Num;
	}

	kdebug("entryNum: %d", entryNum);
        if(entryNum < 1)
        {
                if(!both)
                {
                        snprintf(buf, sizeof(buf), ("%d_entrys_num0"),enabled);
                        req_format_write(wp, ("%s"), buf);
                }
                else
                {
                        if(enable24g == enable5g)
                        {
                                snprintf(buf, sizeof(buf), ("%d_entrys_num0"),enable24g);
                                req_format_write(wp, ("%s"), buf);
                        }
			else
                        {	
                        	snprintf(buf, sizeof(buf), ("0_entrys_num0"));
                        	req_format_write(wp, ("%s"), buf);
                        }
                }
        }

	if(!both)
	{
		for (i = 1; i <= entryNum; i++) {
			*((char *)&entry) = (char)i;
			if (!apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&entry)) {
				strcpy(tmpBuf, ("Get MIB_WLAN_MACAC_ADDR error!"));
				goto setErr_wac;
			}

			if (!apmib_get( MIB_WLAN_MACAC_ENABLED, (void *)&enabled)) {
				strcpy(error_buf, ("Set enabled flag error!"));
				kerror("error_buf: %s", error_buf);
				backPage(wp);
			}
            
			kdebug("entry.comment: %s", entry.comment);
			
			changeString(entry.comment);
		
			snprintf(buf, 100, ("%02x:%02x:%02x:%02x:%02x:%02x|%s|0|%d|%d|%d||"),
				entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
				entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], entry.comment, i, curr_ssid, enabled);
		
			req_format_write(wp, ("%s"), buf);
		}
	}
	else 
	{
		wlan_idx = 1;
		for (i = 1; i <= entry2Num; i++) {
			*((char *)&entry) = (char)i;
			if (!apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&entry)) {
				strcpy(tmpBuf, ("Get MIB_WLAN_MACAC_ADDR2 error!"));
				goto setErr_wac;
			}

			changeString(entry.comment);

			if (!apmib_get( MIB_WLAN_MACAC_ENABLED, (void *)&enabled)) {
				strcpy(error_buf, ("Set enabled flag error!"));
				kerror("error_buf: %s", error_buf);
				backPage(wp);
			}


			snprintf(buf, 100, ("%02x:%02x:%02x:%02x:%02x:%02x|%s|10|%d|%d|%d||"),
				entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
				entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], entry.comment, i, curr_ssid, enabled);
		
			req_format_write(wp, ("%s"), buf);
		}

		wlan_idx = 0;
		for (i = 1; i <= entry5Num; i++) {
			*((char *)&entry) = (char)i;
			if (!apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&entry)) {
				strcpy(tmpBuf, ("Get MIB_WLAN_MACAC_ADDR5 error!"));
				goto setErr_wac;
			}
			
			changeString(entry.comment);

			if (!apmib_get( MIB_WLAN_MACAC_ENABLED, (void *)&enabled)) {
				strcpy(error_buf, ("Set enabled flag error!"));
				kerror("error_buf: %s", error_buf);
				backPage(wp);
			}


			snprintf(buf, 100, ("%02x:%02x:%02x:%02x:%02x:%02x|%s|11|%d|%d|%d||"),
				entry.macAddr[0], entry.macAddr[1], entry.macAddr[2],
				entry.macAddr[3], entry.macAddr[4], entry.macAddr[5], entry.comment, i, curr_ssid, enabled);
		
			req_format_write(wp, ("%s"), buf);
		}
	}

	return 1;
		
setErr_wac:
	return 0;	
}


void actWlwac(request *wp, char *path, char *query)
{
	char *strVal = NULL;
	char *action = NULL;
	char *token = NULL;
	char *index_token = NULL;
	char token_delete[256][32] = {0};	
	int wl_freq = 0;
	int entryNum = 0;
	int i = 0;
	int enabled = 0;
	int index = 0;
	int index_delete = 0;
	MACFILTER_T entry;	
	
	both_freq = 0;	
	wlan_idx = 0;
	
	strVal = req_get_cstream_var(wp, ("wl_freq"), "");
	if(strVal[0])
	{
		kinfo("wl_freq: %s", strVal);
		wl_freq = atoi(strVal);

		if(wl_freq == 10)
			wlan_idx = 1;
		else if(wl_freq == 11)
			wlan_idx = 0;
		else if(wl_freq == 0)
			both_freq = 1;
	}

	strVal = req_get_cstream_var(wp, ("wl_mssididx"), "");
	if(strVal[0])
	{
		kinfo("wl_mssididx: %s", strVal);
		vwlan_idx = atoi(strVal);
		curr_ssid = vwlan_idx;
	}

	action = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", action);
	
	if(!safe_strcmp(action, "apply"))
	{
		if(!both_freq)
		{			
			if(!applyWac(wp, error_buf))
				goto setErr_wac;
		}
		else
		{
			wlan_idx = 1;
			if(!applyWac(wp, error_buf))
				goto setErr_wac;

			wlan_idx = 0;
			if(!applyWac(wp, error_buf))
				goto setErr_wac;
		}		
	}
	else if(!safe_strcmp(action, "getlist"))
	{
		if(!getlistWac(wp, error_buf, both_freq))
			goto setErr_wac;
		/**
		if(!both_freq)
		{
			if (!apmib_get( MIB_WLAN_MACAC_ENABLED, (void *)&enabled)) {
				strcpy(error_buf, ("Set enabled flag error!"));
				goto setErr_wac;
			}			
		}
		req_format_write(wp, (";%d"), enabled);
		**/
		return;
	}
	else if(!safe_strcmp(action, "delete"))
	{
		strVal = req_get_cstream_var(wp, ("delete_index"), "");
		if(strVal[0])
		{
			kinfo("delete_index: %s", strVal);	

			token = strtok(strVal, ";");
	        while(token != NULL) {
	        	kdebug("token_delete: %s", token);
				strncpy(token_delete[index_delete++], token, 31);
				token = strtok(NULL, ";");						
	        }	

			for(i = 0; i < index_delete; i++)
			{
				index = 1;
				index_token = strtok(token_delete[i], "_");			
				while(index_token != NULL)
				{
					kdebug("index_token_delete: %s", index_token);
					if(index == 1)
					{
						entryNum = atoi(index_token);
					}
					else if(index == 2)
					{
						if(!safe_strcmp(index_token, "10"))
							wlan_idx = 1;
						else if(!safe_strcmp(index_token, "11"))
							wlan_idx = 0;
					}
					index++;
					index_token = strtok(NULL, "_");
				}				

				kdebug("wlan_idx: %d, entryNum: %d", wlan_idx, entryNum);
				
				*((char *)&entry) = (char)entryNum;
				if (!apmib_get(MIB_WLAN_MACAC_ADDR, (void *)&entry)) {
					strcpy(error_buf, ("Get table entry error!"));
					goto setErr_wac;
				}
				if (!apmib_set(MIB_WLAN_AC_ADDR_DEL, (void *)&entry)) {
					strcpy(error_buf, ("Delete table entry error!"));
					goto setErr_wac;
				}
			}
		}		
	}
	else if(!safe_strcmp(action, "deleteall"))
	{
		if(!both_freq)
		{
			if (!apmib_set(MIB_WLAN_AC_ADDR_DELALL, (void *)&entry)) {
				strcpy(error_buf, ("Delete all table error!"));
				goto setErr_wac;
			}	
		}
		else
		{
			wlan_idx = 1;
			if (!apmib_set(MIB_WLAN_AC_ADDR_DELALL, (void *)&entry)) {
				strcpy(error_buf, ("Delete all table2 error!"));
				goto setErr_wac;
			}

			wlan_idx = 0;
			if (!apmib_set(MIB_WLAN_AC_ADDR_DELALL, (void *)&entry)) {
				strcpy(error_buf, ("Delete all table5 error!"));
				goto setErr_wac;
			}
		}		
	}
	
	applyPage(wp, INIT_ALL);	
	return;
	
setErr_wac:	
	kerror("error_buf: %s", error_buf);
	backPage(wp);		
}

int setWlanDisabled(char *tmpBuf, char *strVal, int all_flag)
{	
	int disabled = atoi(strVal);
	int i = 0;
	//int disabled_index = 0;
	//int enabled_index = 0;
	//int getset_disabled = 0;
	int current_vwlan = vwlan_idx;
    char cmd[256] = {0};

    // Basic Setup
    if(all_flag == 1)
    {
        if(disabled == 1)
        {
            vwlan_idx = 0;
            if (!apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&disabled)) {				
            	strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED1 error!"));
            	goto setErr;
            }
        }
        else if(disabled == 0)
        {    
            for(i = 1; i >= 0; i--)
        	{
        		vwlan_idx = i;
        		if (!apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&disabled)) {				
        			strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED2 error!"));
        			goto setErr;
        		}
                ktrace("set wlan_idx %d vwlan_idx %d MIB_WLAN_WLAN_DISABLED %d", wlan_idx, i, disabled);
        	}        	
        }
    }
    else if(all_flag == 0)
    {        // Multiple SSID
        vwlan_idx = current_vwlan;
        if (!apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&disabled)) {                
            strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED3 error!"));
            goto setErr;
        }

       /* for(i = 4; i >= 1; i--)
    	{
    		vwlan_idx = i;
    		if (!apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&getset_disabled)) {				
    			strcpy(tmpBuf, ("Get MIB_WLAN_WLAN_DISABLED2 error!"));
    			goto setErr;
    		}

    		if(getset_disabled)
    			disabled_index++;
    		else
    			enabled_index++;
    	}

    	vwlan_idx = 0;
        ktrace("disabled_index: %d, enabled_index: %d", disabled_index, enabled_index);
    	if(disabled_index == 4)
    	{
    		getset_disabled = 1;
    		if (!apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&getset_disabled)) {				
    			strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED4 error!"));
    			goto setErr;
    		}
    	}
    	else if(enabled_index >= 1)
    	{
    		getset_disabled = 0;		
    		if (!apmib_set(MIB_WLAN_WLAN_DISABLED, (void *)&getset_disabled)) {				
    			strcpy(tmpBuf, ("Set MIB_WLAN_WLAN_DISABLED5 error!"));
    			goto setErr;
    		}
    	}	 */  
    }
    
	vwlan_idx = current_vwlan;
		
#ifdef ACTIONTEC_TR69
	int id = 0;
    int radio_id = 0;
	int enabled = 0;

	if(disabled == 0)
		enabled = 1;
	
	if(wlan_idx == 1)
	{
		if(vwlan_idx > 0)			
		{
			id = vwlan_idx;
            radio_id = 1;
		}
	}
	else if(wlan_idx == 0)
	{
		if(vwlan_idx > 0)
		{
			id = 4 + vwlan_idx;
            radio_id = 2;
		}
	}	

	if(id != 0)
	{        
        if(all_flag == 1)
        {
            if(disabled == 1)
            {
                if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", radio_id, "Enable", 0))
        		{
        			sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.Enable error!"), radio_id);
        			goto setErr;
        		}
            }
            else if(disabled == 0)
            {
                if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", radio_id, "Enable", 1))
        		{
        			sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.Enable error!"), radio_id);
        			goto setErr;
        		}

                if(tr69SetUnfreshLeafDataInt("Device.WiFi.SSID", id, "Enable", 1))
        		{
        			sprintf(tmpBuf, ("Set Device.WiFi.SSID.%d.Enable error!"), id);
        			goto setErr;
        		}

                if(tr69SetUnfreshLeafDataInt("Device.WiFi.AccessPoint", id, "Enable", 1))
        		{
        			sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Enable error!"), id);
        			goto setErr;
        		}           		
            }
        }
        else if(all_flag == 0)
        {
            if(tr69SetUnfreshLeafDataInt("Device.WiFi.SSID", id, "Enable", enabled))
    		{
    			sprintf(tmpBuf, ("Set Device.WiFi.SSID.%d.Enable error!"), id);
    			goto setErr;
    		}

            if(tr69SetUnfreshLeafDataInt("Device.WiFi.AccessPoint", id, "Enable", enabled))
    		{
    			sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Enable error!"), id);
    			goto setErr;
    		}

          /*  if(disabled_index == 4)
            {
                if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", radio_id, "Enable", 0))
        		{
        			sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.Enable error!"), radio_id);
        			goto setErr;
        		}
            }
            else if(enabled_index >= 1)
            {
                if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", radio_id, "Enable", 1))
        		{
        			sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.Enable error!"), radio_id);
        			goto setErr;
        		}
            }*/
        }    	
	}
#endif

	return 1;			
setErr:
	return 0;
}

int setBAND(char *tmpBuf, char *strVal)
{
	int band = 0;	
	int wpaCipher = 0;
	int wpa2Cipher = 0;
	int wdsEncrypt = 0;
	int wlan_encrypt = 0;	
	int wlan_onoff_tkip = 0;
	int band_no = strtol( strVal, (char **)NULL, 10);
    int i = 0;

	if (band_no < 0 || band_no > 19) {
  		strcpy(tmpBuf, ("Invalid band value!"));
		goto setErr;
	}

#ifdef ACTIONTEC_WIFI_CERTIFICATED
	apmib_get(MIB_WLAN_11N_ONOFF_TKIP, (void *)&wlan_onoff_tkip);
		
	if(wlan_onoff_tkip == 0) //Wifi request
	{	
		apmib_get(MIB_WLAN_ENCRYPT, (void *)&wlan_encrypt);
		apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpaCipher);
		apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa2Cipher);
		apmib_get(MIB_WLAN_WDS_ENCRYPT, (void *)&wdsEncrypt);
			
		if(band_no == 7 || band_no == 9 || band_no == 10 || band_no == 11) //7:n; 9:gn; 10:bgn 11:5g_an
		{
			if(wlan_encrypt == ENCRYPT_WPA || wlan_encrypt == ENCRYPT_WPA2){
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
				apmib_set(MIB_WLAN_WDS_ENCRYPT, (void *)&wdsEncrypt);
			}
		}		
	}
#endif

	band = (band_no + 1);

    for(i = 0; i < 5; i++)
    {
        vwlan_idx = i;
    	if(!apmib_set( MIB_WLAN_BAND, (void *)&band)) {
       		strcpy(tmpBuf, ("Set band error!"));
    		goto setErr;
    	}		
    }

    vwlan_idx = curr_ssid;

#ifdef ACTIONTEC_TR69
	int id = 0;
	char band_buf[8] = {0};

	switch(band_no)
	{
		case 0:
			strcpy(band_buf, "b");
			break;
		case 1:
			strcpy(band_buf, "g");
			break;
		case 7:
			strcpy(band_buf, "n");
			break;
		case 2:
			strcpy(band_buf, "b,g");
			break;
		case 9:
			strcpy(band_buf, "g,n");
			break;
		case 10:
			strcpy(band_buf, "b,g,n");
			break;
		case 3:
			strcpy(band_buf, "a");
			break;
		case 11:
			strcpy(band_buf, "a,n");
			break;			
		default:
			break;
	}
	
	if(wlan_idx == 1)
	{
		id = 1;
	}
	else if(wlan_idx == 0)
	{
		id = 2;
	}	

	if(id != 0)
	{
		if(tr69SetUnfreshLeafDataString("Device.WiFi.Radio", id, "OperatingStandards", band_buf))
		{
			sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.OperatingStandards error!"), id);
			goto setErr;
		}
	}
#endif
	
	return 1;			
setErr:
	return 0;
}

int setRate(char *tmpBuf, char *strVal)
{
	int rate = 0;
	
	if (strVal[0] == '0') 
	{ 
		// auto
		rate = 1;
		if (!apmib_set(MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&rate)) {
			strcpy(tmpBuf, ("Set rate adaptive failed!"));
			goto setErr;
		}
	}
	else  
	{
		if (!apmib_set(MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&rate)) {
			strcpy(tmpBuf, ("Set rate adaptive failed!"));
			goto setErr;
		}  
		rate = atoi(strVal);
		rate = 1 << (rate - 1);
		if (!apmib_set(MIB_WLAN_FIX_RATE, (void *)&rate)) {
			strcpy(tmpBuf, ("Set fix rate failed!"));
			goto setErr;
		}
	}			
							
	return 1;			
setErr:
	return 0;
}

int setSSID(char *tmpBuf, char *strVal)
{
	/*
	if(strchr(strVal, FORM_INPUT_SPECIAL))
	{
		strcpy(tmpBuf, ("Error! SSID is invalid."));
		goto setErr;
	}
	*/
	
	if (!apmib_set(MIB_WLAN_SSID, (void *)strVal)) {				
		strcpy(tmpBuf, ("Set MIB_WLAN_SSID error!"));
		goto setErr;
	}

	if(!apmib_set(MIB_WLAN_WSC_SSID, (void *)strVal)) {
		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_SSID mib error!"));
		goto setErr;
	}

#ifdef ACTIONTEC_TR69
	int id = 0;	
	
	if(wlan_idx == 1)
	{
		if(vwlan_idx > 0)			
		{
			id = vwlan_idx;
		}
	}
	else if(wlan_idx == 0)
	{
		if(vwlan_idx > 0)
		{
			id = 4 + vwlan_idx;
		}
	}	

	if(id != 0)
	{
		if(tr69SetUnfreshLeafDataString("Device.WiFi.SSID", id, "SSID", strVal))
		{
			sprintf(tmpBuf, ("Set Device.WiFi.SSID.%d.SSID error!"), id);
			goto setErr;
		}
	}
#endif
	
	return 1;			
setErr:
	return 0;
}

int setChanwidth(char *tmpBuf, char *strVal)
{
	int val = 0;
	int coexist = 0;
	int sideband = 1;
    char cmd[256] = {0};
	
	if ( strVal[0] == '0')
		val = 0;
	else if (strVal[0] == '1')
		val = 1;
	else if (strVal[0] == '2')
		val = 2;
	else if (strVal[0] == '3')
		val = 3;
	else {
		strcpy(tmpBuf, ("Error! Invalid Channel Bonding."));
		goto setErr;
	}

	if(val != 3)
	{
		if(val == 2)
		{
			val = 1;
			sideband = 0;
		}
		
		if (!apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&val)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL_BONDING failed!"));
			goto setErr;
		}		

		if(val == 1)
		{
			if (!apmib_set(MIB_WLAN_CONTROL_SIDEBAND, (void *)&sideband)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_CONTROL_SIDEBAND failed!"));
				goto setErr;
			}

            snprintf(cmd, sizeof(cmd), "iwpriv wlan%d set_mib Non40MSTADeny=1", wlan_idx);
            ktrace("cmd: %s", cmd);
            system(cmd);
		}
	}
	else if(val == 3)
	{ 
		val = 1;		
		if (!apmib_set(MIB_WLAN_CHANNEL_BONDING, (void *)&val)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_CHANNEL_BONDING failed!"));
			goto setErr;
		}
        
        snprintf(cmd, sizeof(cmd), "iwpriv wlan%d set_mib Non40MSTADeny=0", wlan_idx);
        ktrace("cmd: %s", cmd);
        system(cmd);
	}

	/*
	if (!apmib_set(MIB_WLAN_COEXIST_ENABLED, (void *)&coexist)) {
		strcpy(tmpBuf, ("Set MIB_WLAN_COEXIST_ENABLED failed!"));
		goto setErr;
	}	
	*/
	
		
#ifdef ACTIONTEC_TR69
	int id = 0; 
	char band_width[8] = {0};
	char control_channel[24] = {0};

	if ( strVal[0] == '0')
		val = 0;
	else if (strVal[0] == '1')
		val = 1;
	else if (strVal[0] == '2')
		val = 2;
	else if (strVal[0] == '3')
		val = 3;
	else {
		strcpy(tmpBuf, ("Error! Invalid Channel Bonding."));
		goto setErr;
	}
	
	if(val == 0)
		strcpy(band_width, "20MHz");
	else if(val == 1)
		strcpy(band_width, "40MHz");
	else if(val == 2)
		strcpy(band_width, "40MHz");
	else if(val == 3)
		strcpy(band_width, "Auto");
	
	if(wlan_idx == 1)
	{
		id = 1;
	}
	else if(wlan_idx == 0)
	{
		id = 2;
	}	

	if(id != 0)
	{
		if(tr69SetUnfreshLeafDataString("Device.WiFi.Radio", id, "OperatingChannelBandwidth", band_width))
		{
			sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.OperatingChannelBandwidth error!"), id);
			goto setErr;
		}	

		if(val == 1||val == 2)
		{
			if(sideband == 1)
				strcpy(control_channel, "AboveControlChannel");
			else if(sideband == 0)
				strcpy(control_channel, "BelowControlChannel");
				
			if(tr69SetUnfreshLeafDataString("Device.WiFi.Radio", id, "ExtensionChannel", control_channel))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.ExtensionChannel error!"), id);
				goto setErr;
			}
		}
	}
#endif

	return 1;		
setErr:
	return 0;
}

int setChannel(char *tmpBuf, char *strVal, int chanwidth)
{
	int chan = atoi(strVal);
	int sideband = 0;

	if(chanwidth == 3)
	{
		if(wlan_idx == 1)
		{
			if((chan == 1) || (chan == 2) || (chan == 3) || (chan == 4)) // Upper
			{
				sideband = 1;
			}            
		}
		else if(wlan_idx == 0)
		{
			if((chan == 36) || (chan == 44) || (chan == 149) || (chan == 157)) // Upper
			{
				sideband = 1;
			}           
		}

		if (!apmib_set(MIB_WLAN_CONTROL_SIDEBAND, (void *)&sideband)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_CONTROL_SIDEBAND failed!"));
			goto setErr;
		}
	}
	
	if (!apmib_set( MIB_WLAN_CHANNEL, (void *)&chan)) {
	   	strcpy(tmpBuf, ("Set channel number error!"));
		goto setErr;
	}

#ifdef ACTIONTEC_TR69
	int id = 0; 
	char control_channel[24] = {0};
	
	if(wlan_idx == 1)
	{
		id = 1;
	}
	else if(wlan_idx == 0)
	{
		id = 2;
	}	

	if(id != 0)
	{
		if(chan != 0)
		{
			if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", id, "Channel", chan))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.Channel error!"), id);
				goto setErr;
			}
			if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", id, "AutoChannelEnable", 0))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.AutoChannelEnable error!"), id);
				goto setErr;
			}
		}
		else
		{
			if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", id, "AutoChannelEnable", 1))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.AutoChannelEnable error!"), id);
				goto setErr;
			}
		}

		if(chanwidth == 3)
		{
			if(sideband == 1)
				strcpy(control_channel, "AboveControlChannel");
			else if(sideband == 0)
				strcpy(control_channel, "BelowControlChannel");
					
			if(tr69SetUnfreshLeafDataString("Device.WiFi.Radio", id, "ExtensionChannel", control_channel))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.ExtensionChannel error!"), id);
				goto setErr;
			}
		}
	}
#endif
	
	return 1;		
setErr:
	return 0;
}

int setTransmit(char *tmpBuf, char *strVal)
{
	int transmit = atoi(strVal);

	if (!apmib_set(MIB_WLAN_RFPOWER_SCALE, (void *)&transmit)) {
		strcpy(tmpBuf, ("Set MIB_WLAN_RFPOWER_SCALE failed!"));
		goto setErr;
	}
			
#ifdef ACTIONTEC_TR69
	int id = 0; 
	
	if(wlan_idx == 1)
	{
		id = 1;
	}
	else if(wlan_idx == 0)
	{
		id = 2;
	}	

	if(id != 0)
	{
		if(transmit == 0)
			transmit = 100;
		else if(transmit == 1)
			transmit = 70;
		else if(transmit == 2)
			transmit = 50;
		else if(transmit == 3)
			transmit = 35;
		else if(transmit == 4)
			transmit = 15;		
		
		if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", id, "TransmitPower", transmit))
		{
			sprintf(tmpBuf, ("Set Device.WiFi.Radio.%d.TransmitPower error!"), id);
			goto setErr;
		}
	}
#endif

	return 1;		
setErr:
	return 0;
}

int setSecurity(char *tmpBuf, ENCRYPT_T encrypt, int basic)
{
	int wsc_configured = 1;
	int wsc_auth = 1;
	int wsc_enc = 0;
	int wpa_cipher = 0;
	int disabled = 0;
	
	if(encrypt == ENCRYPT_DISABLED)
	{
		wsc_enc = WSC_ENCRYPT_NONE;
	}
	else if(encrypt == ENCRYPT_WEP) 
	{				
		wsc_enc = WSC_ENCRYPT_WEP;
	}	
	else if(encrypt == ENCRYPT_WPA)
	{
		wsc_enc = WSC_ENCRYPT_TKIP;
		wsc_auth = 2;	
		
#ifdef ACTIONTEC_WIFI_CERTIFICATED
		if(basic)
		{
			wpa_cipher = WPA_CIPHER_TKIP;
			if(!apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_cipher)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
				goto setErr;							
			}
		}
#endif

	}
	else if(encrypt == ENCRYPT_WPA2)
	{
		wsc_enc = WSC_ENCRYPT_AES;
		wsc_auth = 32;		

#ifdef ACTIONTEC_WIFI_CERTIFICATED
		if(basic)
		{
			wpa_cipher = WPA_CIPHER_AES;
			if(!apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa_cipher)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
				goto setErr;							
			}
		}
#endif
		
	}
	else if(encrypt == ENCRYPT_WPA2_MIXED)
	{
		wsc_enc = WSC_ENCRYPT_TKIPAES;
		wsc_auth = 34;

#ifdef ACTIONTEC_WIFI_CERTIFICATED
		if(basic)
		{
			wpa_cipher = WPA_CIPHER_MIXED;

			if(!apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_cipher)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
				goto setErr;							
			}
			
			if(!apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa_cipher)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
				goto setErr;							
			}
		}
#endif

	}
	
	if(!apmib_set( MIB_WLAN_ENCRYPT, (void *)&encrypt)) {
		strcpy(tmpBuf, ("Set MIB_WLAN_ENCRYPT mib error!"));
		goto setErr;
	}

	if(!apmib_set( MIB_WLAN_WSC_ENC, (void *)&wsc_enc)) {
		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_ENC mib error!"));
		goto setErr;
	}
	
	if(!apmib_set(MIB_WLAN_WSC_AUTH, (void *)&wsc_auth)) {
		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_AUTH error!"));
		goto setErr;
	}
	
	if(!apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&wsc_configured)) {
		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_CONFIGURED error!"));
		goto setErr;
	}

	if((encrypt == ENCRYPT_DISABLED) || (encrypt == ENCRYPT_WEP) || (encrypt == ENCRYPT_WPA))
	{
		disabled = 1;		
	}
	
	if (!apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&disabled)) {
		strcpy(tmpBuf, ("Set MIB_WLAN_WSC_DISABLE error!"));
		goto setErr;
	}
				
	updateVapWscDisable(wlan_idx, disabled);

#ifdef ACTIONTEC_TR69
	int id = 0;
	char encrypt_buf[24] = {0};
	char cipher_buf[24] = {0};

	if(wlan_idx == 1)
	{
		if(vwlan_idx > 0)			
		{
			id = vwlan_idx;
		}
	}
	else if(wlan_idx == 0)
	{
		if(vwlan_idx > 0)
		{
			id = 4 + vwlan_idx;
		}
	}	

	if(wpa_cipher != 0)
	{
		switch(wpa_cipher)
		{
			case WPA_CIPHER_TKIP:
				strcpy(cipher_buf, "TKIPEncryption");
				break;
			case WPA_CIPHER_AES:
				strcpy(cipher_buf, "AESEncryption");
				break;
			case WPA_CIPHER_MIXED:
				strcpy(cipher_buf, "TKIPandAESEncryption");
				break;
			default:
				break;
		}
	}

	if(id != 0)
	{
		switch(encrypt)
		{
			case ENCRYPT_DISABLED:
				strcpy(encrypt_buf, "None");
				break;
			case ENCRYPT_WEP:
				strcpy(encrypt_buf, "WEP-64");
				if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_WEPAuthenticationMode", "None"))	
				{
					sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WEPAuthenticationMode error!"), id);
					goto setErr;
				}
				break;
			case ENCRYPT_WPA:
				strcpy(encrypt_buf, "WPA-Personal");	

#ifdef ACTIONTEC_WIFI_CERTIFICATED
				if(basic)
				{
					if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_WPAEncryptionMode", cipher_buf))
					{
						sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPAEncryptionMode error!"), id);
						goto setErr;
					}
				}
#endif				
				break;
			case ENCRYPT_WPA2:
				strcpy(encrypt_buf, "WPA2-Personal");	

#ifdef ACTIONTEC_WIFI_CERTIFICATED
				if(basic)
				{
					if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_WPA2EncryptionMode", cipher_buf))
					{
						sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPA2EncryptionMode error!"), id);
						goto setErr;
					}
				}
#endif				
				break;
			case ENCRYPT_WPA2_MIXED:
				strcpy(encrypt_buf, "WPA-WPA2-Personal");	

#ifdef ACTIONTEC_WIFI_CERTIFICATED
				if(basic)
				{
					if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_WPAEncryptionMode", cipher_buf))
					{
						sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPAEncryptionMode error!"), id);
						goto setErr;
					}
					if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_WPA2EncryptionMode", cipher_buf))
					{
						sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPA2EncryptionMode error!"), id);
						goto setErr;
					}
				}
#endif				
				break;
			default:
				break;
		}	

		if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.ModeEnabled", encrypt_buf))
		{
			sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.ModeEnabled error!"), id);
			goto setErr;
		}
		if(tr69SetUnfreshLeafDataInt("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_RadiusEnabled", 0))
		{
			sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_RadiusEnabled error!"), id);
			goto setErr;
		}
	}
#endif
	
	return 1;		
setErr:
	return 0;
}

int setPSK(char *tmpBuf, char *strVal, ENCRYPT_T encrypt, int defaultKey, char *strSSID)
{
	char key[30] = {0};
	int wep = WEP64;
	int keyLen = WEP64_KEY_LEN * 2;	
	int keyType = KEY_HEX;
	int wpaAuth = WPA_AUTH_PSK;

	if(encrypt == ENCRYPT_WEP) 
	{
		if(defaultKey >= 0)
		{
			if(strlen(strVal) > 10)
			{
				wep = WEP128;
				keyLen = WEP128_KEY_LEN * 2;	
			}

			if (!apmib_set( MIB_WLAN_WEP, (void *)&wep)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WEP error!"));
				goto setErr;
			}

			if (!apmib_set( MIB_WLAN_WEP_KEY_TYPE, (void *)&keyType)) {
				strcpy(tmpBuf, ("Set WEP key type error!"));
				goto setErr;
			}

			if (!apmib_set( MIB_WLAN_WEP_DEFAULT_KEY, (void *)&defaultKey)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WEP_DEFAULT_KEY error!"));
				goto setErr;
			}
					
			if (!string_to_hex(strVal, key, keyLen)) {
				strcpy(tmpBuf, ("Invalid wep-key value!"));
				goto setErr;
			}

			if(wep == WEP64)
			{
				if(defaultKey == 0)

				{
					if (!apmib_set( MIB_WLAN_WEP64_KEY1, (void *)key)) {
						strcpy(tmpBuf, ("Set WEP64 key1 error!"));
						goto setErr;
					}	
				}
				else if(defaultKey == 1)
				{
					if (!apmib_set( MIB_WLAN_WEP64_KEY2, (void *)key)) {
						strcpy(tmpBuf, ("Set WEP64 key2 error!"));
						goto setErr;
					}
				}
				else if(defaultKey == 2)
				{
					if (!apmib_set( MIB_WLAN_WEP64_KEY3, (void *)key)) {
						strcpy(tmpBuf, ("Set WEP64 key3 error!"));
						goto setErr;
					}
				}
				else if(defaultKey == 3)
				{
					if (!apmib_set( MIB_WLAN_WEP64_KEY4, (void *)key)) {
						strcpy(tmpBuf, ("Set WEP64 key4 error!"));
						goto setErr;
					}
				}
			}
			else if(wep == WEP128)
			{
				if(defaultKey == 0)

				{
					if (!apmib_set( MIB_WLAN_WEP128_KEY1, (void *)key)) {
						strcpy(tmpBuf, ("Set WEP128 key1 error!"));
						goto setErr;
					}
				}
				else if(defaultKey == 1)
				{
					if (!apmib_set( MIB_WLAN_WEP128_KEY2, (void *)key)) {
						strcpy(tmpBuf, ("Set WEP128 key2 error!"));
						goto setErr;
					}
				}
				else if(defaultKey == 2)
				{
					if (!apmib_set( MIB_WLAN_WEP128_KEY3, (void *)key)) {
						strcpy(tmpBuf, ("Set WEP128 key3 error!"));
						goto setErr;
					}
				}
				else if(defaultKey == 3)
				{
					if (!apmib_set( MIB_WLAN_WEP128_KEY4, (void *)key)) {
						strcpy(tmpBuf, ("Set WEP128 key4 error!"));
						goto setErr;
					}
				}
			}
		}				
	}
	else if(encrypt == ENCRYPT_WPA || encrypt == ENCRYPT_WPA2 || encrypt == ENCRYPT_WPA2_MIXED)
	{
		if(strchr(strVal, FORM_INPUT_SPECIAL) || (strlen(strVal) == 64 && !IsValidHexadecimalString(strVal)))
		{
			strcpy(tmpBuf, ("Error! Password is invalid."));
			goto setErr;
		}
		
		if (!apmib_set(MIB_WLAN_WPA_PSK, (void *)strVal)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WPA_PSK error!"));
			goto setErr;
		}

		if (!apmib_set(MIB_WLAN_WSC_PSK, (void *)strVal)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WSC_PSK error!"));
			goto setErr;
		}		
	}	

#ifdef ACTIONTEC_TR69
	int id = 0;
	char encrypt_buf[24] = {0};
    char PreSharedKey[64+1] = {0};
	
	if(wlan_idx == 1)
	{
		if(vwlan_idx > 0)			
		{
			id = vwlan_idx;
		}
	}
	else if(wlan_idx == 0)
	{
		if(vwlan_idx > 0)
		{
			id = 4 + vwlan_idx;
		}
	}		
	
	if(id != 0)
	{
		switch(encrypt)
		{
			case ENCRYPT_DISABLED:
				strcpy(encrypt_buf, "None");
				break;
			case ENCRYPT_WEP:
				if(wep == WEP64)
					strcpy(encrypt_buf, "WEP-64");
				else if(wep == WEP128)
					strcpy(encrypt_buf, "WEP-128");	
				if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.WEPKey", strVal))
				{
					sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.WEPKey error!"), id);
					goto setErr;
				}					
				break;
            case ENCRYPT_WPA:
                strcpy(encrypt_buf, "WPA-Personal");
                if(strlen(strVal) == 64)
                {
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.PreSharedKey", strVal))	
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.PreSharedKey error!"), id);
                        goto setErr;
                    }
                    tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_PreSharedKey", strVal);
                }
                else if(strlen(strVal) >=8 && strlen(strVal) < 64)
                {
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.KeyPassphrase", strVal))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.KeyPassphrase error!"), id);
                        goto setErr;
                    }

                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_KeyPassPhrase", strVal))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_TWC_COM_KeyPassPhrase error!"), id);
                        goto setErr;
                    }
                    
                    pkcs5_pbkdf2(strVal, strSSID, strlen(strSSID), &PreSharedKey);
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_PreSharedKey", PreSharedKey))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_TWC_COM_PreSharedKey error!"), id);
                        goto setErr;
                    }
                }
                else
                    goto setErr;

                tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_WPAKey", strVal);                
                break;
            case ENCRYPT_WPA2:
                strcpy(encrypt_buf, "WPA2-Personal");
                if(strlen(strVal) == 64)
                {
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.PreSharedKey", strVal))	
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.PreSharedKey error!"), id);
                        goto setErr;
                    }
                    tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_PreSharedKey", strVal);
                }
                else if(strlen(strVal) >=8 && strlen(strVal) < 64)
                {
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.KeyPassphrase", strVal))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.KeyPassphrase error!"), id);
                        goto setErr;
                    }

                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_KeyPassPhrase", strVal))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_TWC_COM_KeyPassPhrase error!"), id);
                        goto setErr;
                    }
                    
                    pkcs5_pbkdf2(strVal, strSSID, strlen(strSSID), &PreSharedKey);
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_PreSharedKey", PreSharedKey))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_TWC_COM_PreSharedKey error!"), id);
                        goto setErr;
                    }
                }
                else
                    goto setErr;

                tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_WPAKey", strVal);                
                break;
            case ENCRYPT_WPA2_MIXED:
                strcpy(encrypt_buf, "WPA-WPA2-Personal");
                if(strlen(strVal) == 64)
                {
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.PreSharedKey", strVal))	
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.PreSharedKey error!"), id);
                        goto setErr;
                    }
                    tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_PreSharedKey", strVal);
                }
                else if(strlen(strVal) >=8 && strlen(strVal) < 64)
                {
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.KeyPassphrase", strVal))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.KeyPassphrase error!"), id);
                        goto setErr;
                    }
                    
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_KeyPassPhrase", strVal))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_TWC_COM_KeyPassPhrase error!"), id);
                        goto setErr;
                    }
                    
                    pkcs5_pbkdf2(strVal, strSSID, strlen(strSSID), &PreSharedKey);
                    if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_TWC_COM_PreSharedKey", PreSharedKey))
                    {
                        sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_TWC_COM_PreSharedKey error!"), id);
                        goto setErr;
                    }
                }
                else
                    goto setErr;

                tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.X_ACTIONTEC_COM_WPAKey", strVal);
                break;
            default:
                break;
        }

		if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", id, "Security.ModeEnabled", encrypt_buf))			
		{
			sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.ModeEnabled error!"), id);
			goto setErr;
		}	
	}
#endif

	return 1;	
setErr:
	return 0;	
}

int applyBasic(request *wp, char *tmpBuf, int both)
{
	char *strVal = NULL;
    char *strSSID = NULL;
	ENCRYPT_T encrypt;
	int rate = 0;
	int transmit = 0;
	int chanwidth = -1;

	vwlan_idx = 1;
	
	// 2.4 G
	if(both)
		wlan_idx = 1;

	if(wlan_idx == 1)
	{
		strVal = req_get_cstream_var(wp, ("wl2_enb"), "");
		if(strVal[0])
		{
			kinfo("wl2_enb: %s", strVal);			
			if(!setWlanDisabled(tmpBuf, strVal, 1))
				goto setErr_basic;
		}		
		
		strSSID = req_get_cstream_var(wp, ("wl2_ssid"), "");
		if(strSSID[0])
		{
			kinfo("wl2_ssid: %s", strSSID);		
			if(!setSSID(tmpBuf, strSSID))
				goto setErr_basic;
		}

		strVal = req_get_cstream_var(wp, ("wl2_security"), "");
		if(strVal[0])
		{
			kinfo("wl2_security: %s", strVal);
			encrypt = (ENCRYPT_T) strVal[0] - '0';
			 
			if(!setSecurity(tmpBuf, encrypt, 1))
				goto setErr_basic;
		}

		strVal = req_get_cstream_var(wp, ("wl2_psk"), "");
		if(strVal[0])
		{
			kinfo("wl2_psk: %s", strVal);
			if(!setPSK(tmpBuf, strVal, encrypt, 0, strSSID))
				goto setErr_basic;
		}	

		strVal = req_get_cstream_var(wp, ("wl2_band"), "");
		if(strVal[0])
		{
			kinfo("wl2_band: %s", strVal);		
			if(!setBAND(tmpBuf, strVal))
				goto setErr_basic;
		}
		
		strVal = req_get_cstream_var(wp, ("wl2_rate"), "");
		if (strVal[0]) 
		{
			kinfo("wl2_rate: %s", strVal);	
			if(!setRate(tmpBuf, strVal))
				goto setErr_basic;					
		}

		vwlan_idx = 0;		
		strVal = req_get_cstream_var(wp, ("wl2_chanwidth"), "");
		if(strVal[0])
		{
			kinfo("wl2_chanwidth: %s", strVal);
			chanwidth = atoi(strVal);
			if(!setChanwidth(tmpBuf, strVal))
				goto setErr_basic;
		}

		strVal = req_get_cstream_var(wp, ("wl2_channel"), "");
		if(strVal[0])
		{
			kinfo("wl2_channel: %s", strVal);
			if(!setChannel(tmpBuf, strVal, chanwidth))
				goto setErr_basic;
		}	

		strVal = req_get_cstream_var(wp, ("wl2_transmit"), "");
		if (strVal[0]) 
		{
			kinfo("wl2_transmit: %s", strVal);
			if(!setTransmit(tmpBuf, strVal))
				goto setErr_basic;			
		}		
		vwlan_idx = 1;			
	}
	
	// 5G
	if(both)
		wlan_idx = 0;

	if(wlan_idx == 0)
	{
		strVal = req_get_cstream_var(wp, ("wl5_enb"), "");
		if(strVal[0])
		{
			kinfo("wl5_enb: %s", strVal);		
			if(!setWlanDisabled(tmpBuf, strVal, 1))
				goto setErr_basic;
		}		

		strSSID = req_get_cstream_var(wp, ("wl5_ssid"), "");
		if(strSSID[0])
		{
			kinfo("wl5_ssid: %s", strSSID);
			if(!setSSID(tmpBuf, strSSID))
				goto setErr_basic;
		}

		strVal = req_get_cstream_var(wp, ("wl5_security"), "");
		if(strVal[0])
		{
			kinfo("wl5_security: %s", strVal);
			encrypt = (ENCRYPT_T) strVal[0] - '0';
			
			if(!setSecurity(tmpBuf, encrypt, 1))
				goto setErr_basic;
		}

		strVal = req_get_cstream_var(wp, ("wl5_psk"), "");
		if(strVal[0])
		{
			kinfo("wl5_psk: %s", strVal);
			if(!setPSK(tmpBuf, strVal, encrypt, 0, strSSID))
				goto setErr_basic;
		}

		strVal = req_get_cstream_var(wp, ("wl5_band"), "");
		if(strVal[0])
		{
			kinfo("wl5_band: %s", strVal);		
			if(!setBAND(tmpBuf, strVal))
				goto setErr_basic;
		}

		strVal = req_get_cstream_var(wp, ("wl5_rate"), "");
		if (strVal[0]) 
		{
			kinfo("wl5_rate: %s", strVal);	
			if(!setRate(tmpBuf, strVal))
				goto setErr_basic;				
		}

		vwlan_idx = 0;		
		strVal = req_get_cstream_var(wp, ("wl5_chanwidth"), "");
		if(strVal[0])
		{
			kinfo("wl5_chanwidth: %s", strVal);
			chanwidth = atoi(strVal);
			if(!setChanwidth(tmpBuf, strVal))
				goto setErr_basic;
		}

		strVal = req_get_cstream_var(wp, ("wl5_channel"), "");
		if(strVal[0])
		{
			kinfo("wl5_channel: %s", strVal);
			if(!setChannel(tmpBuf, strVal, chanwidth))
				goto setErr_basic;
		}			

		strVal = req_get_cstream_var(wp, ("wl5_transmit"), "");
		if (strVal[0]) 
		{
			kinfo("wl5_transmit: %s", strVal);	
			if(!setTransmit(tmpBuf, strVal))
				goto setErr_basic;	
		}
		
		vwlan_idx = 1;		
	}
	
	return 1;
		
setErr_basic:
	return 0;
}

int getvalueBasic(request *wp, char *tmpBuf, int both, int security)
{
	int disabled = 0;
	int band = 0;
	int chanwidth = 0;
	int coexist = 0;
	int sideband = 0;
	int channumber = 0;
	char cur_channumber[BUF_LEN8] = {0};
	int tr69_id = 0;
	char tr69_fullname[BUF_LEN100] = {0};
	int encrypt = 0;
	int wep = 0;
	int wep_defaultkey = 0;
	int wep_authtype = 0;
	int wpa_cipher = 0;
	int wpa2_cipher = 0;
	int wpa_auth = 0;
	char wep_key[100] = {0};
	char ssid[1024] = {0};
	char wep64_key1[11] = {0};
	char wep64_key2[11] = {0};
	char wep64_key3[11] = {0};
	char wep64_key4[11] = {0};
	char wep128_key1[27] = {0};
	char wep128_key2[27] = {0};
	char wep128_key3[27] = {0};
	char wep128_key4[27] = {0};
	char wpa_psk[100] = {0};
	char rs_ip[20] = {0};
	char rs_ip_buf[20] = {0};
	int rs_port = 0;
	char rs_password[65] = {0};
	int rate_enabled = 0;
	int rate = 0;
	int broadcast = 0;
	int wmm = 0;
	int transmit = 0;
	char wlan_buf[12] = {0};
 	int clients = 0;	
	char sys_buf[BUF_LEN100]={0};
	char line[BUF_LEN100]={0};
	int sleep_time=0;
	FILE *fp;
#ifdef AEI_RADIO_CONTROL_ENABLE
  char buffer[32] = {0};
#endif
	
	if(!both)	
	{
		if(!security)
		{
			vwlan_idx = curr_ssid;
			if (!apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disabled)) {				
				strcpy(tmpBuf, ("Get MIB_WLAN_WLAN_DISABLED error!"));
				goto setErr_basic;
			}

			if(disabled == 0)
			{
				sprintf(wlan_buf, "wlan%d-va%d", wlan_idx, (vwlan_idx - 1));
				if (getWlStaNum(wlan_buf, &clients) < 0)
					clients = 0;
			}			
			
			vwlan_idx = 0;	
            if (!apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&disabled)) {				
				strcpy(tmpBuf, ("Get MIB_WLAN_WLAN_DISABLED error!"));
				goto setErr_basic;
			}
            
			if (!apmib_get(MIB_WLAN_RFPOWER_SCALE, (void *)&transmit)) {
				strcpy(tmpBuf, ("Get MIB_WLAN_RFPOWER_SCALE error!"));
				goto setErr_basic;
			}
			vwlan_idx = curr_ssid;
		}

		if (!apmib_get(MIB_WLAN_BAND, (void *)&band)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_BAND error!"));
			goto setErr_basic;
		}

		if (!apmib_get(MIB_WLAN_RATE_ADAPTIVE_ENABLED, (void *)&rate_enabled)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_RATE_ADAPTIVE_ENABLED error!"));
			goto setErr_basic;
		}

		if (!apmib_get(MIB_WLAN_FIX_RATE, (void *)&rate)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_FIX_RATE error!"));
			goto setErr_basic;
		}	
		
		vwlan_idx = 0;
		if (!apmib_get(MIB_WLAN_CHANNEL_BONDING, (void *)&chanwidth)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_CHANNEL_BONDING error!"));
			goto setErr_basic;
		}

    		snprintf(sys_buf,sizeof(sys_buf),"iwpriv wlan%d get_mib Non40MSTADeny > %s", wlan_idx,NON40DENY_FILE);
    		system(sys_buf);

		fp = fopen(NON40DENY_FILE, "r");
		if (fp == NULL)
		{
		strcpy(tmpBuf, ("Fopen /tmp/non40deny_file.txt error!"));		
   		goto setErr_basic;
		}


		for (;;){
		      if(fgets(line,sizeof(line),fp) == NULL)
		      {
		      	    sleep(1);
			    sleep_time++;
			    if(sleep_time==4)
			       break;
    			    snprintf(sys_buf,sizeof(sys_buf),"iwpriv wlan%d get_mib Non40MSTADeny > %s", wlan_idx,NON40DENY_FILE);
		           system(sys_buf);
		           fclose(fp);
		           fp = fopen(NON40DENY_FILE, "r");
		           if (fp == NULL)
		           {
			        strcpy(tmpBuf, ("Fopen2 /tmp/non40deny_file.txt error!"));		
		               goto setErr_basic;
		           }
		           continue;		        
		      }
		      else
			    break;
		 }

		fclose(fp);
		

		if (!apmib_get(MIB_WLAN_CONTROL_SIDEBAND, (void *)&sideband)) {
			strcpy(tmpBuf, ("Get MIB_WLAN_CONTROL_SIDEBAND error!"));
			goto setErr_basic;
		}

		if (!apmib_get(MIB_WLAN_CHANNEL, (void *)&channumber)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_CHANNEL error!"));
			goto setErr_basic;
		}

		if(1 == wlan_idx)
			tr69_id = 1;
		else if(0 == wlan_idx)
			tr69_id = 2;
		snprintf(tr69_fullname,sizeof(tr69_fullname),"Device.WiFi.Radio.%d",tr69_id);
		if(tr69GetUnfreshLeafData(cur_channumber, tr69_fullname ,"Channel"))
		{
			sprintf(tmpBuf, ("Get Device.WiFi.Radio.%d.Channel error!"),tr69_id);
			goto setErr_basic;
		}
	//fprintf(stdout,"\n\n@1@chanwidth=%d,line=%s,sideband=%d,channumber=%d\n\n",chanwidth,line,sideband,channumber);
		
		vwlan_idx = curr_ssid;

		if (!apmib_get(MIB_WLAN_ENCRYPT, (void *)&encrypt)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_ENCRYPT error!"));
			goto setErr_basic;
		}	

		if (!apmib_get(MIB_WLAN_WEP, (void *)&wep)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_WEP error!"));
			goto setErr_basic;
		}

		if (!apmib_get(MIB_WLAN_WEP_DEFAULT_KEY, (void *)&wep_defaultkey)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_WEP_DEFAULT_KEY error!"));
			goto setErr_basic;
		}

		if (!apmib_get(MIB_WLAN_SSID, (void *)ssid)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_SSID error!"));
			goto setErr_basic;
		}

		if(wep == WEP64)
		{
			if(wep_defaultkey == 0)
			{
				if (!apmib_get(MIB_WLAN_WEP64_KEY1, (void *)wep64_key1)) {				
					strcpy(tmpBuf, ("Get MIB_WLAN_WEP64_KEY1 error!"));
					goto setErr_basic;
				}
				convert_bin_to_str(wep64_key1, 5, wep_key);			
			}
			else if(wep_defaultkey == 1)
			{
				if (!apmib_get(MIB_WLAN_WEP64_KEY2, (void *)wep64_key2)) {				
					strcpy(tmpBuf, ("Get MIB_WLAN_WEP64_KEY2 error!"));
					goto setErr_basic;
				}
				convert_bin_to_str(wep64_key2, 5, wep_key);	
			}
			else if(wep_defaultkey == 2)
			{
				if (!apmib_get(MIB_WLAN_WEP64_KEY3, (void *)wep64_key3)) {				
					strcpy(tmpBuf, ("Get MIB_WLAN_WEP64_KEY3 error!"));
					goto setErr_basic;
				}
				convert_bin_to_str(wep64_key3, 5, wep_key);	
			}
			else if(wep_defaultkey == 3)
			{
				if (!apmib_get(MIB_WLAN_WEP64_KEY4, (void *)wep64_key4)) {				
					strcpy(tmpBuf, ("Get MIB_WLAN_WEP64_KEY4 error!"));
					goto setErr_basic;
				}
				convert_bin_to_str(wep64_key4, 5, wep_key);	
			}
		}
		else if(wep == WEP128)
		{
			if(wep_defaultkey == 0)
			{
				if (!apmib_get(MIB_WLAN_WEP128_KEY1, (void *)wep128_key1)) {				
					strcpy(tmpBuf, ("Get MIB_WLAN_WEP128_KEY1 error!"));
					goto setErr_basic;
				}
				convert_bin_to_str(wep128_key1, 13, wep_key);	
			}
			else if(wep_defaultkey == 1)
			{
				if (!apmib_get(MIB_WLAN_WEP128_KEY2, (void *)wep128_key2)) {				
					strcpy(tmpBuf, ("Get MIB_WLAN_WEP128_KEY2 error!"));
					goto setErr_basic;
				}
				convert_bin_to_str(wep128_key2, 13, wep_key);	
			}
			else if(wep_defaultkey == 2)
			{
				if (!apmib_get(MIB_WLAN_WEP128_KEY3, (void *)wep128_key3)) {				
					strcpy(tmpBuf, ("Get MIB_WLAN_WEP128_KEY3 error!"));
					goto setErr_basic;
				}
				convert_bin_to_str(wep128_key3, 13, wep_key);	
			}
			else if(wep_defaultkey == 3)
			{
				if (!apmib_get(MIB_WLAN_WEP128_KEY4, (void *)wep128_key4)) {				
					strcpy(tmpBuf, ("Get MIB_WLAN_WEP128_KEY4 error!"));
					goto setErr_basic;
				}
				convert_bin_to_str(wep128_key4, 13, wep_key);	
			}
		}				 

		if (!apmib_get(MIB_WLAN_WPA_PSK, (void *)wpa_psk)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_WPA_PSK error!"));
			goto setErr_basic;
		}	

		if (!apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_cipher)) {				
			strcpy(tmpBuf, ("Get MIB_WLAN_WPA_CIPHER_SUITE error!"));
			goto setErr_basic;
		}

        if (!apmib_get(MIB_WLAN_WPA_AUTH, (void *)&wpa_auth)) {
            strcpy(tmpBuf, ("Get MIB_WLAN_WPA_AUTH error!"));
            goto setErr_basic;
        }
        if (!apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa2_cipher)) {             
            strcpy(tmpBuf, ("Get MIB_WLAN_WPA2_CIPHER_SUITE error!"));
            goto setErr_basic;
        }
		if(security)
		{
			if (!apmib_get(MIB_WLAN_HIDDEN_SSID, (void *)&broadcast)) {
				strcpy(tmpBuf, ("Get MIB_WLAN_HIDDEN_SSID failed!"));
				goto setErr_basic;
			}

			if (!apmib_get(MIB_WLAN_WMM_ENABLED, (void *)&wmm)) {
				strcpy(tmpBuf, ("Get MIB_WLAN_WMM_ENABLED failed!"));
				goto setErr_basic;
			}
		
			if (!apmib_get(MIB_WLAN_AUTH_TYPE, (void *)&wep_authtype)) {				
				strcpy(tmpBuf, ("Get MIB_WLAN_AUTH_TYPE error!"));
				goto setErr_basic;
			}	

			if (!apmib_get(MIB_WLAN_WPA_AUTH, (void *)&wpa_auth)) {				
				strcpy(tmpBuf, ("Get MIB_WLAN_WPA_AUTH error!"));
				goto setErr_basic;
			}		

			if (!apmib_get(MIB_WLAN_RS_IP, (void *)rs_ip_buf)) {				
				strcpy(tmpBuf, ("Get MIB_WLAN_RS_IP error!"));
				goto setErr_basic;
			}

			if(!memcmp(rs_ip_buf, "\x0\x0\x0\x0", 4))
				strncpy(rs_ip, "", 20);
			else
				strncpy(rs_ip, inet_ntoa(*((struct in_addr *)rs_ip_buf)), 20);						

			if (!apmib_get(MIB_WLAN_RS_PORT, (void *)&rs_port)) {				
				strcpy(tmpBuf, ("Get MIB_WLAN_RS_PORT error!"));
				goto setErr_basic;
			}

			if (!apmib_get(MIB_WLAN_RS_PASSWORD, (void *)rs_password)) {				
				strcpy(tmpBuf, ("Get MIB_WLAN_RS_PASSWORD error!"));
				goto setErr_basic;
			}

			changeString(rs_ip);
			changeString(rs_password);
		}

		speciCharEncode(ssid, sizeof(ssid));
		changeString(ssid);		
		changeString(wep_key);		
		changeString(wpa_psk);		
#ifdef AEI_RADIO_CONTROL_ENABLE
    if(tr69GetUnfreshLeafData(buffer, "Device.UserInterface", "X_TWC_COM_InhibitRadioControl"))
    {
      sprintf(tmpBuf, "Get Device.UserInterface.X_TWC_COM_InhibitRadioControl error!");
      goto setErr_basic;
    }
#endif
		if(!security)
		{
#ifdef AEI_RADIO_CONTROL_ENABLE
			req_format_write(wp, ("%d|%d|%d|%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s|%s|%d|%s|%d||"), 
        wlan_idx, disabled, chanwidth, line, sideband, channumber, encrypt, (band - 1), wpa_cipher, rate_enabled, rate, transmit, 
        clients,
        ssid, wep_key, wpa_psk,cur_channumber,wpa_auth, buffer,wpa2_cipher);
#else
      req_format_write(wp, ("%d|%d|%d|%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%s|%s|%s|%d||"), 
        wlan_idx, disabled, chanwidth, line, sideband, channumber, encrypt, (band - 1), wpa_cipher, rate_enabled, rate, transmit, 
        clients,
        ssid, wep_key, wpa_psk,cur_channumber,wpa_auth);
#endif      
		}
		else 
		{
#ifdef AEI_RADIO_CONTROL_ENABLE
            req_format_write(wp, ("%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%d|%d|%d|%d|%d|%s|%s|%s|%s|%s|%s||"), 
                encrypt, wep_defaultkey, wep_authtype, wpa_cipher, wpa2_cipher, wpa_auth, rs_port, (band - 1), chanwidth, line, sideband, 
                channumber, broadcast, wmm, rate_enabled, rate, 
                ssid, wep_key, wpa_psk, rs_ip, rs_password, buffer);
#else
			req_format_write(wp, ("%d|%d|%d|%d|%d|%d|%d|%d|%d|%s|%d|%d|%d|%d|%d|%d|%s|%s|%s|%s|%s||"), 
				encrypt, wep_defaultkey, wep_authtype, wpa_cipher, wpa2_cipher, wpa_auth, rs_port, (band - 1), chanwidth, line, sideband, 
				channumber, broadcast, wmm, rate_enabled, rate, 
				ssid, wep_key, wpa_psk, rs_ip, rs_password);
#endif	
		}
	}
	else
	{
		wlan_idx = 1;
		if(!getvalueBasic(wp, tmpBuf, 0, 0))
			goto setErr_basic;

		wlan_idx = 0;
		if(!getvalueBasic(wp, tmpBuf, 0, 0))
			goto setErr_basic;
	}
	return 1;
		
setErr_basic:
	return 0;
}

void actWlbasic(request *wp, char *path, char *query)
{
	char *strVal = NULL;
	char *action = NULL;	
	int wl_freq = 0;
	
	both_freq = 0;	
	wlan_idx = 0;
	curr_ssid = 1;
	
	strVal = req_get_cstream_var(wp, ("wl_freq"), "");
	if(strVal[0])
	{
		kinfo("wl_freq: %s", strVal);
		wl_freq = atoi(strVal);

		if(wl_freq == 10)
			wlan_idx = 1;
		else if(wl_freq == 11)
			wlan_idx = 0;
		else if(wl_freq == 0)
			both_freq = 1;
	}

	action = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", action);
	
	if(!safe_strcmp(action, "apply"))
	{
		if(!applyBasic(wp, error_buf, both_freq))
			goto setErr_basic;

#ifdef ACTIONTEC_TR69
		if(tr69SaveNow())
        {
            strcpy(error_buf, "tr69SaveNow error!");
            goto setErr_basic;
        }      
#endif
	}
	else if(!safe_strcmp(action, "getvalue"))
	{
		if(!getvalueBasic(wp, error_buf, both_freq, 0))
			goto setErr_basic;

		return;
	}
	
	applyPage(wp, INIT_ALL);
	return;

setErr_basic:
	kerror("error_buf: %s", error_buf);
	backPage(wp);		
}

int applySecurity(request *wp, char *tmpBuf)
{
	char *strVal = NULL;
    char *strSSID = NULL;
	int wl_secmode = 0;
	int wl_wpamode = 0;
	int authType = 0;
	int defaultKey = 0;
	int wpa_cipher = 0;
	int wsc_enc = 0;
	int disabled = 0;
	int authentication = 0;
	int server_port = 0;
	ENCRYPT_T encrypt;
	struct in_addr inIp;
	int broadcast = 0;
	int wmm = 0;
	int chanwidth = -1;
	int wsc_disabled = 0;

#ifdef ACTIONTEC_TR69
	int tr69_id = 0; 
		
	if(wlan_idx == 1)
	{
		if(vwlan_idx > 0)			
		{
			tr69_id = vwlan_idx;
		}
	}
	else if(wlan_idx == 0)
	{
		if(vwlan_idx > 0)
		{
			tr69_id = 4 + vwlan_idx;
		}
	}	
#endif
	
	strSSID = req_get_cstream_var(wp, ("wl_ssid"), "");
	if(strSSID[0])
	{
		kinfo("wl_ssid: %s", strSSID);	
		if(!setSSID(tmpBuf, strSSID))
			goto setErr_security;
	}

	strVal = req_get_cstream_var(wp, ("wl_broadcast"), "");
	if(strVal[0])
	{
		kinfo("wl_broadcast: %s", strVal);
		broadcast = atoi(strVal);
	
		if (!apmib_set(MIB_WLAN_HIDDEN_SSID, (void *)&broadcast)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_HIDDEN_SSID failed!"));
			goto setErr_security;
		}	

		if(broadcast == 1)
		{
			wsc_disabled = 1;
			
			if (!apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&wsc_disabled)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WSC_DISABLE error!"));
				goto setErr_security;
			}
								
			updateVapWscDisable(wlan_idx, wsc_disabled);
		}
	
#ifdef ACTIONTEC_TR69
		int advertisement_enabled = 0;
	
		if(broadcast == 0)
			advertisement_enabled = 1;
				
		if(tr69_id != 0)
		{
			if(tr69SetUnfreshLeafDataInt("Device.WiFi.AccessPoint", tr69_id, "SSIDAdvertisementEnabled", advertisement_enabled))	
			{
				sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.SSIDAdvertisementEnabled error!"), tr69_id);
				goto setErr_security;
			}
		}			
#endif
	}
	
	strVal = req_get_cstream_var(wp, ("wl_wmm"), "");
	if(strVal[0])
	{
		kinfo("wl_wmm: %s", strVal);
		wmm = atoi(strVal);
				
		if (!apmib_set(MIB_WLAN_WMM_ENABLED, (void *)&wmm)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WMM_ENABLED failed!"));
			goto setErr_security;
		}
	
#ifdef ACTIONTEC_TR69			
		if(tr69_id != 0)
		{
			if(tr69SetUnfreshLeafDataInt("Device.WiFi.AccessPoint", tr69_id, "WMMEnable", wmm))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.WMMEnable error!"), tr69_id);
				goto setErr_security;
			}
		}			
#endif
	}		

	strVal = req_get_cstream_var(wp, ("wl_band"), "");
	if(strVal[0])
	{
		kinfo("wl_band: %s", strVal);		
		if(!setBAND(tmpBuf, strVal))
			goto setErr_security;
	}

	strVal = req_get_cstream_var(wp, ("wl_rate"), "");
	if (strVal[0]) 
	{
		kinfo("wl_rate: %s", strVal);	
		if(!setRate(tmpBuf, strVal))
			goto setErr_security;					
	}

	vwlan_idx = 0;
	strVal = req_get_cstream_var(wp, ("wl_chanwidth"), "");
	if(strVal[0])
	{
		kinfo("wl_chanwidth: %s", strVal);
		chanwidth = atoi(strVal);
		if(!setChanwidth(tmpBuf, strVal))
			goto setErr_security;
	}
		
	strVal = req_get_cstream_var(wp, ("wl_channel"), "");
	if(strVal[0])
	{
		kinfo("wl_channel: %s", strVal);
		if(!setChannel(tmpBuf, strVal, chanwidth))
			goto setErr_security;
	}	
	vwlan_idx = curr_ssid;

	strVal = req_get_cstream_var(wp, ("wl_secmode"), "");
	if(strVal[0])
	{
		kinfo("wl_secmode: %s", strVal);
		wl_secmode = atoi(strVal);
	}

	strVal = req_get_cstream_var(wp, ("wl_wpamode"), "");
	if(strVal[0])
	{
		kinfo("wl_wpamode: %s", strVal);	
		wl_wpamode = atoi(strVal);
	}

	if(wl_secmode == 0)
		encrypt = ENCRYPT_DISABLED;
	else if(wl_secmode == 1)
		encrypt = ENCRYPT_WEP;
	else if(wl_secmode == 2)
	{
		if(wl_wpamode == 2)
			encrypt = ENCRYPT_WPA;
		else if(wl_wpamode == 4)
			encrypt = ENCRYPT_WPA2;
		else if(wl_wpamode == 6)
			encrypt = ENCRYPT_WPA2_MIXED;
	}

	if(!setSecurity(tmpBuf, encrypt, 0))
		goto setErr_security;
	

	strVal = req_get_cstream_var(wp, ("wl_wepmode"), "");
	if(strVal[0])
	{
		kinfo("wl_wepmode: %s", strVal);	
		authType = atoi(strVal);
		
		if (!apmib_set(MIB_WLAN_AUTH_TYPE, (void *)&authType)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_AUTH_TYPE error!"));
			goto setErr_security;
		}

#ifdef ACTIONTEC_TR69
		char wepmode_buf[24] = {0};
		if(authType == 0)
			strcpy(wepmode_buf, "None");
		else if(authType == 1)
			strcpy(wepmode_buf, "SharedAuthentication");
		else if(authType == 2)
			strcpy(wepmode_buf, "Auto");
		
		if(tr69_id != 0)
		{
			if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", tr69_id, "Security.X_ACTIONTEC_COM_WEPAuthenticationMode", wepmode_buf))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WEPAuthenticationMode error!"), tr69_id);
				goto setErr_security;
			}	
		}
#endif
	}

	strVal = req_get_cstream_var(wp, ("wl_primarykey"), "");
	if(strVal[0])
	{
		kinfo("wl_primarykey: %s", strVal);	
		defaultKey = atoi(strVal);		
	}

	strVal = req_get_cstream_var(wp, ("wl_wepkey1"), "");
	if(strVal[0])
	{
		kinfo("wl_wepkey1: %s", strVal);
		if(defaultKey == 0)
		{
			if(!setPSK(tmpBuf, strVal, encrypt, defaultKey, strSSID))
				goto setErr_security;
		}
	}

	strVal = req_get_cstream_var(wp, ("wl_wepkey2"), "");
	if(strVal[0])
	{
		kinfo("wl_wepkey2: %s", strVal);
		if(defaultKey == 1)
		{
			if(!setPSK(tmpBuf, strVal, encrypt, defaultKey, strSSID))
				goto setErr_security;
		}
	}

	strVal = req_get_cstream_var(wp, ("wl_wepkey3"), "");
	if(strVal[0])
	{
		kinfo("wl_wepkey3: %s", strVal);	
		if(defaultKey == 2)
		{
			if(!setPSK(tmpBuf, strVal, encrypt, defaultKey, strSSID))
				goto setErr_security;
		}
	}

	strVal = req_get_cstream_var(wp, ("wl_wepkey4"), "");
	if(strVal[0])
	{
		kinfo("wl_wepkey4: %s", strVal);	
		if(defaultKey == 3)
		{
			if(!setPSK(tmpBuf, strVal, encrypt, defaultKey, strSSID))
				goto setErr_security;
		}
	}

	strVal = req_get_cstream_var(wp, ("wl_cypher"), "");
	if(strVal[0])
	{
		kinfo("wl_cypher: %s", strVal);	
		wpa_cipher = atoi(strVal);						

		if((encrypt == ENCRYPT_WPA) || (encrypt == ENCRYPT_WPA2_MIXED))
		{			
			if (!apmib_set(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_cipher)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WPA_CIPHER_SUITE failed!"));
				goto setErr_security;							
			}
		}

		if((encrypt == ENCRYPT_WPA2) || (encrypt == ENCRYPT_WPA2_MIXED))
		{
			if (!apmib_set(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa_cipher)) {
				strcpy(tmpBuf, ("Set MIB_WLAN_WPA2_CIPHER_SUITE failed!"));
				goto setErr_security;							
			}
		}

		if(wpa_cipher == WPA_CIPHER_TKIP)
			wsc_enc =  WSC_ENCRYPT_TKIP;
		else if(wpa_cipher == WPA_CIPHER_AES)
			wsc_enc =  WSC_ENCRYPT_AES;
		else if(wpa_cipher == WPA_CIPHER_MIXED)
			wsc_enc =  WSC_ENCRYPT_TKIPAES;

		if(!apmib_set(MIB_WLAN_WSC_ENC, (void *)&wsc_enc)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WSC_ENC mib error!"));
			goto setErr_security;
		}

		if((encrypt == ENCRYPT_DISABLED) || (encrypt == ENCRYPT_WEP)
			|| (encrypt == ENCRYPT_WPA) || (wpa_cipher == WPA_CIPHER_TKIP))
		{		
			wsc_disabled = 1;			
		}
		
		if (!apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&wsc_disabled)) {
			strcpy(tmpBuf, ("Set MIB_WLAN_WSC_DISABLE error!"));
			goto setErr_security;
		}
							
		updateVapWscDisable(wlan_idx, wsc_disabled);

#ifdef ACTIONTEC_TR69
		char cipher_buf[24] = {0};

		if(wpa_cipher != 0)
		{
			switch(wpa_cipher)
			{
				case WPA_CIPHER_TKIP:
					strcpy(cipher_buf, "TKIPEncryption");
					break;
				case WPA_CIPHER_AES:
					strcpy(cipher_buf, "AESEncryption");
					break;
				case WPA_CIPHER_MIXED:
					strcpy(cipher_buf, "TKIPandAESEncryption");
					break;
				default:
					break;
			}
		}

		if(tr69_id != 0)
		{
			switch(encrypt)
			{			
				case ENCRYPT_WPA:				
					if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", tr69_id, "Security.X_ACTIONTEC_COM_WPAEncryptionMode", cipher_buf))
					{
						sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPAEncryptionMode error!"), tr69_id);
						goto setErr_security;
					}	
					break;
				case ENCRYPT_WPA2:				
					if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", tr69_id, "Security.X_ACTIONTEC_COM_WPA2EncryptionMode", cipher_buf))
					{
						sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPA2EncryptionMode error!"), tr69_id);
						goto setErr_security;
					}	
					break;
				case ENCRYPT_WPA2_MIXED:					
					if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", tr69_id, "Security.X_ACTIONTEC_COM_WPAEncryptionMode", cipher_buf))
					{
						sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPAEncryptionMode error!"), tr69_id);
						goto setErr_security;
					}	
					if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", tr69_id, "Security.X_ACTIONTEC_COM_WPA2EncryptionMode", cipher_buf))
					{
						sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPA2EncryptionMode error!"), tr69_id);
						goto setErr_security;
					}	
					break;
				default:
					break;
			}	
		}
#endif		
	}

	strVal = req_get_cstream_var(wp, ("wl_authentication"), "");
	if(strVal[0])
	{
		kinfo("wl_authentication: %s", strVal);
		authentication = atoi(strVal);

		if (!apmib_set(MIB_WLAN_WPA_AUTH, (void *)&authentication)) {				
			strcpy(tmpBuf, ("Set MIB_WLAN_WPA_AUTH error!"));
			goto setErr_security;
		}
	}
	
	if(authentication == 1)
	{
		strVal = req_get_cstream_var(wp, ("wl_serverip"), "");
		if(strVal[0])
		{
			kinfo("wl_serverip: %s", strVal);
			
			if ( !inet_aton(strVal, &inIp) ) {
				strcpy(tmpBuf, ("Invalid RS IP-address value!"));
				goto setErr_security;
			}
			if (!apmib_set(MIB_WLAN_RS_IP, (void *)&inIp)) {
				strcpy(tmpBuf, ("Set RS IP-address error!"));
				goto setErr_security;
			}
			
#ifdef ACTIONTEC_TR69
			if(tr69_id != 0)
			{
				if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", tr69_id, "Security.RadiusServerIPAddr", strVal))
				{
					sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.RadiusServerIPAddr error!"), tr69_id);
					goto setErr_security;
				}
			}
#endif
		}

		strVal = req_get_cstream_var(wp, ("wl_serverport"), "");
		if(strVal[0])
		{
			kinfo("wl_serverport: %s", strVal);

			if (!string_to_dec(strVal, &server_port) || (server_port <= 0) || (server_port > 65535)) {
				strcpy(tmpBuf, ("Error! Invalid value of RS port number."));
				goto setErr_security;
			}
			if ( !apmib_set(MIB_WLAN_RS_PORT, (void *)&server_port)) {
				strcpy(tmpBuf, ("Set RS port error!"));
				goto setErr_security;
			}

#ifdef ACTIONTEC_TR69
			if(tr69_id != 0)
			{
				if(tr69SetUnfreshLeafDataInt("Device.WiFi.AccessPoint", tr69_id, "Security.RadiusServerPort", server_port))
				{
					sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.RadiusServerPort error!"), tr69_id);
					goto setErr_security;
				}
			}
#endif
		}

		strVal = req_get_cstream_var(wp, ("wl_serverpassword"), "");
		if(strVal[0])
		{
			kinfo("wl_serverpassword: %s", strVal);

			if (strlen(strVal) > (MAX_RS_PASS_LEN -1) ) {
				strcpy(tmpBuf, ("RS password length too long!"));
				goto setErr_security;
			}
			else if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(tmpBuf, ("Error! RS password is invalid."));
				goto setErr_security;
			}
			
			if (!apmib_set(MIB_WLAN_RS_PASSWORD, (void *)strVal)) {
				strcpy(tmpBuf, ("Set RS password error!"));
				goto setErr_security;
			}

#ifdef ACTIONTEC_TR69
			if(tr69_id != 0)
			{
				if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", tr69_id, "Security.RadiusSecret", strVal))
				{
					sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.RadiusSecret error!"), tr69_id);
					goto setErr_security;
				}
			}
#endif
		}
	}
	else if(authentication == 2)
	{
		strVal = req_get_cstream_var(wp, ("wl_psk"), "");
		if(strVal[0])
		{
			kinfo("wl_psk: %s", strVal);
			if(!setPSK(tmpBuf, strVal, encrypt, -1, strSSID))
				goto setErr_security;
		}
	}

#ifdef ACTIONTEC_TR69
	char encrypt_buf[24] = {0};

	switch(encrypt)
	{			
		case ENCRYPT_WPA:					
			if(authentication == 1)
				strcpy(encrypt_buf, "WPA-Enterprise");
			else if(authentication == 2)
				strcpy(encrypt_buf, "WPA-Personal");
			break;
		case ENCRYPT_WPA2:				
			if(authentication == 1)
				strcpy(encrypt_buf, "WPA2-Enterprise");
			else if(authentication == 2)
				strcpy(encrypt_buf, "WPA2-Personal");
			break;
		case ENCRYPT_WPA2_MIXED:					
			if(authentication == 1)
				strcpy(encrypt_buf, "WPA-WPA2-Enterprise");
			else if(authentication == 2)
				strcpy(encrypt_buf, "WPA-WPA2-Personal");
			break;
		default:
			break;
	}	

	if(tr69_id != 0)
	{
		if(safe_strcmp(encrypt_buf, ""))
		{
			if(tr69SetUnfreshLeafDataString("Device.WiFi.AccessPoint", tr69_id, "Security.ModeEnabled", encrypt_buf))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.ModeEnabled error!"), tr69_id);
				goto setErr_security;
			}
		}
		
		if(authentication == 1)
		{
			if(tr69SetUnfreshLeafDataInt("Device.WiFi.AccessPoint", tr69_id, "Security.X_ACTIONTEC_COM_RadiusEnabled", 1))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_RadiusEnabled error!"), tr69_id);
				goto setErr_security;
			}
		}
		else if(authentication == 2)
		{
			if(tr69SetUnfreshLeafDataInt("Device.WiFi.AccessPoint", tr69_id, "Security.X_ACTIONTEC_COM_RadiusEnabled", 0))
			{
				sprintf(tmpBuf, ("Set Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_RadiusEnabled error!"), tr69_id);
				goto setErr_security;
			}
		}
	}
#endif	
	return 1;
			
setErr_security:
	return 0;
}

void actWlsecurity(request *wp, char *path, char *query)
{
	char *strVal = NULL;
	char *action = NULL;
	int wl_freq = 0;	
	char ssid[500] = {0};
	
	both_freq = 0;	
	wlan_idx = 0;
	
	strVal = req_get_cstream_var(wp, ("wl_freq"), "");
	if(strVal[0])
	{
		kinfo("wl_freq: %s", strVal);
		wl_freq = atoi(strVal);

		if(wl_freq == 10)
			wlan_idx = 1;
		else if(wl_freq == 11)
			wlan_idx = 0;
		else if(wl_freq == 0)
			both_freq = 1;
	}

	strVal = req_get_cstream_var(wp, ("wl_mssididx"), "");
	if(strVal[0])
	{
		kinfo("wl_mssididx: %s", strVal);
		vwlan_idx = atoi(strVal);
		curr_ssid = vwlan_idx;
	}

	action = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", action);
	
	if(!safe_strcmp(action, "apply"))
	{
		if(!applySecurity(wp, error_buf))
			goto setErr_security;

#ifdef ACTIONTEC_TR69
		if(tr69SaveNow())
        {
            strcpy(error_buf, "tr69SaveNow error!");
            goto setErr_security;
        }      
#endif
	}
	else if(!safe_strcmp(action, "getvalue"))
	{
		if(!getvalueBasic(wp, error_buf, both_freq, 1))
			goto setErr_security;

		return;
	}

	applyPage(wp, INIT_ALL);
	return;
	
setErr_security:
	kerror("error_buf: %s", error_buf);
	backPage(wp);	
}

void actWlssid(request *wp, char *path, char *query)
{
	char *strVal = NULL;
	char *action = NULL;
	int wl_freq = 0;
	int ssid1 = 0;
	int ssid2 = 0;
	int ssid3 = 0;
	int ssid4 = 0;
	char ssid1name[1024] = {0};
	char ssid2name[1024] = {0};
	char ssid3name[1024] = {0};
	char ssid4name[1024] = {0};	

	both_freq = 0;
	wlan_idx = 0;
	
	strVal = req_get_cstream_var(wp, ("wl_freq"), "");
	if(strVal[0])
	{
		kinfo("wl_freq: %s", strVal);
		wl_freq = atoi(strVal);

		if(wl_freq == 10)
			wlan_idx = 1;
		else if(wl_freq == 11)
			wlan_idx = 0;
		else if(wl_freq == 0)
			both_freq = 1;
	}

	action = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", action);
	
	if(!safe_strcmp(action, "apply"))
	{
		vwlan_idx = 1;
		strVal = req_get_cstream_var(wp, ("wl_ssid1"), "");
		if(strVal[0])
		{
			kinfo("wl_ssid1: %s", strVal);		
			
			if(!setWlanDisabled(error_buf, strVal, 0))
				goto setErr_ssid;
		}

		strVal = req_get_cstream_var(wp, ("wl_ssid1name"), "");
		if(strVal[0])
		{
			kinfo("wl_ssid1name: %s", strVal);		

			/*
			if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! SSID1 Network Name is invalid."));
				goto setErr_ssid;
			}
			*/
			
			if(!setSSID(error_buf, strVal))
				goto setErr_ssid;
		}

		vwlan_idx = 2;
		strVal = req_get_cstream_var(wp, ("wl_ssid2"), "");
		if(strVal[0])
		{
			kinfo("wl_ssid2: %s", strVal);	
			
			if(!setWlanDisabled(error_buf, strVal, 0))
				goto setErr_ssid;
		}

		strVal = req_get_cstream_var(wp, ("wl_ssid2name"), "");
		if(strVal[0])
		{
			kinfo("wl_ssid2name: %s", strVal);	

			/*
			if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! SSID2 Network Name is invalid."));
				goto setErr_ssid;
			}
			*/
			
			if(!setSSID(error_buf, strVal))
				goto setErr_ssid;
		}

		vwlan_idx = 3;
		strVal = req_get_cstream_var(wp, ("wl_ssid3"), "");
		if(strVal[0])
		{
			kinfo("wl_ssid3: %s", strVal);		
			
			if(!setWlanDisabled(error_buf, strVal, 0))
				goto setErr_ssid;
		}

		strVal = req_get_cstream_var(wp, ("wl_ssid3name"), "");
		if(strVal[0])
		{
			kinfo("wl_ssid3name: %s", strVal);	

			/*
			if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! SSID3 Network Name is invalid."));
				goto setErr_ssid;
			}
			*/
			
			if(!setSSID(error_buf, strVal))
				goto setErr_ssid;
		}

		vwlan_idx = 4;
		strVal = req_get_cstream_var(wp, ("wl_ssid4"), "");
		if(strVal[0])
		{
			kinfo("wl_ssid4: %s", strVal);	
			
			if(!setWlanDisabled(error_buf, strVal, 0))
				goto setErr_ssid;
		}		

		strVal = req_get_cstream_var(wp, ("wl_ssid4name"), "");
		if(strVal[0])
		{
			kinfo("wl_ssid4name: %s", strVal);	

			/*
			if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! SSID4 Network Name is invalid."));
				goto setErr_ssid;
			}
			*/
			
			if(!setSSID(error_buf, strVal))
				goto setErr_ssid;
		}
	}
	else if(!safe_strcmp(action, "getvalue"))
	{
		vwlan_idx = 1;
		if (!apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&ssid1)) {				
			strcpy(error_buf, ("Get MIB_WLAN_WLAN_DISABLED 1 error!"));
			goto setErr_ssid;
		}
		
		if (!apmib_get(MIB_WLAN_SSID, (void *)ssid1name)) {				
			strcpy(error_buf, ("Get MIB_WLAN_SSID 1 error!"));
			goto setErr_ssid;
		}
		
		vwlan_idx = 2;
		if (!apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&ssid2)) {				
			strcpy(error_buf, ("Get MIB_WLAN_WLAN_DISABLED 2 error!"));
			goto setErr_ssid;
		}

		if (!apmib_get(MIB_WLAN_SSID, (void *)ssid2name)) {				
			strcpy(error_buf, ("Get MIB_WLAN_SSID 2 error!"));
			goto setErr_ssid;
		}
		
		vwlan_idx = 3;
		if (!apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&ssid3)) {				
			strcpy(error_buf, ("Get MIB_WLAN_WLAN_DISABLED 3 error!"));
			goto setErr_ssid;
		}

		if (!apmib_get(MIB_WLAN_SSID, (void *)ssid3name)) {				
			strcpy(error_buf, ("Get MIB_WLAN_SSID 3 error!"));
			goto setErr_ssid;
		}
		
		vwlan_idx = 4;
		if (!apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&ssid4)) {				
			strcpy(error_buf, ("Get MIB_WLAN_WLAN_DISABLED 4 error!"));
			goto setErr_ssid;
		}

		if (!apmib_get(MIB_WLAN_SSID, (void *)ssid4name)) {				
			strcpy(error_buf, ("Get MIB_WLAN_SSID 4 error!"));
			goto setErr_ssid;
		}

		speciCharEncode(ssid1name, sizeof(ssid1name));
		changeString(ssid1name);
		speciCharEncode(ssid2name, sizeof(ssid2name));
		changeString(ssid2name);
		speciCharEncode(ssid3name, sizeof(ssid3name));
		changeString(ssid3name);
		speciCharEncode(ssid4name, sizeof(ssid4name));
		changeString(ssid4name);		
#ifdef AEI_SSID_CONTROL_ENABLE
        int inhit_ssid1 = 0;
        int inhit_ssid2 = 0;
        int inhit_ssid3 = 0;
        int inhit_ssid4 = 0;
        int i = 1;
        for(i=1; i<5; i++)
        {
            char tmp[4] = {0};
            char ssid_object[256] = {0};
            if(wl_freq == 11)
                vwlan_idx = i+4;
            else
                vwlan_idx = i;
            snprintf(ssid_object,sizeof(ssid_object),"Device.WiFi.SSID.%d",vwlan_idx);
            if(tr69GetUnfreshLeafData(tmp, ssid_object, "X_ACTIONTEC_COM_InhibitSSIDControl"))
            {
                sprintf(error_buf, "Get %s.X_ACTIONTEC_COM_InhibitSSIDControl error!", ssid_object);
                goto setErr_ssid;
            }
            if(i == 1)
                inhit_ssid1 = atoi(tmp);
            else if(i == 2)
                inhit_ssid2 = atoi(tmp);
            else if(i == 3)
                inhit_ssid3 = atoi(tmp);
            else if(i == 4)
                inhit_ssid4 = atoi(tmp);
        }
        req_format_write(wp, ("%d|%d|%d|%d|%s|%s|%s|%s|%d|%d|%d|%d||"), 
            ssid1, ssid2, ssid3, ssid4, ssid1name, ssid2name, ssid3name, ssid4name, inhit_ssid1, inhit_ssid2, inhit_ssid3, inhit_ssid4);
#else
		req_format_write(wp, ("%d|%d|%d|%d|%s|%s|%s|%s||"), 
				ssid1, ssid2, ssid3, ssid4, ssid1name, ssid2name, ssid3name, ssid4name);	
#endif	
		return;
	}

#ifdef ACTIONTEC_TR69
	if(tr69SaveNow())
    {
        strcpy(error_buf, "tr69SaveNow error!");
        goto setErr_ssid;
    }   
#endif

	applyPage(wp, INIT_ALL);
	return;
		
setErr_ssid:
	kerror("error_buf: %s", error_buf);
	backPage(wp);	
}

void actWlwfps(request *wp, char *path, char *query)
{
	char *strVal = NULL;
	char *action = NULL;
	char *token = NULL;
	int wl_freq = 0;
	int disabled = 0;		
	char pincodestr_b[20] = {0};
	char wps_buf[100] = {0};
	char wsc_pin[10] = {0};
	char init_appin_buf[BUF_LEN100] = {0};
	int idx = 0, idx2 = 0;	
	int wsc_configured = 0;
	struct timeval tod;
	unsigned long num = 0;
	char tmp1[100] = {0};
	int current_wlanidx = 0;
	int other_disabled = 0;
	
	both_freq = 0;
	wlan_idx = 0;
	vwlan_idx = 1;	
	
	strVal = req_get_cstream_var(wp, ("wl_freq"), "");
	if(strVal[0])
	{
		kinfo("wl_freq: %s", strVal);
		wl_freq = atoi(strVal);

		if(wl_freq == 10)
			wlan_idx = 1;
		else if(wl_freq == 11)
			wlan_idx = 0;
		else if(wl_freq == 0)
			both_freq = 1;
	}
	
	action = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", action);
	
	if(!safe_strcmp(action, "apply"))
	{
		strVal = req_get_cstream_var(wp, ("wl_wpsen"), "");
		if(strVal[0])
		{
			kinfo("wl_wpsen: %s", strVal);
			disabled = atoi(strVal);

			current_wlanidx = wlan_idx;
			if(disabled == 1)
			{
				if(current_wlanidx == 1)
				{
					wlan_idx = 0;	
				}
				else if(current_wlanidx == 0)
				{
					wlan_idx = 1;
				}
				
				if (!apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&other_disabled)) {
			 		strcpy(error_buf, ("Get MIB_WLAN_WSC_DISABLE error!"));
					goto setErr_wfps;
				}

				if(other_disabled == 1)
				{
					system("echo 2 >  /proc/act_config/WPSRedLED");
    				system("echo 2 >  /proc/act_config/WPSGreenLED");
				}
			}
			else
			{
				system("echo 1 >  /proc/act_config/WPSGreenLED");
			}

			wlan_idx = current_wlanidx;
			if (!apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&disabled)) {
		 		strcpy(error_buf, ("Set MIB_WLAN_WSC_DISABLE error!"));
				goto setErr_wfps;
			}
			
			updateVapWscDisable(wlan_idx, disabled);
		}

		strVal = req_get_cstream_var(wp, ("wl_appin"), "");
		if(strVal[0])
		{
			kinfo("wl_appin: %s", strVal);	
			if(strlen(strVal) > 9)
			{
				strcpy(error_buf, ("Invalid AP PIN length! The device PIN is usally four or eight digits long"));
				goto setErr_wfps;
			}

			if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! AP PIN is invalid."));
				goto setErr_wfps;
			}

			memset(wsc_pin, 0, 10);
			token = strtok(strVal, "- ");
			while(token != NULL)
			{
				strcat(wsc_pin, token);
				token = strtok(NULL, "- ");
			}
			
			if (!apmib_set(MIB_HW_WSC_PIN, (void *)wsc_pin)) {
		 		strcpy(error_buf, ("Set MIB_HW_WSC_PIN error!"));
				goto setErr_wfps;
			}
		}

		strVal = req_get_cstream_var(wp, ("wl_apdevice"), "");
		if(strVal[0])
		{
			kinfo("wl_apdevice: %s", strVal);

			if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! AP Device Name is invalid."));
				goto setErr_wfps;
			}
			
			if (!apmib_set(MIB_DEVICE_NAME, (void *)strVal)) {
		 		strcpy(error_buf, ("Set MIB_DEVICE_NAME error!"));
				goto setErr_wfps;
			}			
		}

		strVal = req_get_cstream_var(wp, ("wl_wpsconfig"), "");
		if(strVal[0])
		{
			kinfo("wl_wpsconfig: %s", strVal);
			wsc_configured = atoi(strVal);
			
			if (!apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&wsc_configured)) {
		 		strcpy(error_buf, ("Set MIB_WLAN_WSC_CONFIGURED error!"));
				goto setErr_wfps;
			}			
		}		
	}
	else if(!safe_strcmp(action, "resetunconfigured"))
	{		
		if(wlan_idx == 1)
			apmib_reset_wlan_to_default((unsigned char *)"wlan1-va0");
		else if(wlan_idx == 0)
			apmib_reset_wlan_to_default((unsigned char *)"wlan0-va0");

		strVal = req_get_cstream_var(wp, ("wl_wpsen"), "");
		if(strVal[0])
		{
			kinfo("wl_wpsen: %s", strVal);
			disabled = atoi(strVal);
			
			if (!apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&disabled)) {
		 		strcpy(error_buf, ("Set MIB_WLAN_WSC_DISABLE error!"));
				goto setErr_wfps;
			}
			
			updateVapWscDisable(wlan_idx, disabled);
		}

		wsc_configured = 0;
		if (!apmib_set(MIB_WLAN_WSC_CONFIGURED, (void *)&wsc_configured)) {
		 	strcpy(error_buf, ("Set MIB_WLAN_WSC_CONFIGURED error!"));
			goto setErr_wfps;
		}

		applyPage(wp, INIT_BRIDGE);	
		return;
	}
	else if(!safe_strcmp(action, "startpin"))
	{
		strVal = req_get_cstream_var(wp, ("wl_enrpin"), "");
		if(strVal[0])
		{
			kinfo("wl_enrpin: %s", strVal);
			if(strlen(strVal) > 9)
			{
				strcpy(error_buf, ("Invalid Enrollee's PIN length! The device PIN is usally four or eight digits long"));
				goto setErr_wfps;
			}

			if(strchr(strVal, FORM_INPUT_SPECIAL))
			{
				strcpy(error_buf, ("Error! Enrollee's PIN is invalid."));
				goto setErr_wfps;
			}

			memset(wsc_pin, 0, 10);
			token = strtok(strVal, "- ");
			while(token != NULL)
			{
				strcat(wsc_pin, token);
				token = strtok(NULL, "- ");
			}
			
			apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&disabled);
			if (disabled) {
				disabled = 0;
				if (!apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&disabled)) {
			 		strcpy(error_buf, ("Set MIB_WLAN_WSC_DISABLE error!"));
					goto setErr_wfps;
				}
				
				updateVapWscDisable(wlan_idx, disabled);
				apmib_update_web(CURRENT_SETTING);	

				sprintf(wps_buf, "echo %s > /var/wps_peer_pin", wsc_pin);
				system(wps_buf);

#ifndef NO_ACTION
				run_init_script("bridge");
#endif					
			}
			else {	
				
#ifndef NO_ACTION				
				for(idx=0; idx < strlen(wsc_pin); idx++){					
					if(wsc_pin[idx] >= '0' && wsc_pin[idx] <= '9'){
						pincodestr_b[idx2] = wsc_pin[idx];	
						idx2++;
					}
				}
							
				sprintf(wps_buf, "iwpriv wlan%d-va0 set_mib pin=%s", wlan_idx, pincodestr_b);	
				kinfo("%s", wps_buf);
				system(wps_buf);							
#endif
			}
		}

		thankyouPage(wp);
		return;
	}
	else if(!safe_strcmp(action, "startpbc"))
	{	
		// 
		kinfo("current: %s", wlan_idx==1?"2.4G":"5G");
		apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&disabled);
	
		current_wlanidx = wlan_idx;
		wlan_idx = (current_wlanidx==1?0:1);
		
		kinfo("other: %s", wlan_idx==1?"2.4G":"5G");
		apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&other_disabled);

		if (disabled) {
			if (other_disabled) {	// Both disabled
				kinfo("both disabled");

				disabled = 0;				
				wlan_idx = current_wlanidx;
				if (!apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&disabled)) {
					strcpy(error_buf, ("Set MIB_WLAN_WSC_DISABLE error!"));
					goto setErr_wfps;
				}
						
				updateVapWscDisable(wlan_idx, disabled);
				apmib_update_web(CURRENT_SETTING);	// update to flash	
					
				system("echo 1 > /var/wps_start_pbc");
						
#ifndef NO_ACTION
				kinfo("run_init_script('bridge')");
				run_init_script("bridge");
#endif			
			} else { // One of 2.4G, 5G enabled
	
				kinfo("%s disabled, %s enabled", current_wlanidx==1?"2.4G":"5G", wlan_idx==1?"2.4G":"5G");
				sprintf(wps_buf, "%s -sig_pbc wlan0-va0-wlan1-va0", _WSC_DAEMON_PROG);

#ifndef NO_ACTION				
				kinfo("%s", wps_buf);
				system(wps_buf);
#endif			
				
			}
		} else {
			if (other_disabled) {
				kinfo("%s enabled, %s disabled", current_wlanidx==1?"2.4G":"5G", wlan_idx==1?"2.4G":"5G");

				if (current_wlanidx == 1) { // 2.4 G WPS enabled
					sprintf(wps_buf, "%s -sig_pbc wlan1-va0", _WSC_DAEMON_PROG);
				} else {	// 5 G WPS enabled
					sprintf(wps_buf, "%s -sig_pbc wlan0-va0", _WSC_DAEMON_PROG);
				}
			} else {	// Both enabled
				kinfo("both enabled");
				sprintf(wps_buf, "%s -sig_pbc wlan0-va0-wlan1-va0", _WSC_DAEMON_PROG);
			}
			
#ifndef NO_ACTION
			kinfo("%s", wps_buf);
			system(wps_buf);
#endif			
	
		}
		

//	Comments by Tom  2012-10-17
/*
		apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&disabled);
		if (disabled) {
			disabled = 0;
			if (!apmib_set(MIB_WLAN_WSC_DISABLE, (void *)&disabled)) {
				strcpy(error_buf, ("Set MIB_WLAN_WSC_DISABLE error!"));
				goto setErr_wfps;
			}
			
			updateVapWscDisable(wlan_idx, disabled);
			apmib_update_web(CURRENT_SETTING);	// update to flash	
			
			system("echo 1 > /var/wps_start_pbc");
			
#ifndef NO_ACTION
			run_init_script("bridge");
#endif			
		}
		else {		
#ifndef NO_ACTION	
			sprintf(wps_buf, "%s -sig_pbc wlan0-va0-wlan1-va0", _WSC_DAEMON_PROG);
			kinfo("%s", wps_buf);
			system(wps_buf);
#endif
		}	
*/
		thankyouPage(wp);
		return;

	}
	else if(!safe_strcmp(action, "getvalue"))
	{
		if (!apmib_get(MIB_WLAN_WSC_DISABLE, (void *)&disabled)) {
			strcpy(error_buf, ("Get MIB_WLAN_WSC_DISABLE error!"));
			goto setErr_wfps;
		}

		memset(wsc_pin, 0, 10);
		if (!apmib_get(MIB_HW_WSC_PIN, (void *)wsc_pin)) {
			strcpy(error_buf, ("Get MIB_HW_WSC_PIN error!"));
			goto setErr_wfps;
		}

		if (!apmib_get(MIB_WLAN_WSC_CONFIGURED, (void *)&wsc_configured)) {
		 	strcpy(error_buf, ("Get MIB_WLAN_WSC_CONFIGURED error!"));
			goto setErr_wfps;
		}		

		changeString(wsc_pin);
		
		req_format_write(wp, ("%d|%d|%s||"), disabled, wsc_configured, wsc_pin);			
		return;
	}	
	else if(!safe_strcmp(action, "generatepin"))
	{			
		gettimeofday(&tod , NULL);

		apmib_get(MIB_HW_NIC0_ADDR, (void *)&tmp1);			
		tod.tv_sec += tmp1[4] + tmp1[5];		
		srand(tod.tv_sec);
		num = rand() % 10000000;
		num = num * 10 + compute_pin_checksum(num);
		convert_hex_to_ascii((unsigned long)num, wsc_pin);		

        char cur_if[32];
		SetWlan_idx("wlan0-va0");
		apmib_set(MIB_HW_WSC_PIN, (void *)wsc_pin);
		SetWlan_idx("wlan1-va0");
		apmib_set(MIB_HW_WSC_PIN, (void *)wsc_pin);
        snprintf(cur_if, sizeof(cur_if), "wlan%d-va0", wlan_idx);
		SetWlan_idx(cur_if);

#ifdef AEI_WECB_CUSTOMER_NCS
		send_redirect_perm(wp, "/wl_wfps.html");
#else
		changeString(wsc_pin);
		kinfo("Generated PIN = %s", wsc_pin);
		req_format_write(wp, ("%s"), wsc_pin);
#endif		
		return;
	}
#ifdef ACTIONTEC_WCB_TELUS
	else if(!safe_strcmp(action, "disable_appin")||!safe_strcmp(action, "enable_appin"))
	{
		if(!safe_strcmp(action, "disable_appin"))
		{
			disabled = 1;
		}
		else if(!safe_strcmp(action, "enable_appin"))
		{
			disabled = 0;
		}

		if (!apmib_set(MIB_PIN_DISABLE_CONFIG_BY_EXREG, (void *)&disabled)) {
			strcpy(error_buf, ("Set MIB_PIN_DISABLE_CONFIG_BY_EXREG error!"));
			goto setErr_wfps;
		}

		memset(init_appin_buf,0,sizeof(init_appin_buf));
		snprintf(init_appin_buf,sizeof(init_appin_buf),"%s","sysconf init gw all");
		system(init_appin_buf);
	}
#endif
	apmib_update(HW_SETTING);
	applyPage(wp, INIT_ALL);
	return;
		
setErr_wfps:
	kerror("error_buf: %s", error_buf);
	backPage(wp);		
}

void actWlgetPinStatus(request *wp, char *path, char *query)
{
#ifdef ACTIONTEC_WCB_TELUS
	char buf[BUF_LEN100]={0};
	int  getVal = 0;

	if (!apmib_get(MIB_PIN_DISABLE_CONFIG_BY_EXREG, (void *)&getVal)) {
		strcpy(error_buf, ("Get MIB_PIN_DISABLE_CONFIG_BY_EXREG failed!"));
		goto getErr_wfps;
	}	
	snprintf(buf,sizeof(buf),"%d",getVal);
	req_format_write(wp, ("%s"), buf);
	return;

getErr_wfps:
	kerror("error_buf: %s", error_buf);
	backPage(wp);
#endif
}

void actWladv(request *wp, char *path, char *query)
{
	char *strVal = NULL;
	char *action = NULL;
	int wl_freq = 0;	
	int gi = 0;
	int beacon = 0;	
	int protection = 0;
	int block = 0;
	int stbc = 0;
	int aggregation = 0;
	int chipVersion = 0;

	both_freq = 0;
	wlan_idx = 0;	
#ifdef AEI_RADIO_CONTROL_ENABLE
  char buffer[32] = {0};
#endif	
	strVal = req_get_cstream_var(wp, ("wl_freq"), "");
	if(strVal[0])
	{
		kinfo("wl_freq: %s", strVal);
		wl_freq = atoi(strVal);

		if(wl_freq == 10)
			wlan_idx = 1;
		else if(wl_freq == 11)
			wlan_idx = 0;
		else if(wl_freq == 0)
			both_freq = 1;
	}

	strVal = req_get_cstream_var(wp, ("wl_mssididx"), "");
	if(strVal[0])
	{
		kinfo("wl_mssididx: %s", strVal);
		vwlan_idx = atoi(strVal);
		curr_ssid = vwlan_idx;
	}

#ifdef ACTIONTEC_TR69
	int tr69_id = 0; 
			
	if(wlan_idx == 1)
	{
		if(vwlan_idx > 0)			
		{
			tr69_id = vwlan_idx;
		}
	}
	else if(wlan_idx == 0)
	{
		if(vwlan_idx > 0)
		{
			tr69_id = 4 + vwlan_idx;
		}
	}	
#endif

	action = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", action);
	
	if(!safe_strcmp(action, "apply"))
	{
		strVal = req_get_cstream_var(wp, ("wl_protection"), "");
		if(strVal[0])
		{
			kinfo("wl_protection: %s", strVal);
			protection = atoi(strVal);

			if (!apmib_set(MIB_WLAN_PROTECTION_DISABLED, (void *)&protection)) {
				strcpy(error_buf, ("Set MIB_WLAN_PROTECTION_DISABLED failed!"));
				goto setErr_adv;
			}
		}
		
		strVal = req_get_cstream_var(wp, ("wl_stbc"), "");
		if(strVal[0])
		{
			kinfo("wl_stbc: %s", strVal);
			stbc = atoi(strVal);

			if(!apmib_set(MIB_WLAN_STBC_ENABLED, (void *)&stbc)) {
				strcpy(error_buf, ("Set MIB_WLAN_STBC_ENABLED failed!"));
				goto setErr_adv;
			}	
		}
		else
		{		
			chipVersion = getWLAN_ChipVersion();
			if(chipVersion == 1)
			{
				stbc = 0;	
				if(!apmib_set(MIB_WLAN_STBC_ENABLED, (void *)&stbc)) {
					strcpy(error_buf, ("Set MIB_WLAN_STBC_ENABLED failed!"));
					goto setErr_adv;
				}	
			}
		}

		strVal = req_get_cstream_var(wp, ("wl_aggregation"), "");
		if(strVal[0])
		{
			kinfo("wl_aggregation: %s", strVal);
			aggregation = atoi(strVal);

			if (!apmib_set(MIB_WLAN_AGGREGATION, (void *)&aggregation)) {
				strcpy(error_buf, ("Set MIB_WLAN_AGGREGATION failed!"));
				goto setErr_adv;
			}
		}
		
		        
        strVal = req_get_cstream_var(wp, ("wl_block"), "");
        if(strVal[0])
        {
            kinfo("wl_block: %s", strVal);
            block = atoi(strVal);
        
            if (!apmib_set(MIB_WLAN_BLOCK_RELAY, (void *)&block)) {
               strcpy(error_buf, ("Set MIB_WLAN_BLOCK_RELAY failed!"));
               goto setErr_adv;
            }
        }


		vwlan_idx = 0;
		strVal = req_get_cstream_var(wp, ("wl_gi"), "");
		if(strVal[0])
		{
			kinfo("wl_gi: %s", strVal);
			gi = atoi(strVal);
			
			if (!apmib_set(MIB_WLAN_SHORT_GI, (void *)&gi)) {
				strcpy(error_buf, ("Set MIB_WLAN_SHORT_GI failed!"));
				goto setErr_adv;
			}
#ifdef ACTIONTEC_TR69	
			if(wlan_idx == 1)
				tr69_id = 1;
			else if(wlan_idx == 0)
				tr69_id = 2;

			char guard_buf[10] = {0};
			if(gi == 1)
				strcpy(guard_buf, "400nsec");
			else if(gi == 0)
				strcpy(guard_buf, "800nsec");			

			if(tr69_id != 0)
			{
				if(tr69SetUnfreshLeafDataString("Device.WiFi.Radio", tr69_id, "GuardInterval", guard_buf))
				{
					sprintf(error_buf, ("Set Device.WiFi.Radio.%d.GuardInterval error!"), tr69_id);
					goto setErr_adv;
				}
			}			
#endif
		}	

		strVal = req_get_cstream_var(wp, ("wl_beacon"), "");
		if(strVal[0])
		{
			kinfo("wl_beacon: %s", strVal);			
			
			if (!string_to_dec(strVal, &beacon) || (beacon < 20) || (beacon > 1024)) {
				strcpy(error_buf, ("Error! Invalid value of Beacon Interval."));
				goto setErr_adv;
			}
			
			if (!apmib_set(MIB_WLAN_BEACON_INTERVAL, (void *)&beacon)) {
				strcpy(error_buf, ("Set Beacon interval failed!"));
				goto setErr_adv;
			}

#ifdef ACTIONTEC_TR69				
			if(tr69_id != 0)
			{
				if(tr69SetUnfreshLeafDataInt("Device.WiFi.Radio", tr69_id, "TransmitPower", beacon)) 
				{
					sprintf(error_buf, ("Set Device.WiFi.Radio.%d.TransmitPower error!"), tr69_id);
					goto setErr_adv;
				}
			}			
#endif
		}	
		vwlan_idx = curr_ssid;				
	}
	else if(!safe_strcmp(action, "getvalue"))
	{
		if (!apmib_get(MIB_WLAN_PROTECTION_DISABLED, (void *)&protection)) {
			strcpy(error_buf, ("Get MIB_WLAN_PROTECTION_DISABLED failed!"));
			goto setErr_adv;
		}		

		if(!apmib_get(MIB_WLAN_STBC_ENABLED, (void *)&stbc)) {
			strcpy(error_buf, ("Get MIB_WLAN_STBC_ENABLED failed!"));
			goto setErr_adv;
		}

		if (!apmib_get(MIB_WLAN_AGGREGATION, (void *)&aggregation)) {
			strcpy(error_buf, ("Get MIB_WLAN_AGGREGATION failed!"));
			goto setErr_adv;
		}		
	
		
        if (!apmib_get(MIB_WLAN_BLOCK_RELAY, (void *)&block)) {
			strcpy(error_buf, ("Get MIB_WLAN_BLOCK_RELAY failed!"));
			goto setErr_adv;
		}

		
		vwlan_idx = 0;        
		if (!apmib_get(MIB_WLAN_SHORT_GI, (void *)&gi)) {
			strcpy(error_buf, ("Get MIB_WLAN_SHORT_GI failed!"));
			goto setErr_adv;
		}

		if (!apmib_get(MIB_WLAN_BEACON_INTERVAL, (void *)&beacon)) {
			strcpy(error_buf, ("Get MIB_WLAN_BEACON_INTERVAL failed!"));
			goto setErr_adv;
		}
		vwlan_idx = curr_ssid;
#ifdef AEI_RADIO_CONTROL_ENABLE
    if(tr69GetUnfreshLeafData(buffer, "Device.UserInterface", "X_TWC_COM_InhibitRadioControl"))
    {
      sprintf(error_buf, "Get Device.UserInterface.X_TWC_COM_InhibitRadioControl error!");
      goto setErr_adv;
    }
		
		req_format_write(wp, ("%d|%d|%d|%d|%d|%d|%s||"), gi, beacon, protection, block, stbc, aggregation, buffer);
#else
    req_format_write(wp, ("%d|%d|%d|%d|%d|%d||"), gi, beacon, protection, block, stbc, aggregation);
#endif    
		return;
	}

#ifdef ACTIONTEC_TR69
    if(tr69SaveNow())
    {
        strcpy(error_buf, "tr69SaveNow error!");
        goto setErr_adv;
    }
#endif

	applyPage(wp, INIT_ALL);	
	return;
		
setErr_adv:
	kerror("error_buf: %s", error_buf);
	backPage(wp);	
}

int getlistNetwork(request *wp, char *tmpBuf, int wlan)
{
	int i = 0;
	int wlan_disabled = 0;
	char interface_name[12] = {0};
	char mac_address[24] = {0};
	char ssid_name[1024] = {0};
	char security_mode[32] = {0};
	ENCRYPT_T encrypt;
	int wep = 0;
	int wpaAuth = 0;
	int wpa_cipher = 0;
	int wpa2_cipher = 0;
	int band = 0;
	
	wlan_idx = wlan;
	for(i = 1; i <= 4; i++)
	{
		vwlan_idx = i;
#ifdef AEI_WECB_CUSTOMER_TWC
        if (i > 1)  continue;
#endif

#ifdef ACTIONTEC_WCB_TELUS
		if(vwlan_idx == 4)		
#else
#ifdef AEI_WECB_CUSTOMER_NCS
		if(vwlan_idx == 3 || vwlan_idx == 4)
#else			
		if(vwlan_idx == 2 || vwlan_idx == 4)
#endif			
#endif
			continue;

		if (!apmib_get(MIB_WLAN_WLAN_DISABLED, (void *)&wlan_disabled)) {				
			sprintf(tmpBuf, ("Get MIB_WLAN_WLAN_DISABLED %d failed!"), vwlan_idx);
			goto setErr_network;
		}

		if(!wlan_disabled)
		{
			if (!apmib_get(MIB_WLAN_SSID, (void *)ssid_name)) {				
				sprintf(tmpBuf, ("Get MIB_WLAN_SSID %d failed!"), vwlan_idx);
				goto setErr_network;
			}

			sprintf(interface_name, "wlan%d-va%d", wlan_idx, (vwlan_idx-1));
			getMacByIntf(interface_name, mac_address);

			if(!apmib_get(MIB_WLAN_ENCRYPT, (void *)&encrypt)) {
				sprintf(tmpBuf, ("Get MIB_WLAN_ENCRYPT %d failed!"), vwlan_idx);
				goto setErr_network;
			}			

			if(!apmib_get(MIB_WLAN_WPA_AUTH, (void *)&wpaAuth)) {
				sprintf(tmpBuf, ("Get MIB_WLAN_WPA_AUTH %d failed!"), vwlan_idx);
				goto setErr_network;
			}

			if(!apmib_get(MIB_WLAN_WPA_CIPHER_SUITE, (void *)&wpa_cipher)) {
				sprintf(tmpBuf, ("Get MIB_WLAN_WPA_CIPHER_SUITE %d failed!"), vwlan_idx);
				goto setErr_network;							
			}

			if(!apmib_get(MIB_WLAN_WPA2_CIPHER_SUITE, (void *)&wpa2_cipher)) {
				sprintf(tmpBuf, ("Get MIB_WLAN_WPA2_CIPHER_SUITE %d failed!"), vwlan_idx);
				goto setErr_network;							
			}		

			switch(encrypt)
			{
				case ENCRYPT_DISABLED:
					strcpy(security_mode, "Open");
					break;
				case ENCRYPT_WEP:
					if(!apmib_get(MIB_WLAN_WEP, (void *)&wep)) {
						sprintf(tmpBuf, ("Get MIB_WLAN_WEP %d failed!"), vwlan_idx);
						goto setErr_network;
					}

					if(wep == WEP64)
						strcpy(security_mode, "WEP 64");
					else if(wep == WEP128)
						strcpy(security_mode, "WEP 128");
					break;
				case ENCRYPT_WPA:

					if(wpa_cipher == WPA_CIPHER_TKIP)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA-Personal(TKIP)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA-Enterprise(TKIP)");
					}
					else if(wpa_cipher == WPA_CIPHER_AES)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA-Personal(AES)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA-Enterprise(AES)");
					}
					else if(wpa_cipher == WPA_CIPHER_MIXED)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA-Personal(TKIP/AES)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA-Enterprise(TKIP/AES)");
					}
						
					break;
				case ENCRYPT_WPA2:

					if(wpa2_cipher == WPA_CIPHER_TKIP)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA2-Personal(TKIP)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA2-Enterprise(TKIP)");
					}
					else if(wpa2_cipher == WPA_CIPHER_AES)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA2-Personal(AES)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA2-Enterprise(AES)");
					}
					else if(wpa2_cipher == WPA_CIPHER_MIXED)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA2-Personal(TKIP/AES)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA2-Enterprise(TKIP/AES)");
					}
					
					break;
				case ENCRYPT_WPA2_MIXED:
					if(wpa2_cipher == WPA_CIPHER_TKIP)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA/WPA2-Personal(TKIP)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA/WPA2-Enterprise(TKIP)");
					}
					else if(wpa2_cipher == WPA_CIPHER_AES)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA/WPA2-Personal(AES)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA/WPA2-Enterprise(AES)");
					}
					else if(wpa2_cipher == WPA_CIPHER_MIXED)
					{
						if(wpaAuth == WPA_AUTH_PSK)
							strcpy(security_mode, "WPA/WPA2-Personal(TKIP/AES)");
						else if(wpaAuth == WPA_AUTH_AUTO)
							strcpy(security_mode, "WPA/WPA2-Enterprise(TKIP/AES)");
					}
					
					break;
				default:
					break;
			}
			
			if (!apmib_get(MIB_WLAN_BAND, (void *)&band)) {				
				strcpy(tmpBuf, ("Get MIB_WLAN_BAND error!"));
				goto setErr_network;
			}
			
			speciCharEncode(ssid_name, sizeof(ssid_name));
			changeString(ssid_name);	
			changeString(mac_address);
			changeString(security_mode);

			req_format_write(wp, ("%d|%d|%d|%s|%s|%s||"), wlan_idx, vwlan_idx, (band - 1), ssid_name, mac_address, security_mode);
		}
	}
	return 1;
	
setErr_network:
	return 0;	
}

void actWlnetwork(request *wp, char *path, char *query)
{
	char *action = NULL;	
	
	kinfo("Enter");

	action = req_get_cstream_var(wp, ("command"), "");
	kinfo("command: %s", action);
	
	if(!safe_strcmp(action, "getvalue"))
	{		
		if(!getlistNetwork(wp, error_buf, 1))
			goto setErr_network;

		if(!getlistNetwork(wp, error_buf, 0))
			goto setErr_network;		
					
		return;
	}	

setErr_network:
	kerror("error_buf: %s", error_buf);
	backPage(wp);
}


