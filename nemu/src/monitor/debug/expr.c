#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

//Self added
//in order to use atoi 
#include<stdlib.h>
//in order to use isa_reg_str2val
uint32_t isa_reg_str2val(char *,bool *);

enum {
  TK_NOTYPE = 256, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, 
	TK_LP, TK_RP, 
	TK_EQ, TK_NEQ, TK_LE, TK_L, TK_GE, TK_G, 
	TK_AND, TK_OR, TK_NOT,
	TK_DIGNUM, TK_HEXNUM, TK_REG,
  TK_DEREF //Originally, all * is regarded as the multipication sign and shall be identified as dereference sign later.

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", TK_PLUS},    // plus
	{"-", TK_MINUS},    //minus
	{"\\*", TK_MUL},    //multiply
	{"/", TK_DIV},  	//divide

	{"\\(", TK_LP},    //left parenthsis
	{"\\)", TK_RP},    //right parenthesis
  
  {"==", TK_EQ},    // equal
	{"!=", TK_NEQ},    //not equal
	{"<=", TK_LE},    //less or equal
	{"<", TK_L},    //less
	{">=", TK_GE},    //greater or equal
	{">", TK_G},    //greater
	{"&&", TK_AND},    //and
	{"\\|\\|", TK_OR},    //or
	{"!", TK_NOT},    //not

	{"0x[0-9a-fA-F]*", TK_HEXNUM},    //hex number  
	{"[0-9]+", TK_DIGNUM},   //digital numbers
	{"\\$[a-z]*", TK_REG},    //registers

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type,preced;
  uint32_t num;
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
         

