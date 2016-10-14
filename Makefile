CC=gcc

LIBS=-lpcap

CFLAGS=-Wall -Wextra -Ofast
LFLAGS=-s $(LIBS)

OBJS=main.o prism.o datareader.o ieee80211.o llc.o ip.o tcp.o udp.o
DEPS=datareader.h

BIN=pcap_test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	rm -f *.o $(BIN)
