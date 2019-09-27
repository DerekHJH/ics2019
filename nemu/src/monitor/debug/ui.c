#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
void isa_reg_display(void);
void info_w();//Self added
void wp_d(int);//Selfadded
void wp_d_all();//Selfadded
void wp_new(char *, uint32_t);//Selfadded

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args) {
 	if(args==NULL)
	{
    cpu_exec(1);
	}
	else
	{
  	char *Args=strtok(args," ");
  	int len=strlen(Args),N=0;
    
	  for(int i=0;i<len;i++)
  	{
    	N=N+(int)Args[i]-(int)('0');
	  }
		cpu_exec(N);
	}
	
	return 0;
}

static int cmd_info(char *args)
{
  char *Args=strtok(args," ");
	if(strcmp(Args,"r")==0)isa_reg_display();
	else if(strcmp(Args,"w")==0)
	{
    info_w();    
	}
  return 0;	
}

static int cmd_x(char *args)
{
	char *Args=strtok(args," ");
  int N=0,len=strlen(Args);
	uint32_t temp=0,EXPR=0x100000;
	bool *Success=malloc(sizeof(bool));
	for(int i=0;i<len;i++)
	{
		N=N+(int)Args[i]-(int)('0');
	}

	Args=strtok(NULL, " ");
	EXPR=expr(Args, Success);
	for(int i=1;i<=N;i++)
	{
    temp=paddr_read(EXPR,4);
		printf("0x%08x:    0x%08x    %u\n",EXPR,temp,temp);
		EXPR=EXPR+4;
	}

	return 0;
}
static int cmd_d(char *args)
{
  if(args==NULL)wp_d_all();
  else 
	{
		int len=strlen(args),Number=0;
		for(int i=0;i<len;i++)
		Number=Number*10+(int)(args[i]-'0');
		wp_d(Number);
	}
	return 0;
}
static int cmd_p(char *args)
{
	bool *Success=malloc(sizeof(bool));
  printf("%u\n", expr(args, Success));
	return 0;
}
static int cmd_w(char *args)
{
	bool *Success=malloc(sizeof(bool));
  wp_new(args, expr(args, Success));
	return 0;
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si", "Run N successive instructions of the programm and then halt, with default N=1. We can add an integer argument N after si", cmd_si },
	{"info", "Print certain information corresponding to the argument, with 'r'--registers info and 'w'--watchpoint info", cmd_info},
	{"x", "x N EXPR. Print the information stored at given address denoted by the EXPR 4bytes a time. Print N times/groups", cmd_x},
	{"d", "Delete watchpoints", cmd_d},
	{"p", "Print the value of the following exprssion", cmd_p},
	{"w", "Set a watchpoint", cmd_w}
 	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
