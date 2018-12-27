/*
 * This daemon use for getting the network time at DUT first used.
 * --Lewis
 */

#include<stdlib.h>
#include<stdio.h>

#define FIRSTUSE	"/mnt/rt_conf/firstuse"

struct ntp_info{
	char *name;
	char *server;
};

struct ntp_info ntp_servers[] = {
	{"north-america", "0.north-america.pool.ntp.org"},
	{"tummy", "ntp1.tummy.com"},
	{"nist", "time.nist.gov"},
	{"action", "ntp.actiontec.com"},
	{0, 0}
}; 


int get_time(int server_index, const int server_num)
{
	char cmd[128];

	snprintf(cmd, sizeof(cmd), "/sbin/ntpclient -s -l -i 2 -c 2 -h %s > /dev/null 2>&1", ntp_servers[server_index].server);	
	//snprintf(cmd, sizeof(cmd), "echo %s", ntp_servers[server_index].server);	
	system(cmd);

	if(server_index >= server_num)
		return 0;
	else
		return ++server_index;
}

int main(int argc, char **argv)
{
	FILE *file;
	int server_index = 0;

	while(1){
		file = fopen(FIRSTUSE, "r");
		if(file != NULL){
			fprintf(stderr, "%s: have got firstuse time\n", __FILE__, FIRSTUSE);
			fclose(file);
			system("killall ntpclient > /dev/null 2>&1");
			return 0;
		}else{
			server_index = get_time(server_index, sizeof(ntp_servers)/sizeof(struct ntp_info)-2);
		}
		sleep(8);
	}
}
