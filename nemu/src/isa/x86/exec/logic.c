#include "cpu/exec.h"
#include "cc.h"

make_EHelper(rol)
{
  //begin{hjh}
  s0=id_src->val&0x1F;
  while(s0!=0)
	{
		s1=id_dest->val&1u;
		rtl_shri(&id_dest->val,&id_dest->val,1);
		if(s1==1)
		{
      id_dest->val|=(1u<<(id_dest->width*8-1));
		}
    s0--;
	}	
  rtl_set_CF(&s1);
	if(id_src->val==1)
	{
		s1=s1^((id_dest->val>>(id_dest->width*8-2))&0x1);
	}
	rtl_set_OF(&s1);
	operand_write(id_dest,&id_dest->val);
	//printf("eflags: OF is %u, ZF is %u, SF is %u, CF is %u\n",cpu.eflags.OF,cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.CF);
  
  print_asm_template2(rol);
	//end{hjh}
}


make_EHelper(test) {
  rtl_and(&s0,&id_dest->val,&id_src->val);
	s1=0;
	rtl_set_OF(&s1);
	rtl_set_CF(&s1);
	rtl_update_ZFSF(&s0,id_dest->width);
//	printf("eflags: OF is %u, ZF is %u, SF is %u, CF is %u\n",cpu.eflags.OF,cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.CF);
  print_asm_template2(test);
}

make_EHelper(and) {
  //begin{hjh} 
  rtl_and(&s0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&s0);	
	s1=0;
	rtl_set_OF(&s1);
	rtl_set_CF(&s1);
	rtl_update_ZFSF(&s0,id_dest->width);
 // printf("eflags: OF is %u, ZF is %u, SF is %u, CF is %u\n",cpu.eflags.OF,cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.CF);
	//end{hjh}
  print_asm_template2(and);
}

make_EHelper(xor) {
  //begin{hjh}
	rtl_xor(&s0,&id_dest->val,&id_src->val);
	operand_write(id_dest,&s0);
	s1=0;
	rtl_set_OF(&s1);
	rtl_set_CF(&s1);
  rtl_update_ZFSF(&s0,id_dest->width);
//	printf("eflags: OF is %u, ZF is %u, SF is %u, CF is %u\n",cpu.eflags.OF,cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.CF);
	//end{hjh}
  print_asm_template2(xor);
}

make_EHelper(or) {
  //begin{hjh}
	rtl_or(&s0,&id_dest->val,&id_src->val);
	operand_write(id_dest,&s0);
	s1=0;
	rtl_set_OF(&s1);
	rtl_set_CF(&s1);
  rtl_update_ZFSF(&s0,id_dest->width);
//	printf("eflags: OF is %u, ZF is %u, SF is %u, CF is %u\n",cpu.eflags.OF,cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.CF);
	
	//end{hjh}

  print_asm_template2(or);
}

make_EHelper(sar) 
{
	//begin{hjh}
  int32_t temp=(int32_t)id_dest->val;
	temp>>=(id_src->val);
	s0=(uint32_t)temp;
	operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);
  // unnecessary to update CF and OF in NEMU
	//printf("eflags: OF is %u, ZF is %u, SF is %u, CF is %u\n",cpu.eflags.OF,cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.CF);
  //end{hjh}


  print_asm_template2(sar);
}

make_EHelper(shl) 
{
	//begin{hjh}
  id_src->val&=0x1F;
	s0=(id_dest->val)<<(id_src->val);
	operand_write(id_dest,&s0);
	rtl_update_ZFSF(&s0,id_dest->width);
  // unnecessary to update CF and OF in NEMU
 // printf("eflags: OF is %u, ZF is %u, SF is %u, CF is %u\n",cpu.eflags.OF,cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.CF);
  //end{hjh}
  print_asm_template2(shl);
}

make_EHelper(shr) 
{  
  // unnecessary to update CF and OF in NEMU
  //begin{hjh}
  id_src->val&=0x1F;
  s0=(id_dest->val)>>(id_src->val);
  operand_write(id_dest,&s0);
  rtl_update_ZFSF(&s0,id_dest->width);
 // printf("eflags: OF is %u, ZF is %u, SF is %u, CF is %u\n",cpu.eflags.OF,cpu.eflags.ZF,cpu.eflags.SF,cpu.eflags.CF);
	//end{hjh}
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  //begin{hjh}
  rtl_not(&s0,&id_dest->val);
  operand_write(id_dest,&s0);
	//end{hjh}
  print_asm_template1(not);
}
