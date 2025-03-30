#include "logmgr.h"
#include "block.h"
#include "ds.h"
#include "filemgr.h"
#include "page.h"
#include <stdint.h>
#include <string.h>


static BlockId appendNewBlock(LogMgr *lm){
    BlockId blk = FileMgr_append(lm->fm, lm->logfile);
    Page_setint(&lm->page, 0, lm->fm->blockSize);
    FileMgr_write(lm->fm, blk, lm->page);
    return blk;
}

static void flush(LogMgr* lm){
    FileMgr_write(lm->fm, lm->currentblk, lm->page);
    lm->lastSavedLSN = lm->latestLSN;
}
LogMgr LogMgr_create(FileMgr* fm,char* logfile){
    LogMgr lm={0};
    lm.fm= fm;
    strncpy(lm.logfile, logfile, 127);
    lm.page = Page_create(lm.fm->blockSize);
    int logsize = FileMgr_len(lm.fm,lm.logfile);
    if (!logsize){
	lm.currentblk = appendNewBlock(&lm);
    }else{
	lm.currentblk = blockId_create(lm.logfile, logsize-1);
	FileMgr_read(lm.fm, lm.currentblk, lm.page);
    }
    return lm;
}


int LogMgr_append(LogMgr* lm, uint8_t* rec, int recsize){
    int boundary = Page_getint(&lm->page, 0);
    int bytesneeded = recsize+sizeof(int);
    if((boundary - bytesneeded) < (int)sizeof(int)){
	flush(lm);
	lm->currentblk = appendNewBlock(lm);
	boundary = Page_getint(&lm->page, 0);
    }
    int recpos = boundary- bytesneeded;
    ByteArray ba = {.len = recsize, .data = (uint8_t*)rec};
    Page_setbytes(&lm->page, recpos, ba);
    Page_setint(&lm->page, 0, recpos);
    lm->latestLSN += 1;
    return lm->latestLSN;
}

void LogMgr_flush(LogMgr* lm, int lsn){
    if (lm->lastSavedLSN <= lsn){
	flush(lm);
    }	
}

static void moveToBlock(LogIterator* li,BlockId blk){
    FileMgr_read(li->fm,blk, li->p);
    li->boundary = Page_getint(&li->p, 0);
    li->curpos = li->boundary;
}

LogIterator LogIter_create(FileMgr* fm, BlockId blk){
    LogIterator li={0};
    li.fm=fm;
    li.blk=blk;
    li.p = Page_create(fm->blockSize);
    moveToBlock(&li,li.blk);
    return li;
}
ByteArray LogIter_next(LogIterator* li){
    if (li->curpos == li->fm->blockSize){
	li->blk = blockId_create(li->blk.filename, li->blk.blknum -1);
	moveToBlock(li,li->blk);
    }
    ByteArray rec= Page_getbytes(&li->p, li->curpos);
    li->curpos += Page_maxLength(rec.len);
    return rec;
}
int LogIter_hasnext(LogIterator* li){
    return (li->curpos < li->fm->blockSize) || (li->blk.blknum > 0);
}
