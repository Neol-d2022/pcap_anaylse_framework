#include "datareader.h"

int addConn(unsigned int ip_dst, unsigned int ip_src, unsigned short port_dst, unsigned short port_src); //connM.c
int containsConn(unsigned int ip_dst, unsigned int ip_src, unsigned short port_dst, unsigned short port_src); //connM.c

int handler_udp(const unsigned char *p, void *param, unsigned int len) {
	unsigned int ip_src, ip_dst;
	unsigned short port_src, port_dst;
	
	big(param, &ip_src, sizeof(ip_src));
	big(((char*)param) + sizeof(ip_src), &ip_dst, sizeof(ip_dst));
	
	big(p, &port_src, sizeof(port_src));
	big(p + sizeof(port_dst), &port_dst, sizeof(port_dst));
	
	if(!containsConn(ip_dst, ip_src, port_dst, port_src))
		addConn(ip_dst, ip_src, port_dst, port_src);
	
	return 0;
}
