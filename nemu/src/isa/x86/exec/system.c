#include "cpu/exec.h"

void raise_intr(uint32_t,vaddr_t);
make_EHelper(lidt) 
{
	//begin{hjh}
  cpu.IDTR.limit=vaddr_read(id_dest->addr,2);
  if(id_dest->width==2)cpu.IDTR.base=vaddr_read(id_dest->addr+2,4)&0xffffff;
	else if(id_dest->width==4)cpu.IDTR.base=vaddr_read(id_dest->addr+2,4);
	else panic("lidt has met undefined operand length\n");
  //end{hjh}
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

  difftest_skip_ref();
}

make_EHelper(int) 
{

	//printf("the return address is 0x%x and the cpu.pc is 0x%x\n",decinfo.seq_pc,cpu.pc);
  raise_intr(id_dest->val,decinfo.seq_pc);//hjh
  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) 
{
  rtl_pop(&s0);
	rtl_pop(&cpu.cs);
	rtl_pop(&cpu.eflags.val);
	rtl_j(s0);
  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

make_EHelper(in) 
{
	//begin{hjh}
	switch(id_src->width)
	{
    case 1:
		{
      id_dest->val=pio_read_b(id_src->val);
			break;
		}
    case 2:
		{
      id_dest->val=pio_read_w(id_src->val);
			break;
		}
    case 4:
		{
      id_dest->val=pio_read_l(id_src->val);
			break;
		}
	}
	operand_write(id_dest,&id_dest->val);
	//end{hjh}

  print_asm_template2(in);
}

make_EHelper(out) 
{
	//begin{hjh}
	switch(id_src->width)
	{
    case 1:
		{
      pio_write_b(id_dest->val,id_src->val);
			break;
		}
    case 2:
		{
      pio_write_w(id_dest->val,id_src->val);
			break;
		}
    case 4:
		{
      pio_write_l(id_dest->val,id_src->val);
			break;
		}
	}
	//end{hjh}

  print_asm_template2(out);
}
