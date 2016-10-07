#include <string.h>

#define END_L
//#define END_B

static void revert(const unsigned char *source, unsigned char *dst, unsigned int len) {
	unsigned int i;
	
	for(i = 0; i < len; i += 1) {
		dst[i] = source[len - i - 1];
	}
}

void big(const void *source, void *dst, unsigned int len) {
#ifdef END_L
	revert((unsigned char*)source, (unsigned char*)dst, len);
#else
#ifndef END_B
#error "BOTH Big Endian And Little Endian is not defined, Please specify one."
#else
	memcpy(dst, source, len);
#endif
#endif
}

void little(const void *source, void *dst, unsigned int len) {
#ifdef END_B
	revert((unsigned char*)source, (unsigned char*)dst, len);
#else
#ifndef END_L
#error "BOTH Big Endian And Little Endian is not defined, Please specify one."
#else
	memcpy(dst, source, len);
#endif
#endif
}
