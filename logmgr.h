#ifndef __LOGMGR__
#define __LOGMGR__

#include "block.h"
#include "ds.h"
#include "filemgr.h"
#include "page.h"
#include <stdint.h>

typedef struct {
    FileMgr* fm;
    char logfile[128];
    Page page;
    BlockId currentblk;
    int latestLSN;
    int lastSavedLSN;
} LogMgr;

typedef struct {
    FileMgr *fm;
    BlockId blk;
    Page p;
    int32_t curpos;
    int32_t boundary;
} LogIterator;

LogMgr LogMgr_create(FileMgr* fm,char* logfile);

int LogMgr_append(LogMgr* lm, uint8_t* rec, int recsize);

void LogMgr_flush(LogMgr* lm, int lsn);

LogIterator LogIter_create(FileMgr* fm, BlockId blk);
ByteArray LogIter_next(LogIterator* li);
int LogIter_hasnext(LogIterator* li);

#endif
