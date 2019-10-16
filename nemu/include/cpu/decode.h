#ifndef __CPU_DECODE_H__
#define __CPU_DECODE_H__

#include "common.h"

#define make_DHelper(name) void concat(decode_, name) (vaddr_t *pc)
typedef void (*DHelper) (vaddr_t *);

#define OP_STR_SIZE 40
enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM };

typedef struct {
  uint32_t type;  //OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM 
  int width;
  union {
    uint32_t reg;  //It stores the index of the register
    rtlreg_t addr; //like MAR, RTL. It stores the address from which the value is extracted.
    uint32_t imm; //It stores the value of an immediate number. When addr is used, this block of memory is replaced by imm
    int32_t simm; //the same as imm, with just one difference --- it is a signed value.
  };
  rtlreg_t val;  //like MDR, RTL. It stores one of reg, imm or simm
  char str[OP_STR_SIZE];
} Operand;

#include "isa/decode.h"

typedef struct {
  uint32_t opcode;
  uint32_t width;
  vaddr_t seq_pc;  // sequential pc, changed with the execution and is used to update pc
  bool is_jmp; 
  vaddr_t jmp_pc;
  Operand src, dest, src2;
  struct ISADecodeInfo isa;
} DecodeInfo;

void operand_write(Operand *, rtlreg_t *);

/* shared by all helper functions */
extern DecodeInfo decinfo;

#define id_src (&decinfo.src)
#define id_src2 (&decinfo.src2)
#define id_dest (&decinfo.dest)  
//Directly use these three to avoid long variable decinfo.xxx

#ifdef DEBUG
#define print_Dop(...) snprintf(__VA_ARGS__)
#else
#define print_Dop(...)
#endif

#endif
