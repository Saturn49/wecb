/*
 *      Common Function Implementation
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_common.h"

void gui_trim(char *s)
{
	 int len = safe_strlen(s);
	 int lws;
 
	 while(len && isspace(s[len - 1]))
		 --len;
 
	 if(len)
	 {
		 lws = strspn(s, " \n\r\t\v");
		 if(lws)
		 {
			 len -= lws;
			 memmove(s, s + lws, len);
		 }
	 }
	 s[len] = '\0';
}
 
int safe_strcmp(const char *s1, const char *s2)
{
	 char emptyStr = '\0';
	 char *str1 = (char *) s1;
	 char *str2 = (char *) s2;
 
	 if(str1 == NULL)
		 str1 = &emptyStr;
	 if(str2 == NULL)
		 str2 = &emptyStr;
 
	 return strcmp(str1, str2);
}
 
int safe_strlen(const char *s1)
{
	 char emptyStr = '\0';
	 char *str1 = (char *) s1;
 
	 if(str1 == NULL)
		 str1 = &emptyStr;
 
	 return strlen(str1);
}

char *safe_strstr(const char *s1, const char *s2) 
{
   char emptyStr = '\0';
   char *str1 = (char *)s1;
   char *str2 = (char *)s2;

   if (str1 == NULL)
   {
      str1 = &emptyStr;
   }
   if (str2 == NULL)
   {
      str2 = &emptyStr;
   }

   return strstr(str1, str2);
}

void tolower_string (char *string)
{
	int i;

	if (!string)
		return;

	for (i=0;i<strlen (string); i++) {
		string[i] = tolower (string[i]);
	}

	return;
}

int agets(FILE *f, char *buff)
{
  int i;
  int aa;

  i=0;
  for(;;)
  {
      aa=fgetc(f);
      if (aa==EOF)
	  {
	  	if (i==0)
	    	return -1;
	  	break;
	  }
	  
      if (aa==0x0a)
		break;

	  buff[i++]=aa;
      if (i>400)
		break;
  }
  buff[i]=0;
  return i;
}

void convert_bin_to_str(unsigned char *bin, int len, char *out)
{
	int i;
	char tmpbuf[10];

	out[0] = '\0';

	for (i=0; i<len; i++) {
		sprintf(tmpbuf, "%02x", bin[i]);
		strcat(out, tmpbuf);
	}
}

/* Encode the output value base on microsoft standard*/
char* speciCharEncode(char *s, int len)
{
    int c;
    int n = 0;
    char t[4097] = {0};
    char *p;
	
    p = s;
    memset(t, 0, sizeof(t));
	
    if (s == NULL) {
        kerror("The transfer object is null");
        return s;
    }
	
    while ((c = *p++)&& n < 4096) {        
        if (!strchr("<>\"'%;)(&+|,/\\", c)) {
            t[n++] = c;
        } else if (n < 4096-5) {
            t[n++] = '&';
            t[n++] = '#';

            if(strchr("|", c)){
                t[n++] = (c/100) + '0';
                t[n++] = ((c%100)/10) + '0';
                t[n++] = ((c%100)%10) + '0';
            } else {
                t[n++] = (c/10) + '0';
                t[n++] = (c%10) + '0';
            }
            t[n++] = ';';
        } else {
            kerror("The Array size overflow Exception");
            return s;;
        }
    }
    t[n] = '\0';
    if (n <= len && n < 4096)
    {
        memset(s, 0, len);
        strncpy(s, t, n);
    }
    else
        kerror("The Array size overflow Exception");
	
    return s;
}

void changeString(char *buf)
{
	if(!safe_strcmp(buf, ""))
	{
		strncpy(buf, "NULL", 5);
	}	
	else if(strchr(buf, '|'))
	{
		kerror("buf include '|': %s", buf);
		strncpy(buf, "NULL", 5);
	}
}

void convert_hex_to_ascii(unsigned long code, char *out)
{
	*out++ = '0' + ((code / 10000000) % 10);  
	*out++ = '0' + ((code / 1000000) % 10);
	*out++ = '0' + ((code / 100000) % 10);
	*out++ = '0' + ((code / 10000) % 10);
	*out++ = '0' + ((code / 1000) % 10);
	*out++ = '0' + ((code / 100) % 10);
	*out++ = '0' + ((code / 10) % 10);
	*out++ = '0' + ((code / 1) % 10);
	*out = '\0';
}

int compute_pin_checksum(unsigned long int PIN)
{
	unsigned long int accum = 0;
	int digit;
	
	PIN *= 10;
	accum += 3 * ((PIN / 10000000) % 10); 	
	accum += 1 * ((PIN / 1000000) % 10);
	accum += 3 * ((PIN / 100000) % 10);
	accum += 1 * ((PIN / 10000) % 10); 
	accum += 3 * ((PIN / 1000) % 10); 
	accum += 1 * ((PIN / 100) % 10); 
	accum += 3 * ((PIN / 10) % 10);

	digit = (accum % 10);
	return (10 - digit) % 10;
}



