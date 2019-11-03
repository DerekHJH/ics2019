//This can be deleted because we are only using expr for checking purpose
#include<stdlib.h>
#include "nemu.h"
#include<regex.h>
#include<sys/types.h>
#include "../include/monitor/expr.h"

int init_monitor(int, char *[]);
void ui_mainloop(int);


int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

 	//is_batch_mode=false;
  /* Receive commands from user. */
	//printf("tell me what us is_batch_mode %d\n",is_batch_mode);
  ui_mainloop(is_batch_mode);





	//To test the expression. This part can be deleted
/*	uint32_t Result=0;
 	char buf[63356];
	FILE *fp=fopen("/home/hjh/ics2019/nemu/src/tmp/input.txt","r");
  bool Success=true;
	if(fp==NULL)
	{
		printf("File open failed!!\n");
		assert(0);
	}
	while(!feof(fp))
	{
		Success=fscanf(fp,"%u %[^\n]\n",&Result,buf);
		printf("Read: %u %s\n",Result,buf);
		printf("Ture result: %u    Calculated result:%u\n",Result,expr(buf,&Success));
	}
  fclose(fp);
*/

  return 0;
}
