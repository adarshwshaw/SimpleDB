#ifndef __BUFFER__
#define __BUFFER__

#include "block.h"
#include "filemgr.h"
#include "logmgr.h"
#include "page.h"

typedef struct {
    FileMgr* fm;
    LogMgr* lm;
    Page contents;
    BlockId blk;
    int pins, txnums, lsn;
} Buffer;

Buffer Buffer_create(FileMgr* fm, LogMgr* lm);
void Buffer_setModified(Buffer* buf, int txnum, int lsn);

#define Buffer_isPinned(buf) (((buf)->pin > 0)?1:0)

void Buffer_assignToBlock(Buffer* buf,BlockId b);

#define Buffer_pin(buf) (buf)->pin++
#define Buffer_unpin(buf) (buf)->pin--



#endif
