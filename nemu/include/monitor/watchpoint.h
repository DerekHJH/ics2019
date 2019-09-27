#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

#define MAX_EXPR_LEN 1024
typedef struct watchpoint {
  int NO;
	uint32_t ori_val,seq_val,vaddr;
	char buf[MAX_EXPR_LEN];
  struct watchpoint *next;

  /* TODO: Add more members if necessary */


} WP;

#endif
