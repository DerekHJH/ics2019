#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];
//Self added
char *get_expr(int Depth)
{
	char *temp1=(char *)malloc(65536);
	char *ans=(char *)malloc(65536);
	if(Depth>4)
	{
		sprintf(ans,"%d",rand()%500);
		return ans;
	}
	switch(rand()%3)
	{
		case 0:
			sprintf(ans,"%d",rand()%500);
			return ans;
		case 1:
			temp1=get_expr(Depth+1);
    	ans[0]='(',ans[1]='\0';
			strcat(ans,temp1);
			strcat(ans,")");
			return ans;
		default:
			ans=get_expr(Depth+1);
			temp1=get_expr(Depth+1);
			switch(rand()%4)
			{
				case 0:
					strcat(ans,"+");
					strcat(ans,temp1);
					return ans;          
				case 1:
      		strcat(ans,"-");
      		strcat(ans,temp1);
      		return ans;       
      	case 2:
	      	strcat(ans,"*");
	      	strcat(ans,temp1);
	      	return ans;       
      	default:
	      	strcat(ans,"/");
	      	strcat(ans,temp1);
	      	return ans;       
			}
	}
}

static inline void gen_rand_expr() 
{
  buf[0]='\0';
  strcat(buf,get_expr(0));	 
  return;
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }

 //Self added
  FILE *haha=fopen("/home/hjh/ics2019/nemu/src/tmp/input.txt","w");
  if(haha==NULL)
  {
   	printf("File open failed at gen-expr.c line %d\n",__LINE__);
    assert(0);
	}

  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    //Self added if but fscanf is original
    if(fscanf(fp, "%d", &result)==0)
		{
			printf("File read failed at gen-expr line %d\n",__LINE__);
			assert(0);
		}

    pclose(fp);

		//Change printf to fprintf
    fprintf(haha,"%u %s\n", result, buf);
  }

	fclose(haha);//Self added
  return 0;
}
