#include <stdio.h> //for snprintf
#include <stdlib.h> //for malloc
#include <string.h>
#include <assert.h>
#include "sha1.h"


#define SIZE_8                  8
#define SIZE_10                 10
#define SIZE_16                 16
#define SIZE_20                 20
#define SIZE_26                 26
#define SIZE_32                 32
#define SIZE_40             40
#define SIZE_48                 48
#define SIZE_64                 64
#define SIZE_80             80
#define SIZE_128                128


//C020
unsigned int Hex2String(unsigned char *hex, unsigned int hex_len, char *hexString)
{   //0x01 0xFF 0x02 ==> "01FF02"
    int i, j;
    unsigned char data[2];

    if(NULL==hex||0==hex_len||NULL==hexString) {
        return 0;
    }

    for(i=hex_len-1; i>=0; i--) {
        data[1] = hex[i]&0x0F;
        data[0] = hex[i] >> 4;
        for(j=0; j<2; j++) {
            if(data[j]<=9) {
                data[j] += 0x30;
            }
            else if(data[j]>=0x0a&&data[j]<=0x0f) {
                data[j] += 'a'-0x0a;
            } else {
                return 0;
            }
        }

        j = i<<1;
        hexString[j+1] = data[1];
        hexString[j+0] = data[0];
    }

    return (hex_len<<1);
}
/*
 * Password-Based Key Derivation Function 2 (PKCS #5 v2.0).
 * Code based on IEEE Std 802.11i-2004, Annex H.4.2.
 * H.4 Suggested pass-phrase-to-PSK mapping
 * H.4.2 Reference implementation
 * H.4.3 Test vectors
    Test case 1
    Passphrase = ¡°password¡±
    SSID = { ¡®I¡¯, ¡®E¡¯, ¡®E¡¯, ¡®E¡¯ }
    SSIDLength = 4
    PSK = f42c6fc52df0ebef9ebb4b90b38a5f90 2e83fe1b135a70e23aed762e9710a12e
    Test case 2
    Passphrase = ¡°ThisIsAPassword¡±
    SSID = { ¡®T¡¯, ¡®h¡¯, ¡®i¡¯, ¡®s¡¯, ¡®I¡¯, ¡®s¡¯, ¡®A¡¯, ¡®S¡¯, ¡®S¡¯, ¡®I¡¯, ¡®D¡¯ }
    SSIDLength = 11
    PSK = 0dc0d6eb90555ed6419756b9a15ec3e3 209b63df707dd508d14581f8982721af
    Test case 3
    Password = ¡°aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa¡±
    SSID = {¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯, ¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯, ¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯, ¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,
    ¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯, ¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯, ¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯,¡®Z¡¯}
    SSIDLength = 32
    PSK = becb93866bb8c3832cb777c2f559807c 8c59afcb6eae734885001300a981cc62
 */

////////////////////////////////////////////////////////////////////////////////////////
#define A_SHA_DIGEST_LEN SHA1_DIGEST_LENGTH
/*
* F(P, S, c, i) = U1 xor U2 xor ... Uc
* U1 = PRF(P, S || Int(i))
* U2 = PRF(P, U1)
* Uc = PRF(P, Uc-1)
*/
static void F(char *password, unsigned char *ssid, int ssidlength, int iterations, int count, unsigned char *output)
{
    unsigned char digest[36], digest1[A_SHA_DIGEST_LEN];
    int i, j;
    for (i = 0; i < strlen(password); i++) {
        assert((password[i] >= 32) && (password[i] <= 126));
    }
    /* U1 = PRF(P, S || int(i)) */
    memcpy(digest, ssid, ssidlength);
    digest[ssidlength] = (unsigned char)((count>>24) & 0xff);
    digest[ssidlength+1] = (unsigned char)((count>>16) & 0xff);
    digest[ssidlength+2] = (unsigned char)((count>>8) & 0xff);
    digest[ssidlength+3] = (unsigned char)(count & 0xff);
    //hmac_sha1(digest, ssidlength+4, (unsigned char*) password, (int) strlen(password), digest, digest1);
    hmac_sha1(digest, ssidlength+4, (unsigned char*) password, (int) strlen(password), digest1);
    /* output = U1 */
    memcpy(output, digest1, A_SHA_DIGEST_LEN);
    for (i = 1; i < iterations; i++) {
        /* Un = PRF(P, Un-1) */
        hmac_sha1(digest1, A_SHA_DIGEST_LEN, (unsigned char*) password, (int) strlen(password), digest);
        memcpy(digest1, digest, A_SHA_DIGEST_LEN);
        /* output = output xor Un */
        for (j = 0; j < A_SHA_DIGEST_LEN; j++) {
            output[j] ^= digest[j];
        }
    }
}

/*
* password - ascii string up to 63 characters in length
* ssid - octet string up to 32 octets
* ssidlength - length of ssid in octets
* output must be 40 octets in length and outputs 256 bits of key
*/
static int PasswordHash (char *password, unsigned char *ssid, int ssidlength, unsigned char *output)
{
    if ((strlen(password) > 63) || (ssidlength > 32))
        return -__LINE__;
    F(password, ssid, ssidlength, 4096, 1, output);
    F(password, ssid, ssidlength, 4096, 2, &output[A_SHA_DIGEST_LEN]);
    return 0;
}

int pkcs5_pbkdf2 (char *KeyPassphrase, unsigned char *ssid, unsigned int ssidlength, char *PreSharedKey)
{
    int ret = 0;
    char password[64]="";
    unsigned char output[40] = "";

    do{
        if(NULL==KeyPassphrase||NULL==ssid||NULL==PreSharedKey){
            ret = -__LINE__;
            break;
        }
        snprintf(password, sizeof(password), "%s", KeyPassphrase);
        if(PasswordHash (password, ssid, ssidlength, output)){
            ret = -__LINE__;
            break;
        }
        if(64!=Hex2String(output, 32, PreSharedKey)){
            ret = -__LINE__;
            break;
        }
    }while(0);

    return ret;
}

int key_sha1_hash ( unsigned char *ssid, unsigned int ssidlength, char *Key, char *KeySha1)
{
    int ret = 0;
    unsigned char output[40] = "";

    do{
        if(NULL==Key||NULL==ssid||NULL==KeySha1 || ssidlength > 32){
            ret = -__LINE__;
            break;
        }

        F(Key, ssid, ssidlength, 1, 1, output);
        F(Key, ssid, ssidlength, 1, 2, &output[A_SHA_DIGEST_LEN]);

        if(SIZE_80!=Hex2String(output, SIZE_40, KeySha1)){
            ret = -__LINE__;
            break;
        }
    }while(0);

    return ret;
}

