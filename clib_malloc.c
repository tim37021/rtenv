#include "clib.h"
#include "syscall.h"
/*
   Each block contains size(BYTE) and data(array)
   if size is less than zero, means available
 */

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
		if(*p<0){
			if(-*p>=n)
				break;
			p+=-*p+1;
		}else
			p+=*p+1;
	}

	if(p>last_available_mem){
		void *tmp=sbrk(n+1);
		if((int)tmp==-1)
			return NULL;
		last_available_mem=tmp;
		*(last_available_mem-n)=n;
		return last_available_mem-n+1;
	}else{
		*p=-*p;
		return p+1;
	}


}

void free(void *mem){
	if(*((signed char *)mem-1)>0)
		*((signed char *)mem-1)=-*((signed char *)mem-1);
}
