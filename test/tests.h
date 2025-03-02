#ifndef __TESTS__
#define __TESTS__

#include <stdlib.h>
typedef _Bool (*TestFunc)(void);

#define num_test(f) (int)(sizeof(f)/sizeof(*f))

#define validate_test_id(testid, f) \
    do{ \
	if (testid >= num_test(f)){ \
	    fprintf(stderr,"out of bound"); \
	    return 1; \
	}\
    }while(0)

int get_test_id(int argc,char** argv){
    if (argc != 2){
	return 1;
    }
    int testid = atoi(argv[1]);
    return testid;
}

#define execute_test(argc,argv,f) \
    do { \
	int testid=get_test_id(argc, argv);\
	validate_test_id(testid, f);\
	return (f)[testid]();\
    } while(0);


#endif
