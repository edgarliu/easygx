/* vi: set sw=4 ts=4: */
/*
 * crypt() for uClibc
 * Copyright (C) 2000-2006 by Erik Andersen <andersen@uclibc.org>
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

#define __FORCE_GLIBC
//#include <unistd.h>
#include <stdlib.h>
//#include <crypt.h>
#include "crypt_misc.h"
#include "sfpr_crypt.h"
char *sfpr_crypt(const char *key, const char *salt, sfpr_crypt_mode_t mode)
{
	const unsigned char *ukey = (const unsigned char *)key;
	const unsigned char *usalt = (const unsigned char *)salt;
	if (salt[0] == '$' && salt[2] == '$') {
		if (*++salt == '1')
			if(mode == sfpr_encrypt)
				return __md5_crypt(ukey, usalt);
			else
				return NULL;
			
		else if(*salt == '3')
			if(mode == sfpr_encrypt)
				return __aes_encrypt(ukey, usalt);
			else
				return __aes_decrypt(ukey, usalt);
#ifdef __UCLIBC_HAS_SHA256_CRYPT_IMPL__
		else if (*salt == '5')
			if(mode == sfpr_encrypt)
				return __sha256_crypt(ukey, usalt);
			else
				return NULL;
#endif
#ifdef __UCLIBC_HAS_SHA512_CRYPT_IMPL__
		else if (*salt == '6')
			if(mode == sfpr_encrypt)
				return __sha512_crypt(ukey, usalt);
			else
				return NULL;
#endif
	}
	if(mode == sfpr_encrypt)
		return __des_crypt(ukey, usalt);
	else
		return NULL;
}


