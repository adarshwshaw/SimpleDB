#include "logmgr.h"

#include "ds.h"
#include "filemgr.h"
#include "page.h"
#include  "tests.h"
#include "utils.h"
#include <stdio.h>

static void printLogRecords(LogMgr *lm,const char* str){
    printf("%s", str);
    LogIterator li = LogIter_create(lm->fm, lm->currentblk);
    while(LogIter_hasnext(&li)){
	ByteArray ba = LogIter_next(&li);
	Page p = Page_create_from_bytes(ba.data, ba.len); 
	char* str = Page_getstring(&p, 0);
	int npos = Page_maxLength(strlen(str)+1);
	int val = Page_getint(&p, npos);
	printf("[ %s , %d ]\n",str,val);
	free(str);
	ByteArray_free(ba);
    }
}
static ByteArray createLogRecord(char* str, int val){
    int npos = Page_maxLength(strlen(str)+1);
    ByteArray ba = ByteArray_create(npos+sizeof(val));
    Page p = Page_create_from_bytes(ba.data, ba.len);
    Page_setstring(&p, 0, str);
    Page_setint(&p, npos, val);
    return ba;
}

static void createRecords(LogMgr *lm, int start,int end){
    printf("Creating Records\n");
    for (int i=start;i<=end;i++){
	char temp[64]={0};
	sprintf(temp, "record%d", i);
	ByteArray rec = createLogRecord(temp, i+100);
	int lsn = LogMgr_append(lm, rec.data, rec.len);
	printf("LSN:%d\n",lsn);
	// ByteArray_free(rec);
    }
}
_Bool test0(){
    FileMgr fm = FileMgr_create("mydb", 200);
    LogMgr lm = LogMgr_create(&fm, "db.log");
    createRecords(&lm,1,35);
    printLogRecords(&lm,"This log file now has these records:\n");
    createRecords(&lm,36,70);
    LogMgr_flush(&lm,65);
    printLogRecords(&lm,"This log file now has these records:\n");
    return 0;
}


int main(int argc, char** argv){
    TestFunc f[]={test0};
    execute_test(argc, argv, f);
}
