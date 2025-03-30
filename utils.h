#ifndef __UTILS__
#define __UTILS__

/*
* dynamic array defination:
* typedef struct {
* 	uint32_t len;
* 	uint32_t cap;
* 	<type> *data;
* } <array_type_name>;
*
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 8

#define array_append(arr, val) \
    do{ \
	if ((arr)->cap == 0){\
	    (arr)->data = calloc(DEFAULT_SIZE, sizeof(*(arr)->data)); /* allocate new array*/ \
	    (arr)->cap=DEFAULT_SIZE;\
	} else if ((arr)->len == (arr)->cap){\
	    void* new_data = calloc((arr)->cap * 2, sizeof(*(arr)->data));/* reallocate */\
	    if (new_data == NULL){ \
		printf("Error: unable to reallocate\n"); \
		exit(1); \
	    } \
	    memmove(new_data, (arr)->data, (arr)->cap*sizeof(*(arr)->data)); \
	    free((arr)->data); \
	    (arr)->data=new_data; \
	    (arr)->cap *= 2; \
	} \
	(arr)->data[(arr)->len++] = (val); \
    } while(0)

#ifdef NDEBUG
    #define tst_debug(...)
#else
    #include <stdio.h>
    #define tst_debug(...) printf(__VA_ARGS__)
#endif
    
#define sd_assert(e) ((e) ? (1) : \
 (tst_debug("%s,%d: assertion ’%s’ failed\n", \
 __FILE__, __LINE__, #e), 0)) 

#endif
