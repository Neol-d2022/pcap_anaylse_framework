#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "datareader.h"

int handler_80211(const unsigned char *p, void *param, unsigned int len); //ieee80211.c

static double rate2Mbits(unsigned int rate) {
	unsigned int mul;
	mul = rate & 0x0000007F;
	return 500000.0 * mul;
} 

int handler_prism(const unsigned char *p, void *param, unsigned int len) {
	char dev_name[32];
	data_reader dr = 0;
	const unsigned char *data;
	double arrTime, txRate, txTime;
	int result = 1;
	unsigned int code, mlen;
	unsigned int did, index, didData, rateValue = 1, frameSize = len - 144;
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
			fprintf(stderr, "[PRISM HEADER] Unknown endian type in the packet.\n");
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

	index = 24;
	while(index < mlen) {
		dr(p + index, &did, 4u); index += 4;
		dr(p + index, &s, 2u); index += 2;
		dr(p + index, &dlen, 2u); index += 2;
		dr(p + index, &didData, 4u); index += 4;
		switch(did) {
			case 0x00001041:
			case 0x00010044: //host time
				break;
			case 0x00002041:
			case 0x00020044: //MAC time
				break;
			case 0x00003041:
			case 0x00030044: //channel
				break;
			case 0x00004041:
			case 0x00040044: //RSSI
				break;
			case 0x00005041:
			case 0x00050044: //signal quality
				break;
			case 0x00006041:
			case 0x00060044: //signal
				break;
			case 0x00007041:
			case 0x00070044: //noise
				break;
			case 0x00008041:
			case 0x00080044: //rate
				rateValue = didData;
				break;
			case 0x00009041:
			case 0x00090044: //isTX
				break;
			case 0x0000A041:
			case 0x000A0044: //frame length
				frameSize = didData;
				break;
			default:
				fprintf(stderr, "[PRISM HEADER]\nInvalid DID(0x%08X).\n", did);
				return -1;
		}
	}
	
	if(mlen > len) {
		fprintf(stderr, "[PRISM HEADER]\nInvalid Message Length.\nMessage Length: %u\nCapture Packet Length(This Layer): %u\n", mlen, len);
		return -1;
	}

	arrTime = *((double*)param);
	txRate = rate2Mbits(rateValue);
	txTime = (double)frameSize / txRate;

	printf("[DEBUG] %u bytes / %6.2lfMbit/s = %15.9lf seconds\n", frameSize, txRate / 1000000.0, txTime);
	
	handler_80211(p + mlen, param, len - mlen);
	
	return result;
}
