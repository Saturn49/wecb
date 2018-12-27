/*
 *      Common Function Implementation
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_common.h"

float gui_distime(struct timeval tv1, struct timeval tv2)
{
	   unsigned int sec1 = tv1.tv_sec;
	   unsigned int usec1 = tv1.tv_usec;
	   unsigned int sec2 = tv2.tv_sec;
	   unsigned int usec2 = tv2.tv_usec;
	   
	   float f_usec1 = (usec1 * 1.0) / 1000000.0;
	   float f_usec2 = (usec2 * 1.0) / 1000000.0;
	   
	   float dis = (float) (sec2 - sec1);
	   dis += f_usec2 - f_usec1;
	   
	   return dis;
}

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
  int j;
  if (!string)
    return;
  
  j = strlen(string);      
  for (i=0;i<j; i++) {
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
    char t[4096]={0};
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
#if 0
	if(!safe_strcmp(buf, ""))
	{
		strncpy(buf, "NULL", 4);
	}	
	else if(strchr(buf, FORM_INPUT_SPECIAL))
	{
		kerror("buf include '|': %s", buf);
		strncpy(buf, "NULL", 4);
	}
#endif
}



