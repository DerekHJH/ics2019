#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#define  DEFAULT_ENTRY (uint8_t *)0x8048000;
int fs_read(int fd,void *buf,size_t len);//hjh
size_t ramdisk_read(void *, size_t, size_t);//hjh
int fs_open(const char *path,int flags,int mode);//hjh
size_t getopen(int fd);
size_t getsize(int fd);
size_t getdisk(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) 
{

	//printf("the filename is %s\n",filename);
	/*
	int fd=fs_open(filename,0,0);
	int Base=getdisk(fd);
	//printf("fd is %d\n",fd);
  Elf_Ehdr ELFheader;
  Elf_Phdr Proheader;	
	//printf("getdisk(fd) is %d",getdisk(fd));
  ramdisk_read(&ELFheader,Base,sizeof(Elf_Ehdr));
	//for(int i=0;i<16;i++)
  //printf("%02x ",ELFheader.e_ident[i]);
  //printf("The e_phoff is 0x%08x\n",ELFheader.e_phoff);
	//printf("The e_phnum is 0x%08x\n",ELFheader.e_phnum);
	//printf("The size of ELFheader  is 0x%d\n",sizeof(Elf_Ehdr));
	for(int i=0;i<ELFheader.e_phnum;i++)
	{
		//printf("The offset of the header term is %x\n",ELFheader.e_phoff+i*ELFheader.e_phentsize);
		ramdisk_read(&Proheader,Base+ELFheader.e_phoff+i*ELFheader.e_phentsize,ELFheader.e_phentsize);
	  if(Proheader.p_type==PT_LOAD)
    {
      ramdisk_read((void *)Proheader.p_vaddr,Base+Proheader.p_offset,Proheader.p_memsz);
			memset((void *)(Proheader.p_vaddr+Proheader.p_filesz),0,Proheader.p_memsz-Proheader.p_filesz);
		}
	}
	//printf("the entryyyy is 0x%x\n",ELFheader.e_entry);
  return ELFheader.e_entry;
	*/

	//Rewrite ramdisk!!!
	int fd=fs_open(filename,0,0);
  int fsize=getsize(fd);   
	void *va=(void *)DEFAULT_ENTRY;
	void *pa=NULL;
	while(fsize>0)
	{
    //printf("the filesize is %d\n",fsize);
		pa=new_page(1);
		_map(&(pcb->as),va,pa,1);
    fs_read(fd,pa,PGSIZE);
		fsize-=PGSIZE;
		va+=PGSIZE;
	  //printf("the pa is 0x%x\n",pa);
	  //printf("the va is 0x%x\n",va);	
	}
	return (uintptr_t)DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) 
{
	//printf("I am entering file %s",filename);
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %08x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) 
{
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) 
{
	_protect(&(pcb->as));
  uintptr_t entry = loader(pcb, filename);
  //printf("the entry is 0x%x\n",entry);
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
