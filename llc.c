#include <stdio.h>

int handler_ip(const unsigned char *p, void *param, unsigned int len); //ip.c

int handler_llc(const unsigned char *p, void *param, unsigned int len) {
	unsigned char dsap, ssap;
	unsigned short control, etherType;
	unsigned int org;
	
	dsap = p[0];
	ssap = p[1];
	if(dsap == 0xaa && ssap == 0xaa) {
		control = p[2];
		org = (p[3] << 16) | (p[4] << 8) | p[5];
		if(control == 0x03 && org == 0) {
			//Ether
			if(len < 8) return -1;
			etherType = (p[6] << 8) | p[7];
			switch(etherType) {
				case 0x0800: {
					return handler_ip(p + 8, param, len - 8);
				}
				/*case 0x0806: {
					//ARP
					break;
				}
				case 0x8137: {
					//IPX
					break;
				}*/
				case 0x8863: {
					//PPPOE_Discovery
					break;
				}
				case 0x8864: {
					//PPPOE_Session
					break;
				}
				default: {
					fprintf(stderr, "[LLC HEADER] Unknown EtherType in the packet(0x%08hX).\n", etherType);
					break;
				}
			}
		}
		else fprintf(stderr, "[LLC HEADER] Unknown control & org in the packet(DSAP=0x%08hhX, SSAP=0x%08hhX, control=0x%08hX, org=0x%08X).\n", dsap, ssap, control, org);
	}
	else fprintf(stderr, "[LLC HEADER] Unknown DSAP & SSAP in the packet(DSAP=0x%08hhX, SSAP=0x%08hhX).\n", dsap, ssap);
	
	return -1;
}
