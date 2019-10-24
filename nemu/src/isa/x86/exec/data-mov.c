#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) 
{
	//printf("hjh id_dest->val is 0x%x\n",id_dest->val);
  rtl_push(&id_dest->val);
  
  print_asm_template1(push);
}

make_EHelper(pop) 
{
  rtl_pop(&id_dest->val);
	operand_write(id_dest,&id_dest->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) 
{
	//begin{hjh}	
	rtl_mv(&cpu.esp,&cpu.ebp);
  rtl_pop(&cpu.ebp);
  //end{hjh}

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) 
	{
    rtl_msb(&s0,&cpu.eax,2);
	  if(s0)cpu.edx|=0xFFFFu;	
		else cpu.edx&=0xFFFF0000u;
  }
  else 
	{
    rtl_msb(&s0,&cpu.eax,4);
	  if(s0)rtl_li(&cpu.edx,0xFFFFFFFFu);	
		else rtl_li(&cpu.edx,0u);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);

	//printf("the id_dest->width is %d and the id_src->width is %d and the id_dest->val is 0x%x and the id_src->val is 0x%x\n",id_dest->width,id_src->width,id_dest->val,id_src->val);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
