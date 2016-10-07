#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "datareader.h"

int handler_80211(const unsigned char *p, void *param, unsigned int len); //ieee80211.c

int handler_prism(const unsigned char *p, void *param, unsigned int len) {
	char dev_name[32];
	data_reader dr = 0;
	const unsigned char *data;
	int result = 1;
	unsigned int code, mlen;
	unsigned int did;
	unsigned short s, dlen;
	
	if(len < 24) return 1;
	switch(p[0]) {
		case 0x00:
			dr = big;
			break;
		case 0x41:
		case 0x44:
			dr = little;
			break;
		default:
			fprintf(stderr, "[PRISM HEADER]\nUnknown endian type in the packet.\n");
			return -1;
	}
	
	dr(p + 0, &code, 4u);
	dr(p + 4, &mlen, 4u);
	memcpy(dev_name, p + 8, 16);
	dev_name[16] = '\0';
	
	if(code != 0x00000041 && code != 0x00000044) {
		fprintf(stderr, "[PRISM HEADER]\nUnknown Msgcode in the packet(0x%08X).\n", code);
		return -1;
	}
	
	if(mlen > len) {
		fprintf(stderr, "[PRISM HEADER]\nInvalid Message Length.\nMessage Length: %u\nCapture Packet Length(This Layer): %u\n", mlen, len);
		return -1;
	}
	
	handler_80211(p + mlen, param, len - mlen);
	
	return result;
}
