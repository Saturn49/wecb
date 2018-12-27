

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

typedef unsigned char BYTE ;

#include "base64.h"

#define B64encode(outbuf, inbuf, inlen)    misc_base64encode( outbuf, inbuf, inlen, CRYPT_CERTTYPE_NONE );
#define B64decode(outbuf, inbufptr, inlen) misc_base64decode( outbuf, inbufptr, inlen, CRYPT_CERTTYPE_NONE );


							
/****************************************************************************
*                                                                           *
*             Base64 En/Decoding Functions                                  *
*                                                                           *
****************************************************************************/

/* Some interfaces can't handle binary data, so we base64-encode it using the
   following encode/decode tables (from RFC 1113) */

#define EOL		"\r\n"
#define EOL_LEN	2

#define BPAD    '='   /* Padding for odd-sized output */
#define BERR    0xFF  /* Illegal char marker */
#define BEOF    0x7F  /* EOF marker (padding char or EOL) */

#define roundUp( size, roundSize ) ( ( ( size ) + ( ( roundSize ) - 1 ) ) & ~( ( roundSize ) - 1 ) )



static const char binToAscii[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
#if 'A' == 0x41       /* ASCII */
  static const BYTE asciiToBin[] =
  { BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BEOF, BERR, BERR, BEOF, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, 0x3E, BERR, BERR, BERR, 0x3F,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, BERR, BERR, BERR, BEOF, BERR, BERR,
    BERR, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, BERR, BERR, BERR, BERR, BERR,
    BERR, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR
  };
#elif 'A' == 0xC1     /* EBCDIC */
  /* EBCDIC character mappings:
    A-I C1-C9
    J-R D1-D9
    S-Z E2-E9
    a-i 81-89
    j-r 91-99
    s-z A2-A9
    0-9 F0-F9
    +   4E
    /   61
    =   7E  Uses BEOF in table */   
static const BYTE asciiToBin[] =
  { BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR, /*00*/
    BERR, BERR, BEOF, BERR, BERR, BEOF, BERR, BERR, /* CR, LF */
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR, /*10*/
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR, /*20*/
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR, /*30*/
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR, /*40*/
    BERR, BERR, BERR, BERR, BERR, BERR, 0x3E, BERR, /* + */
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR, /*50*/
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, 0x3F, BERR, BERR, BERR, BERR, BERR, BERR, /*60*/  /* / */
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR, /*70*/
    BERR, BERR, BERR, BERR, BERR, BERR, BEOF, BERR,     /* = */
    BERR, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, /*80*/  /* a-i */
    0x21, 0x22, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, /*90*/  /* j-r */
    0x2A, 0x2B, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, /*A0*/  /* s-z */
    0x32, 0x33, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR, /*B0*/
    BERR, BERR, BERR, BERR, BERR, BERR, BERR, BERR
    BERR, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, /*C0*/  /* A-I */
    0x07, 0x08, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, /*D0*/  /* J-R */
    0x10, 0x11, BERR, BERR, BERR, BERR, BERR, BERR,
    BERR, BERR, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, /*E0*/  /* S-Z */
    0x18, x019, BERR, BERR, BERR, BERR, BERR, BERR,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, /*F0*/  /* 0-9 */
    0x3C, 0x3D, BERR, BERR, BERR, BERR, BERR, BERR
  };
#else
  #error System is neither ASCII nor EBCDIC
#endif /* Different character code mappings */

/* The size of lines for PEM-type formatting.  This is only used for encoding,
   for decoding we adjust to whatever size the sender has used */

#define TEXT_LINESIZE 64
#define BINARY_LINESIZE 48

/* Basic single-char en/decode functions */

#define misc_encode(data)  binToAscii[ data ]
#define misc_decode(data)  asciiToBin[ data ]

/* The headers and trailers used for base64-encoded certificate objects.
   Since the zero-th value is a non-value, we include a dummy entry at the
   start */

static const char *headerTbl[] = {
  "",//NULL,
  "-----BEGIN CERTIFICATE-----" EOL,
  "-----BEGIN ATTRIBUTE CERTIFICATE-----" EOL,
  "-----BEGIN CERTIFICATE CHAIN-----" EOL,
  "-----BEGIN NEW CERTIFICATE REQUEST-----" EOL,
  "-----BEGIN CRL-----"  EOL
};
static const char *trailerTbl[] = {
  "",//NULL,
  "-----END CERTIFICATE-----" EOL,
  "-----END ATTRIBUTE CERTIFICATE-----" EOL,
  "-----END CERTIFICATE CHAIN-----" EOL,
  "-----END NEW CERTIFICATE REQUEST-----" EOL,
  "-----END CRL-----" EOL
};

/* Encode a block of binary data into the base64 format, returning the total
   number of output bytes */
/////////////////////////////////////////////////////////////////////////////////////////////////
//非标准编码
//base64编码，此部分编码结果无分行，数据结尾没有回车换行
/////////////////////////////////////////////////////////////////////////////////////////////////
int misc_base64encode( char *outBuffer, const void *inBuffer, const int count,
          const CRYPT_CERTTYPE_TYPE certType )
{
  int srcIndex = 0, destIndex = 0, lineCount = 0, remainder = count % 3;
  BYTE *inBufferPtr = ( BYTE * ) inBuffer;

  /* If it's a certificate object, add the header */
  if( certType != CRYPT_CERTTYPE_NONE ){
    strcpy( outBuffer, headerTbl[ certType ] );
    destIndex = strlen( headerTbl[ certType ] );
  }

  /* Encode the data */
  while( srcIndex < count ){
    /* If we've reached the end of a line of binary data and it's a
       certificate, add the EOL marker */
    if( certType != CRYPT_CERTTYPE_NONE && lineCount == BINARY_LINESIZE ) {
      strcpy( outBuffer + destIndex, EOL );
      destIndex += EOL_LEN;
      lineCount = 0;
    }
    lineCount += 3;

    /* Encode a block of data from the input buffer */
    outBuffer[ destIndex++ ] = misc_encode( inBufferPtr[ srcIndex ] >> 2 );
    outBuffer[ destIndex++ ] = misc_encode( ( ( inBufferPtr[ srcIndex ] << 4 ) & 0x30 ) |
                       ( ( inBufferPtr[ srcIndex + 1 ] >> 4 ) & 0x0F ) );
    srcIndex++;
    outBuffer[ destIndex++ ] = misc_encode( ( ( inBufferPtr[ srcIndex ] << 2 ) & 0x3C ) |
                       ( ( inBufferPtr[ srcIndex + 1 ] >> 6 ) & 0x03 ) );
    srcIndex++;
    outBuffer[ destIndex++ ] = misc_encode( inBufferPtr[ srcIndex++ ] & 0x3F );
  }

  /* Go back and add padding and correctly encode the last char if we've
     encoded too many characters */
  if( remainder == 2 ) {
    /* There were only 2 bytes in the last group */
    outBuffer[ destIndex - 1 ] = BPAD;
    outBuffer[ destIndex - 2 ] = \
        misc_encode( ( inBufferPtr[ srcIndex - 2 ] << 2 ) & 0x3C );
  }
  else if( remainder == 1 )  {
    /* There was only 1 byte in the last group */
    outBuffer[ destIndex - 2 ] = outBuffer[ destIndex - 1 ] = BPAD;
    outBuffer[ destIndex - 3 ] = \
        misc_encode( ( inBufferPtr[ srcIndex - 3 ] << 4 ) & 0x30 );
  }

  /* If it's a certificate object, add the trailer */
  if( certType != CRYPT_CERTTYPE_NONE ) {
    strcpy( outBuffer + destIndex, EOL );
    strcpy( outBuffer + destIndex + EOL_LEN, trailerTbl[ certType ] );
    destIndex += strlen( trailerTbl[ certType ] );
  }else{
    /* It's not a certificate, truncate the unnecessary padding and add
       der terminador */
       
       
    //  {  即使不是对证书的编码，也不需要去掉结尾的padding
    //destIndex -= ( 3 - remainder ) % 3;
    // }
    
    
    
    //outBuffer[ destIndex ] = '\0';
  }

  /* Return a count of encoded bytes */
  return( destIndex );
}




/////////////////////////////////////////////////////////////////////////////////////////////////
//2003年10月修改为标准编码，标准编码有分行，数据结尾没有回车换行符
//base64编码，此部分编码结果是分行数据，每64个字符一行，换行为“0x0D 0X0A”,数据结尾没有“0x0D 0X0A”
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
int misc_base64encode( char *outBuffer, const void *inBuffer, const int count,
          const CRYPT_CERTTYPE_TYPE certType )
{
  int srcIndex = 0, destIndex = 0, lineCount = 0, remainder = count % 3;
  BYTE *inBufferPtr = ( BYTE * ) inBuffer;

  // If it's a certificate object, add the header 
  if( certType != CRYPT_CERTTYPE_NONE ){
    strcpy( outBuffer, headerTbl[ certType ] );
    destIndex = strlen( headerTbl[ certType ] );
  }

  // Encode the data 
  while( srcIndex < count ){
    // If we've reached the end of a line of binary data and it's a
    //   certificate, add the EOL marker 
///    if( certType != CRYPT_CERTTYPE_NONE && lineCount == BINARY_LINESIZE ) {

	  if(lineCount == BINARY_LINESIZE ) {
      strcpy( outBuffer + destIndex, EOL );
      destIndex += EOL_LEN;
      lineCount = 0;
    }
    lineCount += 3;

    // Encode a block of data from the input buffer 
    outBuffer[ destIndex++ ] = misc_encode( inBufferPtr[ srcIndex ] >> 2 );
    outBuffer[ destIndex++ ] = misc_encode( ( ( inBufferPtr[ srcIndex ] << 4 ) & 0x30 ) |
                       ( ( inBufferPtr[ srcIndex + 1 ] >> 4 ) & 0x0F ) );
    srcIndex++;
    outBuffer[ destIndex++ ] = misc_encode( ( ( inBufferPtr[ srcIndex ] << 2 ) & 0x3C ) |
                       ( ( inBufferPtr[ srcIndex + 1 ] >> 6 ) & 0x03 ) );
    srcIndex++;
    outBuffer[ destIndex++ ] = misc_encode( inBufferPtr[ srcIndex++ ] & 0x3F );
  }

  // Go back and add padding and correctly encode the last char if we've
   //  encoded too many characters 
  if( remainder == 2 ) {
    // There were only 2 bytes in the last group 
    outBuffer[ destIndex - 1 ] = BPAD;
    outBuffer[ destIndex - 2 ] = misc_encode( ( inBufferPtr[ srcIndex - 2 ] << 2 ) & 0x3C );
  }
  else if( remainder == 1 )  {
    // There was only 1 byte in the last group 
    outBuffer[ destIndex - 2 ] = outBuffer[ destIndex - 1 ] = BPAD;
    outBuffer[ destIndex - 3 ] = misc_encode( ( inBufferPtr[ srcIndex - 3 ] << 4 ) & 0x30 );
  }

  // If it's a certificate object, add the trailer 
///  if( certType != CRYPT_CERTTYPE_NONE ) {

///  strcpy( outBuffer + destIndex, EOL );

///	destIndex +=EOL_LEN;//在最后补"\r\n","///"的注释为CG修改的部分，此行为增加，要改回去就去掉此行，打开"\\\"的行即可

///    strcpy( outBuffer + destIndex + EOL_LEN, trailerTbl[ certType ] );
///    destIndex += strlen( trailerTbl[ certType ] );
///  }else{
    // It's not a certificate, truncate the unnecessary padding and add
     //  der terminador 
///    destIndex -= ( 3 - remainder ) % 3;
    //outBuffer[ destIndex ] = '\0';
///  }

  // Return a count of encoded bytes 
  return( destIndex );
}
*/
/* Decode a block of binary data from the base64 format, returning the total
   number of decoded bytes */

static int misc_fixedBase64decode( void *outBuffer, const char *inBuffer,
                const int count )
{
  int srcIndex = 0, destIndex = 0;
  BYTE *outBufferPtr = outBuffer;

  /* Decode the base64 string as a fixed-i_length continuous string without
     padding or newlines */
  while( srcIndex < count ) {
    BYTE c0, c1, c2 = 0, c3 = 0;
    const int delta = count - srcIndex;

    /* Decode a block of data from the input buffer */
    c0 = misc_decode( inBuffer[ srcIndex++ ] );
    c1 = misc_decode( inBuffer[ srcIndex++ ] );
    if( delta > 2 ) {
      c2 = misc_decode( inBuffer[ srcIndex++ ] );
      if( delta > 3 )
        c3 = misc_decode( inBuffer[ srcIndex++ ] );
    }
    if( ( c0 | c1 | c2 | c3 ) == BERR )
      return( 0 );

    /* Copy the decoded data to the output buffer */
    outBufferPtr[ destIndex++ ] = ( c0 << 2 ) | ( c1 >> 4 );
    if( delta > 2 ){
      outBufferPtr[ destIndex++ ] = ( c1 << 4 ) | ( c2 >> 2);
      if( delta > 3 )
        outBufferPtr[ destIndex++ ] = ( c2 << 6 ) | ( c3 );
    }
  }

  /* Return count of decoded bytes */
  return( destIndex );
}

int misc_base64decode( void *outBuffer, const char *inBuffer, const int count,
          const CRYPT_CERTFORMAT_TYPE format )
{
  int srcIndex = 0, destIndex = 0, lineCount = 0, lineSize = 0;
  BYTE c0, c1, c2, c3, *outBufferPtr = outBuffer;

  /* If it's not a certificate, it's a straight base64 string and we can
     use the simplified decoding routines */
///  if( format == CRYPT_CERTFORMAT_NONE )
///   return( misc_fixedBase64decode( outBuffer, inBuffer, count ) );

  /* Decode the certificate body */
  while( 1 ){
    BYTE cx;

    /* Depending on implementations, the i_length of the base64-encoded
       line can vary from 60 to 72 chars, we ajust for this by checking
       for an EOL and setting the line i_length to this size */
    if( (!lineSize && (( inBuffer[ srcIndex ] == '\r' || inBuffer[ srcIndex ] == '\n' )))||srcIndex==count )
      lineSize = lineCount;

    /* If we've reached the end of a line of text, look for the EOL
       marker.  There's one problematic special case here where, if the
       encoding has produced bricktext, the end of the data will coincide
       with the EOL.  For CRYPT_CERTFORMAT_TEXT_CERTIFICATE this will give
       us '-----END' on the next line which is easy to check for, but for
       CRYPT_ICERTFORMAT_SMIME_CERTIFICATE what we end up with depends on
       the calling code, it could truncate immediately at the end of the
       data (which it isn't supposed to) so we get '\0', it could truncate
       after the EOL (so we get EOL + '\0'), it could continue with a
       futher content type after a blank line (so we get EOL + EOL), or
       it could truncate without the '\0' so we get garbage, which is the
       callers problem.  Because of this we look for all of these
       situations and, if any are found, set c0 to BEOF and advance
       srcIndex by 4 to take into account the adjustment for overshoot
       which occurs when we break out of the loop */
    if( lineCount == lineSize ) {
      /* Check for '\0' at the end of the data */
      if( format == CRYPT_ICERTFORMAT_SMIME_CERTIFICATE && !inBuffer[ srcIndex ] )
      {
        c0 = BEOF;
        srcIndex += 4;
        break;
      }

      /* Check for EOL */
      if( inBuffer[ srcIndex ] == '\n' )
        srcIndex++;
      else
        if( inBuffer[ srcIndex ] == '\r' ) {
          srcIndex++;
          if( inBuffer[ srcIndex ] == '\n' )
            srcIndex++;
        }
      lineCount = 0;
	  lineSize = 0;
      /* Check for '\0' or EOL (S/MIME) or '----END' (text) after EOL */

///    if( ( format == CRYPT_ICERTFORMAT_SMIME_CERTIFICATE && ( !inBuffer[ srcIndex ] || inBuffer[ srcIndex ] == '\n' ||
///          inBuffer[ srcIndex ] == '\r' ) ) || (format == CRYPT_CERTFORMAT_TEXT_CERTIFICATE && !strncmp( inBuffer + srcIndex, "-----END ", 9 ) ) )

	  if(( format == CRYPT_CERTTYPE_NONE && ( srcIndex==count||!inBuffer[ srcIndex ] || inBuffer[ srcIndex ] == '\n' ||

          inBuffer[ srcIndex ] == '\r' ) ) || (format == CRYPT_CERTFORMAT_TEXT_CERTIFICATE && !strncmp( inBuffer + srcIndex, "-----END ", 9 ) ) )

      {
        c0 = BEOF;
        srcIndex += 4;
        break;
      }
    }

    /* Decode a block of data from the input buffer */
    c0 = misc_decode( inBuffer[ srcIndex++ ] );
    c1 = misc_decode( inBuffer[ srcIndex++ ] );
    c2 = misc_decode( inBuffer[ srcIndex++ ] );
    c3 = misc_decode( inBuffer[ srcIndex++ ] );
    cx = c0 | c1 | c2 | c3;
    if( c0 == BEOF || cx == BEOF )
      /* We need to check c0 separately since hitting an EOF at c0 may
         cause later chars to be decoded as BERR */
      break;
    else
      if( cx == BERR )
        return( 0 );
    lineCount += 4;

    /* Copy the decoded data to the output buffer */
    outBufferPtr[ destIndex++ ] = ( c0 << 2 ) | ( c1 >> 4 );
    outBufferPtr[ destIndex++ ] = ( c1 << 4 ) | ( c2 >> 2);
    outBufferPtr[ destIndex++ ] = ( c2 << 6 ) | ( c3 );

  }

  /* Handle the truncation of data at the end.  Due to the 3 -> 4 encoding,
     we have the following mapping: 0 chars -> nothing, 1 char -> 2 + 2 pad,
     2 chars = 3 + 1 pad */
  if( c0 == BEOF )
    /* No padding, move back 4 chars */
    srcIndex -= 4;
  else{
    /* 2 chars padding, decode 1 from 2 */
    outBufferPtr[ destIndex++ ] = ( c0 << 2 ) | ( c1 >> 4 );
    if( c2 != BEOF )
      /* 1 char padding, decode 2 from 3 */
      outBufferPtr[ destIndex++ ] = ( c1 << 4 ) | ( c2 >> 2);
  }

  /* Make sure the certificate trailer is present */
 if( format == CRYPT_CERTFORMAT_TEXT_CERTIFICATE ) {

    if( inBuffer[ srcIndex ] == '\n' )
      srcIndex++;
    else if( inBuffer[ srcIndex ] == '\r' ) {
      srcIndex++;
      if( inBuffer[ srcIndex ] == '\n' )
        srcIndex++;
    }
    if( strncmp( inBuffer + srcIndex, "-----END ", 9 ) )
      return( 0 );
  }

  /* Return count of decoded bytes */
  return( destIndex );
}
//Encode binaryDataptr and the encode result is stringData
int base64Encode( unsigned char * binaryDataPtr, int binaryDataLength, 
					 char * stringData ,int *stringDataLength )
{
   int enlen;

   enlen = B64encode(stringData, binaryDataPtr, binaryDataLength);
    
   if (enlen > 0){
	  *stringDataLength = enlen;
      return(0);
   }else{
      return(-1);
   }
}
//Decode stringData and the decode result is binaryDataPtr 
int base64Decode( char * stringData, int stringDataLen, 
    unsigned char * binaryDataPtr, int * binaryDataLength )
{
   int delen;

   delen = B64decode(binaryDataPtr, stringData, stringDataLen);
 
   if (delen == 0){
	   return (-1);
   }else{
	   memcpy(binaryDataLength,&delen,sizeof(int));  
       return(0);
   }
}