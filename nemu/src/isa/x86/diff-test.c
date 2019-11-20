#include "nemu.h"
#include "monitor/diff-test.h"

#include "monitor/monitor.h"

extern NEMUState nemu_state;
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) 
{
	if(cpu.eax!=ref_r->eax)
	{
		printf("Fuck!cpu.eax is 0x%x, while ref_f.eax is 0x%x\n",cpu.eax,ref_r->eax);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
	if(cpu.ecx!=ref_r->ecx)
	{
		printf("Fuck!cpu.ecx is 0x%x, while ref_f.ecx is 0x%x\n",cpu.ecx,ref_r->ecx);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
	if(cpu.edx!=ref_r->edx)
	{
		printf("Fuck!cpu.edx is 0x%x, while ref_f.edx is 0x%x\n",cpu.edx,ref_r->edx);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
	if(cpu.ebx!=ref_r->ebx)
	{
		printf("Fuck!cpu.ebx is 0x%x, while ref_f.ebx is 0x%x\n",cpu.ebx,ref_r->ebx);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
	if(cpu.esp!=ref_r->esp)
	{
		printf("Fuck!cpu.esp is 0x%x, while ref_f.esp is 0x%x\n",cpu.esp,ref_r->esp);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
	if(cpu.ebp!=ref_r->ebp)
	{
		printf("Fuck!cpu.ebp is 0x%x, while ref_f.ebp is 0x%x\n",cpu.ebp,ref_r->ebp);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
	if(cpu.esi!=ref_r->esi)
	{
		printf("Fuck!cpu.esi is 0x%x, while ref_f.esi is 0x%x\n",cpu.esi,ref_r->esi);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
	if(cpu.edi!=ref_r->edi)
	{
		printf("Fuck!cpu.edi is 0x%x, while ref_f.edi is 0x%x\n",cpu.edi,ref_r->edi);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
	if(cpu.pc!=ref_r->pc)
	{
		printf("Fuck!cpu.epc is 0x%x, while ref_f.pc is 0x%x\n",cpu.pc,ref_r->pc);
		printf("Detect difference in registers!!\n");
		nemu_state.state=NEMU_STOP;
		return false;
	}
  return true;
}

void isa_difftest_attach(void) {
}
