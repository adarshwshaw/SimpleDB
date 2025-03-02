#ifndef __PAGE__
#define __PAGE__

#include <stdint.h>

typedef struct {
    uint64_t len;
    uint64_t cap;
    uint8_t *bytes;
} Page;

Page Page_create(uint64_t blocksize);
Page Page_create_from_bytes(uint8_t* bytes,uint64_t size);

int Page_getint(Page *page,uint64_t offset);
void Page_setint(Page *page,uint64_t offset, int n);

uint8_t* Page_getbytes(Page *page,int offset);
void Page_setbytes(Page *page,uint64_t offset, uint8_t* bytes, uint32_t size);

char* Page_getstring(Page *page,uint64_t offset);
void Page_setstring(Page *page,uint64_t offset, const char* str);


#endif

