int handler_llc(const unsigned char *p, void *param, unsigned int len); //llc.c

int handler_80211(const unsigned char *p, void *param, unsigned int len) {
	unsigned char toDS, fromDS, addr4 = 0, header;
	
	if(len < 24) return -1;
	
	toDS = (unsigned char)((p[2] & 0x80) >> 7);
	fromDS = (unsigned char)((p[2] & 0x40) >> 6);
	
	if(toDS && fromDS) addr4 = 0xFF;
	
	if(addr4) header = 30;
	else header = 24;
	
	if(header > len) return -1;
	
	return handler_llc(p + header, param, len - header - 4);
}
