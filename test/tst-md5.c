/* iksemel (XML parser for Jabber)
** Copyright (C) 2004 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iksemel.h"

int main (int argc, char *argv[])
{
	struct lala {
		char *str;
		char *hash;
	} known_hashes[] = {
		// Taken from http://tools.ietf.org/html/rfc1321
		{ "", "d41d8cd98f00b204e9800998ecf8427e" },
		{ "a", "0cc175b9c0f1b6a831c399e269772661" },
		{ "abc", "900150983cd24fb0d6963f7d28e17f72" },
		{ "message digest", "f96b697d7cb7938d525a2f31aaf161d0" },
		{ "abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b" },
		{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		  "d174ab98d277d9f5a5611c2c9f419d9f" },
		{ "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
		  "57edf4a22be3c955ac49da2e2107b67a" },
		{ NULL, NULL }
	};
	int i = 0;
	char buf[33];

	while (known_hashes[i].str) {
		iks_md5 (known_hashes[i].str, buf);
		if (strcmp (buf, known_hashes[i].hash) != 0) {
			printf("MD5 hash of \"%s\"\n", known_hashes[i].str);
			printf(" Result:   %s\n", buf);
			printf(" Expected: %s\n", known_hashes[i].hash);
			return 1;
		}
		i++;
	}
	return 0;
}
