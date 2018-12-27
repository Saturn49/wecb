/*
 *      Access Function Implementation
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#include "act_access.h"

static url_perm_t perm_hashtable[NAME_HASH_PRIME + 1];

//support HASH routines
static void make_hash(const char *key, unsigned int *start, unsigned int *decrement, const int hash_prime)
{
  unsigned long int hash_num = key[0];
  int len = strlen(key);
  int i;

  /* Maybe i should have uses a "proper" hashing algorithm here instead
   * of making one up myself, seems to be working ok though. */
  for (i = 1; i < len; i++) {
    /* shifts the ascii based value and adds it to previous value
     * shift amount is mod 24 because long int is 32 bit and data
     * to be shifted is 8, dont want to shift data to where it has
     * no effect*/
    hash_num += ((key[i] + key[i-1]) << ((key[i] * i) % 24)); 
  }
  *start = (unsigned int) hash_num % hash_prime;
  *decrement = (unsigned int) 1 + (hash_num % (hash_prime - 1));
}

short get_perm(const char *req_url, int auth_level)
{
  int i;
  int loop = 0;
  unsigned int probe_address = 0;  
  unsigned int probe_decrement = 0;
  
  if (req_url == NULL)
    return 1;

  make_hash(req_url, &probe_address, &probe_decrement, NAME_HASH_PRIME);
  
  while ((perm_hashtable[probe_address].url != NULL) && 
         (loop < NAME_HASH_PRIME) ) {
    loop++;

    if (strstr(req_url, perm_hashtable[probe_address].url) 
			&& (perm_hashtable[probe_address].perm == auth_level)) {
    	return 0;
    } else {
      probe_address -= probe_decrement;

      if ((int)probe_address < 0) {
		probe_address += NAME_HASH_PRIME;
      }
    }
  }
  return 1;
}


void insert_perm(const char *req_url, short perm)
{
  static int inited = 0;
  int i;
  int loop = 0;
  unsigned int probe_address = 0; 
  unsigned int probe_decrement = 0;

  if (req_url == NULL)
    return -1;
  
  //init hash table: only once
  if (inited == 0) {
    for (i = 0; i <= NAME_HASH_PRIME; i++) {
		perm_hashtable[i].url = NULL;
		perm_hashtable[i].perm = 0;
    }
    inited = 1;
  }
  
  make_hash(req_url, &probe_address, &probe_decrement, NAME_HASH_PRIME);
  
  while ((perm_hashtable[probe_address].url != NULL) && 
         (loop < NAME_HASH_PRIME) ) {
    loop++;    
    probe_address -= probe_decrement;

    if ((int)probe_address < 0) {
	   probe_address += NAME_HASH_PRIME;
    }    
  }

  perm_hashtable[probe_address].url = strdup(req_url);
  perm_hashtable[probe_address].perm = perm;

  return;
}


void load_perm_table(void)
{
    char line_str[256] = {0};
    char html_name[128] = {0};  
	char perms[128] = {0};
    short perm = 0;
	char *token = NULL;
	int token_index = 0;
	
    FILE* fs = fopen("/var/web/wcb_access.txt", "r");
	
    if(fs != NULL)
    {
        while(fgets(line_str, sizeof(line_str), fs))
        {
        	token_index = 0;
			token = strtok(line_str, ":");
			while(token != NULL)
			{
				token_index++;

				if(token_index == 1)
				{
					sprintf(html_name, "/%s.html", token);
				}
				else if(token_index == 2)
				{
					if(strlen(token) >= 128)
						kerror("perm's list is too long");
					
					strncpy(perms, token, 127);
				}
				token = strtok(NULL, ":");
			}	

			token = strtok(perms, ",");
			while(token != NULL)
			{
				perm = atoi(token);
				
				kdebug("html_name: %s, perm: %d", html_name, perm);	    			
	       	 	insert_perm(html_name, perm);
				
				token = strtok(NULL, ",");
			}			
        }	
        fclose(fs);
    }
}

