#ifndef __BASE64_H__
#define __BASE64_H__

typedef enum {							/* Certificate object types */
	CRYPT_CERTTYPE_NONE,				/* No certificate type */
	CRYPT_CERTTYPE_CERTIFICATE,			/* Certificate */
	CRYPT_CERTTYPE_ATTRIBUTE_CERT,		/* Attribute certificate */
	CRYPT_CERTTYPE_CERTCHAIN,			/* PKCS #7 certificate chain */
	CRYPT_CERTTYPE_CERTREQUEST,			/* PKCS #10 certification request */
	CRYPT_CERTTYPE_REQUEST_CERT,		/* CRMF certification request */
	CRYPT_CERTTYPE_REQUEST_REVOCATION,	/* CMP revocation request */
	CRYPT_CERTTYPE_CRL,					/* CRL */
	CRYPT_CERTTYPE_CMS_ATTRIBUTES,	  	/* CMS attributes */
	CRYPT_CERTTYPE_OCSP_REQUEST,		/* OCSP request */
	CRYPT_CERTTYPE_OCSP_RESPONSE,		/* OCSP response */
	CRYPT_CERTTYPE_NS_SPKAC,			/* Netscape SPKAC = cert request */
	CRYPT_CERTTYPE_CMS_CERTSET,			/* CMS SET OF Certificate = cert chain */
	CRYPT_CERTTYPE_SSL_CERTCHAIN,		/* SSL certificate chain = cert chain */
	CRYPT_CERTTYPE_LAST,				/* Last possible cert.type */
	CRYPT_CERTTYPE_LAST_EXTERNAL = CRYPT_CERTTYPE_NS_SPKAC
} CRYPT_CERTTYPE_TYPE;

typedef enum {
	CRYPT_CERTFORMAT_NONE,				/* No certificate format */
	CRYPT_CERTFORMAT_CERTIFICATE,		/* DER-encoded certificate */
	CRYPT_CERTFORMAT_CERTCHAIN,			/* PKCS #7 certificate chain */
	CRYPT_CERTFORMAT_TEXT_CERTIFICATE,	/* base-64 wrapped cert */
	CRYPT_CERTFORMAT_TEXT_CERTCHAIN,	/* base-64 wrapped cert chain */
	CRYPT_CERTFORMAT_LAST,				/* Last possible cert.format type */
	CRYPT_ICERTFORMAT_SMIME_CERTIFICATE /* S/MIME cert.request or cert chain */
} CRYPT_CERTFORMAT_TYPE;

int base64Encode( unsigned char * binaryDataPtr, int binaryDataLength, 
		char * stringData ,int *stringDataLength);
int base64Decode( char *stringData, int stringDataLen,
    unsigned char * binaryDataPtr, int * binaryDataLength );

#endif /*__BASE64_H__*/
