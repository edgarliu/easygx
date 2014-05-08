/*
* Functions to compute MD5 message digest of files or memory blocks
* according to the definition of MD5 in RFC 1321 from April 1992.
* Copyright (C) 1995, 1996 Free Software Foundation, Inc. NOTE: The
* canonical source of this file is maintained with the GNU C Library.
* Bugs can be reported to bug-glibc@prep.ai.mit.edu.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published
* by the Free Software Foundation; either version 2, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
* 02111-1307, USA.
*
* Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.
* Modified by Gray Watson <http://256.com/gray/>, 1997.
*
* $Id: md5.c,v 1.5 2000/03/09 04:06:41 gray Exp $
*/

/*
* NOTE: during quick performance tests on a Sun Sparc Ultra 1 and an
* Alpha 255 300, these functions performed upwards of 3mb/sec
* including disk I/O time.
*/

/*
* MD5 Test Suite from RFC1321: http://ds.internic.net:/rfc/rfc1321.txt
*
* MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
* MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
* MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
* MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
* MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
* MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
* d174ab98d277d9f5a5611c2c9f419d9f
* MD5 ("123456789012345678901234567890123456789012345678901234567890123456
* 78901234567890") = 57edf4a22be3c955ac49da2e2107b67a
*/
#include "sfpr_md5.h"

/*
* Local defines for the md5 functions.
*
* $Id: md5_loc.h,v 1.3 2000/03/09 04:06:41 gray Exp $
*/

/*
* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
* rights reserved.
*
* License to copy and use this software is granted provided that it is
* identified as the "RSA Data Security, Inc. MD5 Message-Digest
* Algorithm" in all material mentioning or referencing this software
* or this function.
*
* License is also granted to make and use derivative works provided that
* such works are identified as "derived from the RSA Data Security,
* Inc. MD5 Message-Digest Algorithm" in all material mentioning or
* referencing the derived work.
*
* RSA Data Security, Inc. makes no representations concerning either the
* merchantability of this software or the suitability of this
* software for any particular purpose. It is provided "as is" without
* express or implied warranty of any kind.
*
* These notices must be retained in any copies of any part of this
* documentation and/or software.
*/

#ifndef __MD5_LOC_H__
#define __MD5_LOC_H__

#define HEX_STRING "0123456789abcdef" /* to convert to hex */
#define BLOCK_SIZE_MASK (MD5_BLOCK_SIZE - 1)

/* #define __BIG_ENDIAN__ */
#ifdef __BIG_ENDIAN__
/*
* big endian - big is better
*/
#define SWAP(n) (((n) << 24) | (((n) & 0xff00) << 8) | (((n) >> 8) & 0xff00) | ((n) >> 24))
#else
/*
* little endian
*/
#define SWAP(n) (n)
#endif

/*
* These are the four functions used in the four steps of the MD5
* algorithm and defined in the RFC 1321. The first function is a
* little bit optimized (as found in Colin Plumbs public domain
* implementation).
*/
/* #define FF(b, c, d) ((b & c) | (~b & d)) */
#define FF(b, c, d) (d ^ (b & (c ^ d)))
#define FG(b, c, d) FF(d, b, c)
#define FH(b, c, d) (b ^ c ^ d)
#define FI(b, c, d) (c ^ (b | ~d))

/*
* It is unfortunate that C does not provide an operator for cyclic
* rotation. Hope the C compiler is smart enough. -- Modified to
* remove the w = at the front - Gray 2/97
*/
#define CYCLIC(w, s) ((w << s) | (w >> (32 - s)))

/*
* First Round: using the given function, the context and a constant
* the next context is computed. Because the algorithms processing
* unit is a 32-bit word and it is determined to work on words in
* little endian byte order we perhaps have to change the byte order
* before the computation. To reduce the work for the next steps we
* store the swapped words in the array CORRECT_WORDS. -- Modified to
* fix the handling of unaligned buffer spaces - Gray 7/97
*/
#define OP1(a, b, c, d, b_p, c_p, s, T) \
do { \
memcpy(c_p, b_p, sizeof(md5_uint32)); \
*c_p = SWAP(*c_p); \
a += FF (b, c, d) + *c_p + T; \
a = CYCLIC (a, s); \
a += b; \
b_p = (char *)b_p + sizeof(md5_uint32); \
c_p++; \
} while (0)

/*
* Second to Fourth Round: we have the possibly swapped words in
* CORRECT_WORDS. Redefine the macro to take an additional first
* argument specifying the function to use.
*/
#define OP234(FUNC, a, b, c, d, k, s, T) \
do { \
a += FUNC (b, c, d) + k + T; \
a = CYCLIC (a, s); \
a += b; \
} while (0)

#endif /* __MD5_LOC_H__ */
