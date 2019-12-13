#include "nemu.h"

paddr_t page_translate(vaddr_t addr)
{
  vaddr_t d=(addr>>22)&0x3ff;
	vaddr_t t=(addr>>12)&0x3ff;
	vaddr_t o=addr&0xfff;
	PDE pde=(PDE)paddr_read((cpu.cr3.page_directory_base<<12)+(d<<2),4);
  //begin{test}
	//printf("the cpu.cr3 is 0x%x\n",cpu.cr3.page_directory_base);
	
	/*vaddr_t temp=(cpu.cr3.page_directory_base<<12)+4;
  PDE haha=(PDE)paddr_read(temp,4);
	printf("the content of the first entry of the page directory is 0x%x\n",haha.val);
	PTE xixi=(PTE)paddr_read((haha.page_frame<<12),4);
  printf("the content of the first entry of the page table is 0x%x\n",xixi.val);	*/
	//end{test}
	if(pde.present==0)
	{
		printf("OOPS! pde->present==0!!!And the pde is 0x%x\n",pde.val);
		printf("And the addr is 0x%x\n",addr);
    printf("And the d is 0x%x\n",d);
    printf("And the t is 0x%x\n",t);
    printf("And the o is 0x%x\n",o);
		assert(0);
	}
	PTE pte=(PTE)paddr_read((pde.page_frame<<12)+(t<<2),4);

	if(pte.present==0)
	{
		printf("OOPS! pte->present==0!!!And the pde is 0x%x and the pte is 0x%x\n",pde.val,pte.val);
		assert(0);
	}
	return (paddr_t)((pte.page_frame<<12)+o);
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) 
{
	if(cpu.cr0.paging==1)
	{
    if((addr&0xfff)+len>0x1000)
	  {
	    int len1=0x1000-(addr&0xfff);	
			int len2=len-len1;
			vaddr_t addr2=addr+len1;
			paddr_t paddr1=page_translate(addr);
			paddr_t paddr2=page_translate(addr2);
			return (paddr_read(paddr2,len2)<<(len2<<3))|paddr_read(paddr1,len1);
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
    if((addr&0xfff)+len>0x1000)
    {
    	int len1=0x1000-(addr&0xfff);	
      int len2=len-len1;
      vaddr_t addr2=addr+len1;
      paddr_t paddr1=page_translate(addr);
      paddr_t paddr2=page_translate(addr2);
      paddr_write(paddr1,data&((1<<(len1<<3))-1),len1);
			paddr_write(paddr2,data>>(len1<<3),len2);
    }
    else
    {
      paddr_t paddr=page_translate(addr);	
      paddr_write(paddr, data, len);
    }

	}
	else paddr_write(addr, data, len);
}
