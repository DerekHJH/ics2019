#include "nemu.h"

paddr_t page_translate(vaddr_t addr)
{
  vaddr_t d=(addr>>22)&0x3ff;
	vaddr_t t=(addr>>12)&0x3ff;
	vaddr_t o=addr&0xfff;
	PDE pde=(PDE)paddr_read((cpu.cr3.page_directory_base<<12)+(d<<2),4);
	if(pde.present==0)
	{
		printf("OOPS! pde->present==0!!!\n");
		assert(0);
	}
	PTE pte=(PTE)paddr_read((pde.page_frame<<12)+(t<<2),4);
	if(pte.present==0)
	{
		printf("OOPS! pte->present==0!!!\n");
		assert(0);
	}
	return (paddr_t)((pte.page_frame<<12)+o);
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) 
{
	if(cpu.cr0.paging==1)
	{
    if(0)
	  {
	  	printf("data cross the page boundary, and we deal with it later!\n");
	  	assert(0);
	  }
	  else
  	{
  	  paddr_t paddr=page_translate(addr);	
      return paddr_read(paddr, len);
  	}
	}
	else return paddr_read(addr,len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) 
{
	if(cpu.cr0.paging==1)
	{


	}
	else paddr_write(addr, data, len);
}
