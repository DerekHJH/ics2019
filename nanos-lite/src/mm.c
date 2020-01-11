#include "memory.h"
#include "proc.h"
extern PCB *current;//hjh
static void *pf = NULL;

void* new_page(size_t nr_page) 
{
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) 
{
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) 
{

	//printf("the current->max_brk is 0x%x",current->max_brk);
	//printf("brk is 0x%x\n",brk);
	//printf("current->brk is %08d\n",current->max_brk);
	void *pa=NULL;
  while(brk>current->max_brk)
	{
    pa=new_page(1);
		_map(&(current->as),(void *)current->max_brk,pa,1);
		current->max_brk+=PGSIZE;
	}	
  return 0;
}

void init_mm() 
{
	//printf("current->max_brk is %d\n",current->max_brk);
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);
}
