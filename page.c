#include "page.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ds.h"
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
    page.bytes =bytes;
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

ByteArray Page_getbytes(Page *page,int offset){
    uint32_t len = Page_getint(page,offset);
    ByteArray ba = ByteArray_create(len);
    memcpy(ba.data, &page->bytes[offset+sizeof(len)], len);
    return ba;
}
void Page_setbytes(Page *page,uint64_t offset, ByteArray ba){
    int res=sd_assert((offset+sizeof(uint32_t) + ba.len) <= page->cap);
    if (!res) exit(1);
    Page_setint(page, offset, ba.len);
    offset+=sizeof(uint32_t);
    memcpy(&page->bytes[offset],ba.data,ba.len);
    page->len += ba.len+sizeof(uint32_t);
}

char* Page_getstring(Page *page,uint64_t offset){
    ByteArray bytes = Page_getbytes(page,offset);
    return (char*)bytes.data;
}
void Page_setstring(Page *page,uint64_t offset, const char* str){
    ByteArray ba = {.len = strlen(str)+1, .data=(uint8_t*)str};
    Page_setbytes(page,offset,ba);
}

