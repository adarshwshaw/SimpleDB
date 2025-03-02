#include <string.h>
#include "block.h"
#include <stdio.h>

BlockId blockId_create(const char* filename, uint32_t blknum){
    BlockId bid = {0};
    memcpy(bid.filename, filename, 63);
    bid.blknum=blknum;
    return bid;
}

bool blockId_equals(BlockId bid,void* obj){
    BlockId *oid = (BlockId*)obj;
    return (strcmp(bid.filename, oid->filename) == 0) && bid.blknum == oid->blknum;
}

char* blockId_toString(BlockId bid, char* buffer){
    sprintf(buffer, "[file %s, block %u]", bid.filename,bid.blknum);
    return buffer;
}
