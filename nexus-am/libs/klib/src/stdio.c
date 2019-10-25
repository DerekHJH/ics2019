#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) 
{
	panic("this fuction does not seem to be realized in stdio.c\n");
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) 
{
	panic("this fuction does not seem to be realized in stdio.c\n");
  return 0;
}

int sprintf(char *out, const char *fmt, ...) 
{
	panic("this fuction does not seem to be realized in stdio.c\n");
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) 
{
	panic("this fuction does not seem to be realized in stdio.c\n");
  return 0;
}

#endif
