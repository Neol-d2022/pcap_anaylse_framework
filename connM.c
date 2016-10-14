#include <string.h>
#include <stdlib.h>

typedef struct {
	unsigned int ipsrc;
	unsigned int ipdst;
	unsigned short portsrc;
	unsigned short portdst;
} conn;

static conn **conndb = 0;
static unsigned int eleCount = 0;

static int cmpConn(const void *a, const void *b) {
	return memcmp(*((const void **)a), *((const void **)b), sizeof(conn));
}

int addConn(unsigned int ip_dst, unsigned int ip_src, unsigned short port_dst, unsigned short port_src) {
	conn *p, **pp;
	
	p = (conn*)malloc(sizeof(*p));
	if(!p) return -1;
	
	p->ipsrc = ip_src;
	p->ipdst = ip_dst;
	p->portsrc = port_src;
	p->portdst = port_dst;
	
	if(!eleCount) {
		pp = (conn**)malloc(sizeof(*p));
		if(!pp) {
			free(p);
			return -1;
		}
		pp[0] = p;
		conndb = pp;
	}
	else {
		pp = (conn**)realloc(conndb, sizeof(*conndb) * (eleCount + 1));
		if(!pp) {
			free(p);
			return -1;
		}
		pp[eleCount] = p;
		conndb = pp;
	}
	eleCount += 1;
	qsort(pp, eleCount, sizeof(*pp), cmpConn);
	
	return 0;
}

int containsConn(unsigned int ip_dst, unsigned int ip_src, unsigned short port_dst, unsigned short port_src) {
	conn *p;
	int res;
	
	p = (conn*)malloc(sizeof(*p));
	if(!p) return 0;
	
	p->ipsrc = ip_src;
	p->ipdst = ip_dst;
	p->portsrc = port_src;
	p->portdst = port_dst;
	
	if(bsearch(&p, conndb, eleCount, sizeof(*conndb), cmpConn)) res = 1;
	else res = 0;
	
	free(p);
	return res;
}

unsigned int connCount(void) {
	return eleCount;
}
