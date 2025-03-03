#include "page.h"
#include "tests.h"
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "filemgr.h"

_Bool test0(){

    FileMgr fm = FileMgr_create("mydb",200);
    BlockId blk = blockId_create("testfile.db", 0);
    Page p1 = Page_create(fm.blockSize);
    int pos1 = 88;
    Page_setstring(&p1,pos1, "abcdefghijklm");
    int size = strlen("abcdefghijklm")+1+sizeof(uint32_t);
    int pos2 = pos1 + size;
    Page_setint(&p1, pos2, 345);
    FileMgr_write(&fm, blk, p1);
    Page p2 = Page_create(fm.blockSize);
    FileMgr_read(&fm, blk, p2);
    int ret = sd_assert(345 == Page_getint(&p2, pos2));
    if (!ret) return ret;
    ret = sd_assert(strcmp("abcdefghijklm", Page_getstring(&p2,pos1))==0);
    return ret?0:1;
}

int main(int argc,char** argv){
    TestFunc f[]={test0};
    execute_test(argc, argv, f);
}
