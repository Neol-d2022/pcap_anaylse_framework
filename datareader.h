typedef void (*data_reader)(const void *source, void *dst, unsigned int len);

void big(const void *source, void *dst, unsigned int len);
void little(const void *source, void *dst, unsigned int len);
