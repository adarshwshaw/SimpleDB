#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include "filemgr.h"
#include "page.h"


int main(int argc, char** argv){
    (void)argc;
    (void)argv;
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
    printf("offset %d contains %d" ,pos2,Page_getint(&p2,pos2));
    printf("offset %d contains %s" ,pos1,Page_getstring(&p2,pos1));
    return 0;
}
