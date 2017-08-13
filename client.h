#ifndef _CLIENT_H_
#define _CLIENT_H_


void sync_compress(char *buffer, long lSize, char* opbuffer, long* opsize);
void async_compress(char *buffer, long lSize, char* opbuffer, long* opsize, int num, int* opdone);

#endif
