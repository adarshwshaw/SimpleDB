#ifndef __FILEMGR__
#define __FILEMGR__
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>
#include "block.h"
#include "page.h"

#ifdef _WIN64
    typedef HANDLE SD_FILE;
#endif

typedef struct {
    SD_FILE file;
    char* fileName;
    uint32_t len;
} FM_OpenFile;

typedef struct {
    uint32_t len;
    uint32_t cap;
    FM_OpenFile *data;
} FM_OpenFiles;
    
typedef struct {
    char dbDirNm[256];
    int32_t blockSize;
    bool isNew;
    FM_OpenFiles files;
} FileMgr;

FileMgr FileMgr_create(const char* dbdir, int32_t blocksize);
void FileMgr_read(FileMgr *mgr,BlockId blk, Page p);
void FileMgr_write(FileMgr *mgr,BlockId blk, Page p);
BlockId FileMgr_append(FileMgr *mgr,const char* fileName);

uint64_t FileMgr_len(FileMgr *mgr,const char* fileName);
SD_FILE FileMgr_getfile(FileMgr *mgr,const char* fileName);

#endif