				//There is no check for over large nr_token
				if(rules[i].token_type!=TK_NOTYPE)
				{
          tokens[++nr_token].type=rules[i].token_type;
					tokens[nr_token].num=0;
					tokens[nr_token].preced=0;
					switch(rules[i].token_type)
					{
						case TK_OR:
						{
              tokens[nr_token].preced=1;
							break;
						}                               
						case TK_AND:
						{
              tokens[nr_token].preced=2;
							break;
						}                            
						case TK_EQ:
						{
              tokens[nr_token].preced=3;
							break;
						}                            
						case TK_NEQ:
						{
              tokens[nr_token].preced=3;
							break;
						}                            
						case TK_L:
						{
              tokens[nr_token].preced=4;
							break;
						}                            
						case TK_LE:
						{
              tokens[nr_token].preced=4;
							break;
						}                            
						case TK_G:
						{
              tokens[nr_token].preced=4;
							break;
						}                            
						case TK_GE:
						{
              tokens[nr_token].preced=4;
							break;
						}                            
						case TK_PLUS:
						{
							tokens[nr_token].preced=5;
              break;
						}
						case TK_MINUS:
						{
							tokens[nr_token].preced=5;
							break;
						}                             
						case TK_MUL:
						{
              tokens[nr_token].preced=6;
              if(nr_token==1||!(tokens[nr_token-1].type==TK_RP||tokens[nr_token-1].type==TK_DIGNUM||tokens[nr_token-1].type==TK_REG||tokens[nr_token-1].type==TK_HEXNUM))
							{
								printf("%d %u\n",tokens[nr_token-1].type,tokens[nr_token-1].num);
								tokens[nr_token].type=TK_DEREF;
								tokens[nr_token].preced=21;
							}
              break;
						}
						case TK_DIV:
						{
							tokens[nr_token].preced=6;
							break;
						}
						case TK_NOT:
						{
							tokens[nr_token].preced=21;
							break;
						}                            
						case TK_DIGNUM:
						{
					    uint32_t pow10=1;
						  for(int i=substr_len-1;i>=0;i--)
					  	{
						   	tokens[nr_token].num+=((*(substr_start+i))-'0')*pow10;
						   	pow10*=10;
					   	}
							break;
						}
						case TK_HEXNUM:
						{
							tokens[nr_token].type=TK_DIGNUM;
							uint32_t pow16=1;
              for(int i=substr_len-1;i>=2&&(*(substr_start+i))!='x';i--)
					  	{
					  		char temp=*(substr_start+i);
					  		if(temp>='a'&&temp<='f')tokens[nr_token].num+=(temp-'a'+10)*pow16;
					  		else if(temp>='0'&&temp<='9')tokens[nr_token].num+=(temp-'0')*pow16;
					  		else 
					  		{
					  			printf("Invalid hex number!!!\n");
					  			assert(0);
					  		}
					  		pow16*=16;
					  	} 
              break;
						}
						case TK_REG:
						{
							bool *Success=malloc(sizeof(bool));
							char temp[4];
							for(int i=1;i<=3;i++)
							temp[i-1]=*(substr_start+i);
							temp[3]='\0';
 			  			tokens[nr_token].type=TK_DIGNUM;
 			  			tokens[nr_token].num=isa_reg_str2val(temp, Success);
 			  			if(!*Success) 
 			  			{
 			  				printf("Failed to extract value from regesters!!\n");
 			  				assert(0);
 			  			}
              break;
						}							



					}
		  	}

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


bool check_parenthsis(int l,int r)
{
  if(!(tokens[l].type==TK_LP&&tokens[r].type==TK_RP))return false;
	else
	{
		int cnt=0;
		for(int i=l+1;i<r;i++)
		{
		  if(tokens[i].type==TK_LP)cnt++;
			else if(tokens[i].type==TK_RP)cnt--;
			if(cnt<0)return false;
		}	
	}
	return true;
}	 

uint32_t eval(int l,int r)
{
  if(l>r)
	{
   	printf("Bad expresion in debug at line %d\n",__LINE__);
  	assert(0);                                                 
  }
	else if(l==r)
	{
		if(tokens[l].type!=TK_DIGNUM)
		{
			printf("Bad expresion in debug at line %d\n",__LINE__);
			assert(0);                                                 
		}
		else return tokens[l].num;
	}
	else if(check_parenthsis(l,r))
	{
		return eval(l+1,r-1);
	}
	else
	{	
    int op=0,k=r,Min=10000;
    for(op=r;op>=l;op--)
			if(tokens[op].type==TK_RP)
			{
				int cnt=1;
				do
				{
					op--;
					if(tokens[op].type==TK_LP)cnt--;
					else if(tokens[op].type==TK_RP)cnt++;
				}while(op>=l&&cnt>0);
				if(op<l)
				{
        	printf("Unmathched right parenthesis!!\n");
        	assert(0);                                                 
				}
			}
      else if(tokens[op].type==TK_LP)
			{
				printf("Unmatched left parenthesis!!\n");
				assert(0);
			}	
		  else if(tokens[op].type!=TK_DIGNUM)
			{
        if(tokens[op].preced<Min)
				{
					Min=tokens[op].preced;
					k=op;
				}				
			}

		op=k;
		uint32_t val1=0,val2=0;
		if(tokens[op].preced<=20)
		{
		  val1=eval(l,op-1);
		  val2=eval(op+1,r);
		}
		else
		{
      val2=eval(op+1,r);
		}

		switch(tokens[op].type)
		{
			case TK_OR:return val1||val2;
			case TK_AND:return val1&&val2;
		  case TK_EQ:return val1==val2;
		  case TK_NEQ:return val1!=val2;
		  case TK_L:return val1<val2;
		  case TK_LE:return val1<=val2;
		  case TK_G:return val1>val2;
		  case TK_GE:return val1>=val2;
			case TK_PLUS:return val1+val2;
			case TK_MINUS:return val1-val2;
			case TK_MUL:return val1*val2;
			case TK_DIV:return val1/val2;
		  case TK_NOT:return !val2;
		  case TK_DEREF:return paddr_read(val2, 4);
			default:
				printf("There is no correct operation in debug at line %d\n",__LINE__);
				assert(0);
		}

		
	}
  return 0;//It is only added to pass the compilation
}

uint32_t expr(char *e, bool *success) {
 	if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  *success=true;
  return eval(1,nr_token);
}
