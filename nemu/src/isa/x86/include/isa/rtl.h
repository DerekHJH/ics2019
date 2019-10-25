#ifndef __X86_RTL_H__
#define __X86_RTL_H__

#include "rtl/rtl.h"

/* RTL pseudo instructions */

static inline void rtl_lr(rtlreg_t* dest, int r, int width) {
  switch (width) {
    case 4: rtl_mv(dest, &reg_l(r)); return;
    case 1: rtl_host_lm(dest, &reg_b(r), 1); return;
    case 2: rtl_host_lm(dest, &reg_w(r), 2); return;
    default: assert(0);
  }
}

static inline void rtl_sr(int r, const rtlreg_t* src1, int width) {
  switch (width) {
    case 4: rtl_mv(&reg_l(r), src1); return;
    case 1: rtl_host_sm(&reg_b(r), src1, 1); return;
    case 2: rtl_host_sm(&reg_w(r), src1, 2); return;
    default: assert(0);
  }
}

static inline void rtl_push(const rtlreg_t* src1) {
  // esp <- esp - 4
  // M[esp] <- src1
	//begin{hjh}
  rtl_subi(&cpu.esp,&cpu.esp,4);
  rtl_sm(&cpu.esp,src1,4); 
	//end{hjh}
}

static inline void rtl_pop(rtlreg_t* dest) {
  // dest <- M[esp]
  // esp <- esp + 4
  //begin{hjh}
	rtl_lm(dest,&cpu.esp,4);
  rtl_addi(&cpu.esp,&cpu.esp,4);
	//end{hjh}
}

static inline void rtl_is_sub_overflow(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 - src2)
  //begin{hjh}
  rtl_msb(&t0,src1,width);
  rtl_msb(&t1,src2,width);
	rtl_msb(dest,res,width);
	if(t0!=t1&&(*dest)!=t0)(*dest)=1;
	else (*dest)=0;
	//end{hjh}
}

static inline void rtl_is_sub_carry(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_carry(src1 - src2)
	uint64_t mask=(1lu<<(width*8));
  uint64_t a=(*src1)&(mask-1);
	uint64_t b=(~(*src2)+1)&(mask-1);
	uint64_t c=a+b;
  if(c&mask)(*dest)=1;
	else (*dest)=0;
  printf("a is 0x%lx, b is 0x%lx, and c is 0x%lx, and width is %d\n",a,b,c,width);

}

static inline void rtl_is_add_overflow(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 + src2)
	// begin{hjh}
  rtl_msb(&t0,src1,width);
  rtl_msb(&t1,src2,width);
	rtl_msb(dest,res,width);
	if(t0==t1&&(*dest)!=t0)(*dest)=1;
	else (*dest)=0;
	//end{hjh}
}

static inline void rtl_is_add_carry(rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_carry(src1 + src2)
	// begin{hjh}
  uint64_t mask=(1lu<<(width*8));
  uint64_t a=(*src1)&(mask-1);
  uint64_t b=(*src2)&(mask-1);
  uint64_t c=a+b;
  if(c&mask)(*dest)=1;
	else (*dest)=0;

	printf("a is 0x%lx, b is 0x%lx, and c is 0x%lx, and width is %d\n",a,b,c,width);
	//end{hjh}
}

#define make_rtl_setget_eflags(f) \
  static inline void concat(rtl_set_, f) (const rtlreg_t* src) { \
    cpu.eflags.f=*src; \
  } \
  static inline void concat(rtl_get_, f) (rtlreg_t* dest) { \
    *dest=cpu.eflags.f; \
  }

make_rtl_setget_eflags(CF)
make_rtl_setget_eflags(OF)
make_rtl_setget_eflags(ZF)
make_rtl_setget_eflags(SF)

static inline void rtl_update_ZF(const rtlreg_t* result, int width) {
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
	cpu.eflags.ZF=1; //hjh
	if(width==4)
	{
		if(*result!=0)cpu.eflags.ZF=0;
	}
	else if((*result)&((1u<<(width*8))-1))cpu.eflags.ZF=0; //hjh
}

static inline void rtl_update_SF(const rtlreg_t* result, int width) {
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  cpu.eflags.SF=0;//hjh
	if(((*result)>>(width*8-1))&1)cpu.eflags.SF=1;//hjh
}

static inline void rtl_update_ZFSF(const rtlreg_t* result, int width) {
  rtl_update_ZF(result, width);
  rtl_update_SF(result, width);
}

#endif
