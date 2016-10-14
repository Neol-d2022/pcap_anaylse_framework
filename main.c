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

int main(int argc, char **argv) {
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr phdr;
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
		while((p = pcap_next(pf, &phdr))) {
			result = ph(p, 0, phdr.len);
			//if(result == -1) break;
		}
	
		pcap_close(pf);
	}
	
	printf("%u\n", connCount());
	return result;
}
