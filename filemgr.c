#include "filemgr.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"
#include "ds.h"
#include "utils.h"

#ifdef _WIN64
    #include <windows.h>

    #define sd_fseek(file, loffset, starting_point) SetFilePointer((file),(loffset),NULL,(starting_point))
#endif

static uint32_t FM_fread(void* dest, uint32_t len, SD_FILE file){
    uint32_t bytes_read = 0;
    #ifdef _WIN64
    ReadFile(file, dest, (DWORD)len, (LPDWORD) &bytes_read,NULL);
    sd_assert(bytes_read > 0);
    #else
    bytes_read=fread(dest, 1, len, file);
    #endif
    return bytes_read;
}


static uint32_t FM_fwrite(void* buf, uint32_t len, SD_FILE file){
    uint32_t bytes_read = 0;
    #ifdef _WIN64
    WriteFile(file, buf, (DWORD) len, (LPDWORD) &bytes_read, NULL);
    sd_assert(bytes_read > 0);
    #else
    bytes_read=fwrite(buf, 1, len, file);
    #endif
    return bytes_read;
}

static FM_OpenFiles list(const char* dirName){

    FM_OpenFiles out = {0};
    
    char search_name[256]={0};
    strcpy(search_name, dirName);
    strcpy(search_name+strlen(dirName), "\\*");

    WIN32_FIND_DATA ffd;
    HANDLE hfind = INVALID_HANDLE_VALUE;

    hfind = FindFirstFileA(search_name,&ffd);
    if (INVALID_HANDLE_VALUE == hfind){
	printf("error in first\n");
	exit(1);
    }
    do {
	if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
	    //pass;
	}
	else{
	    FM_OpenFile file = {0};
	    file.fileName = _strdup(ffd.cFileName);
	    file.len = strlen(file.fileName);
	    array_append(&out, file);
	}
    }while(FindNextFileA(hfind, &ffd)!=0);
    return out;

}

FileMgr FileMgr_create(const char* dbdir, int32_t blocksize){
    FileMgr mgr={0};
    strcpy(mgr.dbDirNm, dbdir);
    mgr.blockSize=blocksize;
    if(CreateDirectoryA(mgr.dbDirNm,NULL)){
	DWORD error = GetLastError();
	if (error == ERROR_ALREADY_EXISTS){
	    mgr.isNew=false;
	}else if(error > 0) {
	    char msg[1096];
	    size_t sz= FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, msg, sizeof(msg),NULL);
	    printf("sz=%zu Error Code : %ld\n",sz,error);
	    printf("Error: %s",msg);
	    exit(1);
	}
    }else{
	mgr.isNew=true;
    }
    FM_OpenFiles f= list(mgr.dbDirNm);
    printf("files: %u\n",f.len);
    for (uint32_t i =0;i<f.len;i++){
	printf("%.*s\n",f.data[i].len, f.data[i].fileName);
    }
    return mgr;
}

void FileMgr_read(FileMgr *mgr,BlockId blk, Page p){
    SD_FILE f = FileMgr_getfile(mgr,blk.filename);
    int offset = blk.blknum * mgr->blockSize;
    sd_fseek(f, offset, SEEK_SET);
    FM_fread(p.bytes, mgr->blockSize, f);
}
void FileMgr_write(FileMgr *mgr,BlockId blk, Page p){

    SD_FILE f = FileMgr_getfile(mgr,blk.filename);
    int offset = blk.blknum * mgr->blockSize;
    sd_fseek(f, offset, SEEK_SET);
    FM_fwrite(p.bytes, mgr->blockSize, f);
}
BlockId FileMgr_append(FileMgr *mgr,const char* fileName){
    int newblknum = FileMgr_len(mgr, fileName);
    BlockId blk = blockId_create(fileName, newblknum);
    ByteArray ba = ByteArray_create(mgr->blockSize);
    SD_FILE file= FileMgr_getfile(mgr, fileName);
    sd_fseek(file, blk.blknum*mgr->blockSize, 0);
    FM_fwrite(ba.data, ba.len, file);
    return blk;
}

uint64_t FileMgr_len(FileMgr *mgr,const char* fileName){
    SD_FILE file = FileMgr_getfile(mgr, fileName);
    uint64_t len_in_bytes = GetFileSize(file, NULL);
    uint64_t len_in_blocks = len_in_bytes/mgr->blockSize;
    return len_in_blocks;
}
SD_FILE FileMgr_getfile(FileMgr *mgr,const char* fileName){
    FM_OpenFile curr={0};
    int flag=1;
    for (uint32_t i=0;i<mgr->files.len;i++){
	if (strcmp(fileName, mgr->files.data[i].fileName)==0){
	    curr=mgr->files.data[i];
	    flag=0;
	    break;
	}
    }
    if (flag){
	FM_OpenFile f = {
	    .len = strlen(fileName),
	    .fileName = _strdup(fileName),
	    .file = NULL
	};
	array_append(&mgr->files, f);
	curr=f;
    }
    if (curr.file == NULL){

	char fullpath[256]={0};
	strcpy(fullpath, mgr->dbDirNm);
	int t = strlen(mgr->dbDirNm);
	fullpath[t++]='/';
	strcpy(fullpath+t, curr.fileName);
	curr.file = CreateFileA(fullpath, 
	     GENERIC_READ|GENERIC_WRITE,
	     0, NULL, OPEN_ALWAYS,
	     FILE_FLAG_RANDOM_ACCESS|FILE_FLAG_WRITE_THROUGH,
	     NULL);
	mgr->files.data[mgr->files.len-1].file=curr.file;
	sd_assert(curr.file != INVALID_HANDLE_VALUE);
    }
    return curr.file;
}
