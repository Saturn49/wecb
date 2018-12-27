#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "parse_file.h"

int main()
{
  char cmd[256]={""};
  struct stat sb;
  int ret=0;

  printf("Auto Config Wifi Start......\n\n");

  sleep(1);


  for(;;){
    snprintf(cmd,sizeof(cmd),"%s", "/usr/bin/scp "
                                "-i /etc/dropbear/dropbear_wecb_rsa "
                                "vosky@172.16.10.188:/home/vosky/test.cap /tmp/");

    printf("[sharko] LINE:%d,  cmdline=%s\n", __LINE__, cmd);
    ret = system(cmd);
    if(ret == -1){
      sleep(1);
      continue;
    }

    sleep(5);

    if (stat("/tmp/test.cap", &sb) == -1) {
      printf("stat error, wifi config file does not exist.\n");
    }else{
      printf("Parse the wifi config file now......\n");
      parse_config();
      sleep(60);
    }
  }

  return 0;
}
