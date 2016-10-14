#include <stdio.h>

int handler_tcp(const unsigned char *p, void *param, unsigned int len); //tcp.c
int handler_udp(const unsigned char *p, void *param, unsigned int len); //udp.c

int handler_ip(const unsigned char *p, void *param, unsigned int len) {
	unsigned int headerLength, protocol;
	
	headerLength = (p[0] & 0x0F) << 2;
	protocol = p[9];
	
	if(headerLength > len) return -1;
	
	switch(protocol) {
		case 0x06: {
			//TCP
			return handler_tcp(p + headerLength, param, len - headerLength);
		}
		case 0x11: {
			//UDP
			return handler_udp(p + headerLength, param, len - headerLength);
		}
		default: {
			fprintf(stderr, "[IP HEADER]\nUnknown IP protocol in the packet(0x%08hX).\n", protocol);
			break;
		}
	}
	
	return -1;
}
