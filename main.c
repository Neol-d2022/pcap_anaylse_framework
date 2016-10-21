#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

typedef int (*packet_handler)(const unsigned char *p, void *param, unsigned int len);
typedef struct {
	int protocol;
	packet_handler phdler;
} handler;

int handler_prism(const unsigned char *p, void *param, unsigned int len); //prism.c
unsigned int connCount(void); //connM.c

static handler handlers[] = {
	{119, handler_prism}
};

static int cmpHandlers(const void *a, const void *b) {
	handler *c, *d;
	
	c = (handler*)a;
	d = (handler*)b;
	
	return (c->protocol > d->protocol) ? 1 : ((c->protocol < d->protocol) ? -1 : 0);
}

static struct timeval tvdiff(struct timeval a, struct timeval b) {
	struct timeval c;

	if(a.tv_sec < b.tv_sec) return tvdiff(b, a);
	else if(a.tv_sec == b.tv_sec && a.tv_usec < b.tv_usec) return tvdiff(b, a);

	c.tv_sec = a.tv_sec - b.tv_sec;
	c.tv_usec = a.tv_usec - b.tv_usec;
	if(c.tv_usec < 0) {
		c.tv_sec -= 1;
		c.tv_usec = 1 - c.tv_usec;
	} 

	return c;
}

int main(int argc, char **argv) {
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr phdr;
	struct timeval tv, relateTv;
	double arrTime;
	handler key;
	pcap_t *pf;
	handler *h;
	const unsigned char *p;
	packet_handler ph = 0;
	int protocol, result = 0, i;
	
	if(argc < 2) {
		fprintf(stderr, "Too few arguments.\n");
		return 1;
	}
	
	for(i = 1; i < argc; i += 1) {
		pf = pcap_open_offline(argv[i], errbuf);
		if(!pf) {
			fprintf(stderr, "[Error] Cannot open file '%s'\n%s\n", argv[i], errbuf);
			return 1;
		}
		protocol = pcap_datalink(pf);
	
#ifdef DEBUG
		fprintf(stderr, "%s(%d) : %s\n", pcap_datalink_val_to_name(protocol), protocol, pcap_datalink_val_to_description(protocol));
#endif
		key.protocol = protocol;
		h = (handler*)bsearch(&key, handlers, sizeof(handlers) / sizeof(handlers[0]), sizeof(handlers[0]), cmpHandlers);
		if(!h) {
			fprintf(stderr, "[Error] No handler function for protocol:\n%s(%d) : %s\n", pcap_datalink_val_to_name(protocol), protocol, pcap_datalink_val_to_description(protocol));
			return 1;
		}
	
		ph = h->phdler;
		if((p = pcap_next(pf, &phdr))) {
			tv = phdr.ts;
			arrTime = 0;
			//fprintf(stderr, "[DEBUG] arrTime = %14.9lf\n", arrTime);
			result = ph(p, &arrTime, phdr.len);

			while((p = pcap_next(pf, &phdr))) {
				relateTv = tvdiff(phdr.ts, tv);
				arrTime = relateTv.tv_sec + (relateTv.tv_usec * 0.000000001);
				//fprintf(stderr, "[DEBUG] arrTime = %14.9lf\n", arrTime);
				result = ph(p, &arrTime, phdr.len);
				//if(result == -1) break;
			}
		}
	
		pcap_close(pf);
	}
	
	//printf("%u\n", connCount());
	return result;
}
