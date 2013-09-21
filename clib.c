#include "clib.h"
#include "stm32f10x.h"
#include "RTOSConfig.h"
#include <stdarg.h>
#include "syscall.h"


int strcmp(const char *a, const char *b)
{
	asm(
        "strcmp_lop:                \n"
        "   ldrb    r2, [r0],#1     \n"
        "   ldrb    r3, [r1],#1     \n"
        "   cmp     r2, #1          \n"
        "   it      hi              \n"
        "   cmphi   r2, r3          \n"
        "   beq     strcmp_lop      \n"
		"	sub     r0, r2, r3  	\n"
        "   bx      lr              \n"
		:::
	);
}


size_t strlen(const char *s)
{
	asm(
		"	sub  r3, r0, #1			\n"
        "strlen_loop:               \n"
		"	ldrb r2, [r3, #1]!		\n"
		"	cmp  r2, #0				\n"
        "   bne  strlen_loop        \n"
		"	sub  r0, r3, r0			\n"
		"	bx   lr					\n"
		:::
	);
}

char *strncat(char *str1, const char *str2, size_t n){
	int start=strlen(str1);
	char *p;
	for(p=&str1[start]; p-&str1[start]<n; ++p){
		*p=str2[p-&str1[start]];
		if(*p=='\0') break;
	}
}


/*%d %s %c supported*/
int fprintf(int fd, const char *format, ...){
	va_list v1;
	va_start(v1, format);

	int i=0, cur_char=0;
	int argtmpi, tmp2;
	char *argtmpcp, argtmpc;
	char buffer[512]={0};
	while(format[i]!='\0'){
		if(format[i]=='%'){
			switch(format[i+1]){
				case '%':
					buffer[cur_char++]='%'; i+=2; continue;
				case 'd':
					/*to keep it simple, we flush the buffer*/
					buffer[cur_char]='\0';
					write(fd, buffer, cur_char+1);
					cur_char=0;

					argtmpi=va_arg(v1, int);
					tmp2=itoa(buffer, argtmpi);
					write(fd, buffer, tmp2+1);
					i+=2; continue;
					
				case 's':
					/*to keep it simple, we flush the buffer*/
					buffer[cur_char]='\0';
					write(fd, buffer, cur_char+1);
					cur_char=0;
					argtmpcp=va_arg(v1, char *);
					write(fd, argtmpcp, strlen(argtmpcp)+1);
					i+=2; continue;
				case 'c':
					argtmpc=va_arg(v1, int);
					buffer[cur_char++]=argtmpc;
					i+=2; continue;
				default:;
			}
		}else
			buffer[cur_char++]=format[i];
		if(cur_char>=480){
			buffer[cur_char]='\0';
			write(fd, buffer, cur_char+1);
			cur_char=0;
		}
		++i;
	}
	buffer[cur_char]='\0';
	write(fd, buffer, cur_char+1);

	va_end(v1);
}

size_t itoa(char *buf, int number){
	char tmp[20]={0}; int i=0;
	if(number==0){memcpy(buf,"0",2); return 1;}
	while(number!=0){
		tmp[i++]='0'+(number%10);
		number/=10;
	}
	size_t len=strlen(tmp);
	/*Reversing the string*/
	for(i=len-1; i>=0; --i){
		buf[len-1-i]=tmp[i];
	}
	buf[len]='\0';
	return len;
}

/*
void puts(const char *s)
{
	while (*s) {
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		 ;
		USART_SendData(USART2, *s);
		s++;
	}
}*/
