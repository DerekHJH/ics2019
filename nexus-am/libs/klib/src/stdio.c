#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

typedef char * (*Tagexec)(char *); 
struct TAGS
{
	int speci;
	Tagexec exec;

}tag[128]=
{
/*0x00*/	{0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x04*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x08*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x0c*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x10*/	{0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x14*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x18*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x1c*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x20*/	{0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x24*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x28*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x2c*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x30*/	{0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x34*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x38*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x3c*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x40*/	{1,NULL},{0,NULL},{1,NULL},{0,NULL},
/*0x44*/  {1,NULL},{1,NULL},{1,NULL},{0,NULL},
/*0x48*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x4c*/  {0,NULL},{0,NULL},{0,NULL},{1,NULL},
/*0x50*/	{0,NULL},{0,NULL},{1,NULL},{0,NULL},
/*0x54*/  {0,NULL},{0,NULL},{0,NULL},{1,NULL},
/*0x58*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x5c*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x60*/	{1,NULL},{0,NULL},{1,NULL},{1,NULL},
/*0x64*/  {1,NULL},{1,NULL},{1,NULL},{0,NULL},
/*0x68*/  {1,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x6c*/  {1,NULL},{1,NULL},{1,NULL},{0,NULL},
/*0x70*/	{0,NULL},{0,NULL},{1,NULL},{0,NULL},
/*0x74*/  {1,NULL},{0,NULL},{0,NULL},{1,NULL},
/*0x78*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL},
/*0x7c*/  {0,NULL},{0,NULL},{0,NULL},{0,NULL}

};
//0 stands for symbols not defined
//1 stands for specifiers
//2 stands for



int printf(const char *fmt, ...) 
{
	printf("this fuction does not seem to be realized in stdio.c\n");
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) 
{
  //begin{hjh}
  while(*fmt!='\0')
	{
    while(*fmt!='\0'&&*fmt!='%')
		{
			*out=*fmt;
			fmt++;
		}
		int l=0;
		while(*(fmt+l)='\0'&&tag[(int)(*(fmt+l))]->speci!=1)l++;//*(fmt+l)=the spefifier
    tag[(int)*(fmt+l)]->exec();
	}
  //end{hjh}
	return 0;
}

int sprintf(char *out, const char *fmt, ...) 
{
  va_list ap;
  va_start(ap,fmt);
	vsprintf(out);
	va_end(ap);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) 
{
	printf("this fuction does not seem to be realized in stdio.c\n");
  return 0;
}

#endif
