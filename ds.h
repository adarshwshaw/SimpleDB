#ifndef __DS__
#define __DS__

#include <stdint.h>
#include <stdlib.h>
typedef struct {
    uint8_t* data;
    uint64_t len;
} ByteArray;

ByteArray ByteArray_create(uint64_t len);
void ByteArray_free(ByteArray ba);


#endif
