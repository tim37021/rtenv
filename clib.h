#ifndef CLIB_H
#define CLIB_H
#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n);
int strcmp(const char *a, const char *b) __attribute__ ((naked));
int strncmp(const char *a, const char *b, size_t n);
size_t strlen(const char *s) __attribute__ ((naked));
char *strncat(char *, const char *, size_t n);

int fprintf(int, const char *, ...);


/*
Implement a Non-standard itoa
the function will not add '\0' at the end

*/
size_t itoa(char *, int);

#endif
