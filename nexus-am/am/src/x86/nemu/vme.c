#include <am.h>
#include <x86.h>
#include <nemu.h>
#include "klib.h"
#define PG_ALIGN __attribute((aligned(PGSIZE)))

static PDE kpdirs[NR_PDE] PG_ALIGN = {};
static PTE kptabs[(PMEM_SIZE + MMIO_SIZE) / PGSIZE] PG_ALIGN = {};
static void* (*pgalloc_usr)(size_t) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static _Area segments[] = 
{      // Kernel memory mappings
  {.start = (void*)0,          .end = (void*)PMEM_SIZE},
  {.start = (void*)MMIO_BASE,  .end = (void*)(MMIO_BASE + MMIO_SIZE)}
};

#define NR_KSEG_MAP (sizeof(segments) / sizeof(segments[0]))
int _vme_init(void* (*pgalloc_f)(size_t), void (*pgfree_f)(void*)) 
{

	//printf("the address of kpdirs and kptabs are 0x%x, and 0x%x\n",kpdirs,kptabs);

  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  int i;

  // make all PDEs invalid
  for (i = 0; i < NR_PDE; i ++) 
	{
    kpdirs[i] = 0;
  }

  PTE *ptab = kptabs;
  for (i = 0; i < NR_KSEG_MAP; i ++) 
	{
		//printf("the segments.end is 0x%x",segments[0].end);
    uint32_t pdir_idx = (uintptr_t)segments[i].start / (PGSIZE * NR_PTE);
    uint32_t pdir_idx_end = (uintptr_t)segments[i].end / (PGSIZE * NR_PTE);
    for (; pdir_idx < pdir_idx_end; pdir_idx ++) 
		{
      // fill PDE
      kpdirs[pdir_idx] = (uintptr_t)ptab | PTE_P;

      // fill PTE
      PTE pte = PGADDR(pdir_idx, 0, 0) | PTE_P;
      PTE pte_end = PGADDR(pdir_idx + 1, 0, 0) | PTE_P;
      for (; pte < pte_end; pte += PGSIZE) 
			{
        *ptab = pte;
        ptab ++;
      }
    }
  }

  set_cr3(kpdirs);
  set_cr0(get_cr0() | CR0_PG);
  vme_enable = 1;

  return 0;
}

int _protect(_AddressSpace *as) 
{
  PDE *updir = (PDE*)(pgalloc_usr(1));
  as->ptr = updir;
  // map kernel space
  for (int i = 0; i < NR_PDE; i ++) 
	{
    updir[i] = kpdirs[i];
  }

  return 0;
}

void _unprotect(_AddressSpace *as) 
{
}

static _AddressSpace *cur_as = NULL;
void __am_get_cur_as(_Context *c) 
{
  c->as = cur_as;
}

void __am_switch(_Context *c) 
{
  if (vme_enable) 
	{
		//printf("haha\n");
    set_cr3(c->as->ptr);
    cur_as = c->as;
		//printf("the cur_as->ptr is 0x%x\n",cur_as->ptr);
  }
}

int _map(_AddressSpace *as, void *va, void *pa, int prot) 
{
	uint32_t d=((uint32_t)va>>22)&0x3ff;
	//printf("d is 0x%x\n",d);
	uint32_t t=((uint32_t)va>>12)&0x3ff;
  uint32_t *pde=(uint32_t *)((uint32_t)(as->ptr)+(d<<2));
	//printf("previously the *pde is 0x%x\n",*pde);
  if(((*pde)&(PTE_P))==0)
	{
    (*pde)=(uint32_t)pgalloc_usr(1)|PTE_P;
	}	
  uint32_t *pte=(uint32_t *)(((*pde)&0xfffff000)+(t<<2));
  (*pte)=((uint32_t)pa&0xfffff000)|PTE_P;
	//printf("the va is 0x%x and the pa is 0x%x and the pde is 0x%x and the pte is 0x%x and the as->ptr is 0x%x and the *pde is 0x%x and the *pte is 0x%x\n",va,pa,pde,pte,as->ptr,*pde,*pte);
  return 0;
}

_Context *_ucontext(_AddressSpace *as, _Area ustack, _Area kstack, void *entry, void *args) 
{
	int Mainsize=sizeof(int)+2*sizeof(char *);
  void *Stackptr=ustack.end-Mainsize;
	memset(Stackptr,0,Mainsize);
  _Context *c=(_Context *)Stackptr-1;
	memset(c,0,sizeof(_Context));
	c->eip=(uintptr_t)entry;
	c->cs=0x8;
	c->eflags=0x2;//Other students set it as 1<<9;
	c->as=as;
  return c;
}
