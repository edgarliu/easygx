#include "aes.h"

extern char *__aes_encrypt (const char *__key, const char *__salt)
{
    AES_KEY aes;
    unsigned char key[AES_BLOCK_SIZE];        // AES_BLOCK_SIZE = 16
    unsigned char iv[AES_BLOCK_SIZE];        // init vector
    unsigned char encrypt_string[AED_KEY_SIZE];
    unsigned int len;        // encrypt length (in multiple of AES_BLOCK_SIZE)
    unsigned int i;
	static char result[AED_KEY_SIZE*2];
	memset(encrypt_string, 0x0, sizeof(encrypt_string));
	memset(result, 0x0, sizeof(result));
    // set the encryption length
    len = 0;
    if ((strlen(__key) + 1) % AES_BLOCK_SIZE == 0) {
        len = strlen(__key) + 1;
    } else {
        len = ((strlen(__key) + 1) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }
	
	// Set encryption key
	memset(key, 0x0, sizeof(key));
	memcpy(key, __salt, sizeof(key));
	for (i=strlen(key); i<AES_BLOCK_SIZE; ++i) {
		key[i] = 32 + i;
	}
	for (i=0; i<AES_BLOCK_SIZE; ++i) {
		iv[i] = 0;
	}
	if (AES_set_encrypt_key(key, AED_KEY_SIZE, &aes) < 0) {
		fprintf(stderr, "Unable to set encryption key in AES\n");
		return NULL;
	}

#if AES_ENABLE_CBC    
    AES_cbc_encrypt((unsigned char*)__key, encrypt_string, len, &aes, iv, AES_ENCRYPT);
#else 
    AES_encrypt((unsigned char*)__key, encrypt_string, &aes);
#endif
	for(i=0; i < len; i++)
		sprintf(result+i*2, "%x%x", (encrypt_string[i] >> 4) & 0xf,	encrypt_string[i] & 0xf);	
	return result;
}

extern char *__aes_decrypt (const char *__key, const char *__salt)
{
    AES_KEY aes;
    unsigned char key[AES_BLOCK_SIZE];        // AES_BLOCK_SIZE = 16
    unsigned char iv[AES_BLOCK_SIZE];        // init vector
    unsigned char decrypt_string[AED_KEY_SIZE*2];
	static char result[AED_KEY_SIZE];
	char tmp[4];
    unsigned int len = 0;        // encrypt length (in multiple of AES_BLOCK_SIZE)
    unsigned int i,j=0;

	memset(decrypt_string, 0x0, sizeof(decrypt_string));
	memset(result, 0x0, sizeof(result));
    // set the encryption length
	for(i=0; i< strlen(__key); i+=2)
	{
		memset(tmp, 0x0, sizeof(tmp));
		memcpy(tmp, __key+i, 2);
		decrypt_string[j++] = strtol(tmp, NULL, 16);
		len++;
	}
	// Set encryption key
	memset(key, 0x0, sizeof(key));
	memcpy(key, __salt, sizeof(key));
	for (i=strlen(key); i<AES_BLOCK_SIZE; ++i) {
		key[i] = 32 + i;
	}
	for (i=0; i<AES_BLOCK_SIZE; ++i) {
		iv[i] = 0;
	}
    if (AES_set_decrypt_key(key, AED_KEY_SIZE, &aes) < 0) {
        fprintf(stderr, "Unable to set decryption key in AES\n");
        return NULL;
    }
 
    // decrypt
#if AES_ENABLE_CBC    
    AES_cbc_encrypt((unsigned char*)decrypt_string, result, len, &aes, iv, AES_DECRYPT);
#else
	AES_decrypt((unsigned char*)decrypt_string, result, &aes);
#endif	
	return result;
}

