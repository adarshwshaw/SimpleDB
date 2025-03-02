#include "page.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "utils.h"


Page Page_create(uint64_t blocksize){
    Page page = {0};
    page.cap = blocksize;
    page.bytes = calloc(1, blocksize);
    return page;
}
Page Page_create_from_bytes(uint8_t* bytes,uint64_t size){
    Page page={0};
    page.cap=size;
    page.bytes = calloc(1,size);
    memcpy(page.bytes, bytes, size);
    return page;
}

int Page_getint(Page *page,uint64_t offset){
    return *(int*)(&(page->bytes[offset]));
}
void Page_setint(Page *page,uint64_t offset, int n){
    int res=sd_assert((offset+sizeof(n)) <= page->cap);
    if (!res) exit(1);
    memcpy(&page->bytes[offset], &n, sizeof(n));
    page->len+=sizeof(n);
}

uint8_t* Page_getbytes(Page *page,int offset){
    uint32_t len = Page_getint(page,offset);
    uint8_t* bytes = calloc(1,len);
    memcpy(bytes, &page->bytes[offset+sizeof(len)], len);
    return bytes;
}
void Page_setbytes(Page *page,uint64_t offset, uint8_t* bytes, uint32_t size){
    int res=sd_assert((offset+sizeof(size) + size) <= page->cap);
    if (!res) exit(1);
    Page_setint(page, offset, size);
    offset+=sizeof(size);
    memcpy(&page->bytes[offset],bytes,size);
    page->len += size+sizeof(size);
}

char* Page_getstring(Page *page,uint64_t offset){
    uint8_t* bytes = Page_getbytes(page,offset);
    return (char*)bytes;
}
void Page_setstring(Page *page,uint64_t offset, const char* str){
    Page_setbytes(page,offset,(uint8_t*)str,strlen(str)+1);
}

