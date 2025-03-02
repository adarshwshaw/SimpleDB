#include "test/tests.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
 

typedef struct {
    uint32_t len,cap;
    int *data;
} datas;

datas d={0};

void setup(){
    for (int i=0;i<8;i++){
	 array_append(&d, i);
    }
}

_Bool test0(){

    if (d.len != 8){
	fprintf(stderr, "len: %u\n",d.len);
	return 1;
    }

    for (int i=0;i<(int)d.len;i++){
	if ( d.data[i] != i){
	    fprintf(stderr, "%d but got %d",d.data[i],i);
	    return 1;
	}
    }
    return 0;
}
	
_Bool test1(){
    array_append(&d, 8);
    if (d.len != 9){
	fprintf(stderr, "len: %u\n",d.len);
	return 1;
    }

    if (d.cap != 16){
	fprintf(stderr, "cap: %u\n",d.cap);
	return 1;
    }
    for (int i=0;i<(int)d.len;i++){
	if ( d.data[i] != i){
	    fprintf(stderr, "%d but got %d",d.data[i],i);
	    return 1;
	}
    }
    return 0;
}

int main(int argc, char** argv){
    TestFunc f[]={test0,test1};
    setup();
    execute_test(argc, argv, f);
}
