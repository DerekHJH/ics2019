#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int tag[128]=
{
/*0x00*/	0,0,0,0,
/*0x04*/  0,0,0,0,
/*0x08*/  0,0,0,0,
/*0x0c*/  0,0,0,0,
/*0x10*/	0,0,0,0,
/*0x14*/  0,0,0,0,
/*0x18*/  0,0,0,0,
/*0x1c*/  0,0,0,0,
/*0x20*/	0,0,0,0,
/*0x24*/  0,0,0,0,
/*0x28*/  0,0,0,0,
/*0x2c*/  0,0,0,0,
/*0x30*/	0,0,0,0,
/*0x34*/  0,0,0,0,
/*0x38*/  0,0,0,0,
/*0x3c*/  0,0,0,0,
/*0x40*/	1,0,1,0,
/*0x44*/  1,1,1,0,
/*0x48*/  0,0,0,0,
/*0x4c*/  0,0,0,1,
/*0x50*/	0,0,1,0,
/*0x54*/  0,0,0,1,
/*0x58*/  0,0,0,0,
/*0x5c*/  0,0,0,0,
/*0x60*/	1,0,1,1,
/*0x64*/  1,1,1,0,
/*0x68*/  1,0,0,0,
/*0x6c*/  1,1,1,0,
/*0x70*/	0,0,0,1,
/*0x74*/  1,0,0,1,
/*0x78*/  0,0,0,0,
/*0x7c*/  0,0,0,0

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
	int len=0;
  while((*fmt)!='\0')
	{
    while((*fmt)!='\0'&&(*fmt)!='%')
		{
			*(out+len)=*fmt;
			fmt++;
      len++;
		}
		int l=0,tempd=0;//if it is an integer;
		char tempc[128]={'\0'};//temporary string;
		char *temps=NULL;//if it is a string
		int p=0;//used to traverse tempc;
		while(*(fmt+l)!='\0'&&((int)(*(fmt+l))>127||tag[(int)(*(fmt+l))]!=1))l++;//*(fmt+l)=the spefifier
    switch(*(fmt+l))
		{
			case 'd':
			{ 
        tempd=va_arg(ap,int);
        for(p=0;tempd>0;p++)
				{
					tempc[p]=(char)(tempd%10+'0');
					tempd/=10;
				}				
				p--;
				while(p>=0)
				{
					*(out+len)=tempc[p];
					p--;
					len++;
				}
        break;
			}
			case 's':
			{
				p=0;
        temps=va_arg(ap,char*);
				while(temps[p]!='\0')
				{
					*(out+len)=temps[p];
					p++;
					len++;
				}
        break;
			}
		}
    if(*(fmt+l)!='\0')fmt+=l+1;
		else break;
	}
	*(out+len)='\0';
	len++;
  //end{hjh}
	return len;
}

int sprintf(char *out, const char *fmt, ...) 
{
	int len=0;
  va_list ap;
  va_start(ap,fmt);
	vsprintf(out,fmt,ap);
	va_end(ap);
  return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) 
{
	printf("this fuction does not seem to be realized in stdio.c\n");
  return 0;
}

#endif
