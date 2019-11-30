#include "common.h"
#include "syscall.h"
int sys_yield()
{
	_yield();
	return 0;
}

int sys_write(int fd,char *buf,int cnt)
{
  if(fd==1||fd==2)
	{
		for(int i=0;i<cnt;i++)
		{
			_putc(*(buf+i));
		}
	}
	else
	{
    printf("ohohoh!!!\n");
	}
	return cnt;
}
int sys_brk()
{
	return 0;
}

_Context* do_syscall(_Context *c) 
{
  uintptr_t a[4];
  a[0]=c->GPR1;
	a[1]=c->GPR2;
  a[2]=c->GPR3;
	a[3]=c->GPR4;

  switch (a[0]) 
	{
  	case SYS_exit:
		{
      //printf("I have reached here in do_syscall SYS_exit!!!\n");
      _halt(a[1]);
      break;
		}
		case SYS_yield:
		{
      //printf("I have reached here in do_syscall SYS_yield!!!\n");
      c->GPRx=sys_yield();
		  break;		
		}
		case SYS_brk:
		{
      c->GPRx=sys_brk();
			break;
		}
		case SYS_write:
		{	
			//printf("a1 is %d and a2 is %d and a3 is %d\n",a[1],a[2],a[3]);
			c->GPRx=sys_write((int)a[1],(char *)a[2],(int)a[3]);
			//printf("the cnt is %d\n",c->GPRx);
      //printf("I have reached here in do_syscall SYS_write!!!\n");
			break;
		}
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
