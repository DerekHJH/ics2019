#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
	rtl_push(&cpu.eflags.val);
	rtl_push((rtlreg_t *)&cpu.cs);
	rtl_push(&ret_addr);
  vaddr_t Term=(NO<<3)+cpu.IDTR.base;
	vaddr_t JAddress=vaddr_read(Term,2)|(vaddr_read(Term+6,2)<<16);
  rtl_j(JAddress);
	//printf("0x%08x\n",JAddress);
}

bool isa_query_intr(void) {
  return false;
}
