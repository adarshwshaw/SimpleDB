#include "block.h"
#include <stdio.h>

#include "tests.h"

bool test1(){
    BlockId bid = blockId_create("table.db", 1);
    BlockId oid = blockId_create("table.db", 1);
    char buf[128] = {0};

    printf("%s\n", blockId_toString(bid, buf));
    bool f = blockId_equals(bid, &oid);
    return !f;
}


int main(int argc, char **argv) {
    TestFunc f[]={test1};
    execute_test(argc, argv, f);
}
