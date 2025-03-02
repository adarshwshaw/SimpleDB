#include "page.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"

static Page page;
void setup(Page* page){
    *page= Page_create(100l);
    Page_setint(page, 8, 15);
}
bool test1(void){

    return Page_getint(&page,0)==0?0:1;
}

bool test2(void){
    return Page_getint(&page,8)==15?0:1;
}

bool test3(void){
    Page_setstring(&page, 0, "adarsh");
    Page_setstring(&page, page.len, "adarsh");
    char* str = Page_getstring(&page,0);
    return strcmp("adarsh", str);
}

int main(int argc, char** argv){
    TestFunc f[]={test1,test2,test3};
    setup(&page);
    execute_test(argc, argv, f);
}
