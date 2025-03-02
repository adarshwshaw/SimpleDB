#ifndef  __BLOCKID__
#define __BLOCKID__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char filename[64];
    uint32_t blknum;
} BlockId;

BlockId blockId_create(const char* filename, uint32_t blknum);

bool blockId_equals(BlockId bid,void* obj);

char* blockId_toString(BlockId bid, char* buffer);

#endif
