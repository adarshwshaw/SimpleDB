#include "buffer.h"
#include "filemgr.h"
#include "logmgr.h"
#include "page.h"


Buffer Buffer_create(FileMgr* fm, LogMgr* lm){
    Buffer buf={0};
    buf.fm=fm;
    buf.lm=lm;
    buf.txnums=-1;
    buf.lsn=-1;
    buf.contents= Page_create(fm->blockSize);
    return buf;
}
void Buffer_setModified(Buffer* buf, int txnum, int lsn){
    buf->txnums= txnum;
    if (buf->lsn>0)
	buf->lsn=lsn;
}

static void flush(Buffer* buf){
    if(buf->txnums > 0){
	LogMgr_flush(buf->lm, buf->lsn);
	FileMgr_write(buf->fm, buf->blk, buf->contents);
	buf->txnums = -1;
    }
}
void Buffer_assignToBlock(Buffer* buf,BlockId b){
    flush(buf);
    buf->blk=b;
    FileMgr_read(buf->fm, buf->blk, buf->contents);
    buf->pins=0;
}
