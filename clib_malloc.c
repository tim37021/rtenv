#include "clib.h"
#include "syscall.h"
#include <stdint.h>
/*
   Each block contains size(uint16_t) and data(array)
   if size is less than zero, means available
 */

#define SIZEOF_SIZE sizeof(int16_t)
#define TYPEOF_SIZE int16_t

static unsigned char *last_available_mem=NULL;

void *malloc(size_t n){
	static unsigned char *first_available_mem=NULL;
	if(first_available_mem==NULL){
		last_available_mem=sbrk(0);
		first_available_mem=last_available_mem+1;
	}
	/*Search for available mem*/
	signed char *p=first_available_mem;
	while(p<=last_available_mem){
		/*Negative means available*/
		if(*(TYPEOF_SIZE *)p<0){
			if(-*(TYPEOF_SIZE *)p>=n)
				break;
			p+=-*(TYPEOF_SIZE *)p+SIZEOF_SIZE;
		}else
			p+=*(TYPEOF_SIZE *)p+SIZEOF_SIZE;
	}

	if(p>last_available_mem){
		void *tmp=sbrk(n+SIZEOF_SIZE);
		if((int)tmp==-1)
			return NULL;
		last_available_mem=tmp;
		*((TYPEOF_SIZE *)(last_available_mem-n-(SIZEOF_SIZE-1)))=n;
		return last_available_mem-n+1;
	}else{
		*(TYPEOF_SIZE *)p=-*(TYPEOF_SIZE *)p;
		return p+SIZEOF_SIZE;
	}


}

void free(void *mem){
	if(*(TYPEOF_SIZE *)(mem-SIZEOF_SIZE)>0)
		*(TYPEOF_SIZE *)(mem-SIZEOF_SIZE)=-*(TYPEOF_SIZE *)(mem-SIZEOF_SIZE);
}
