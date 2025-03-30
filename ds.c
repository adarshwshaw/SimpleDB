#include "ds.h"
#include <stdlib.h>


ByteArray ByteArray_create(uint64_t len){
    ByteArray ba = { .data =0, .len = len};
    ba.data = calloc(sizeof(*ba.data), len);
    return ba;
}

void ByteArray_free(ByteArray ba){
    if (ba.len > 0 && ba.data != NULL){
	free(ba.data);
	ba.data=NULL;
	ba.len=0;
    }else{
	exit(1);
    }
}
