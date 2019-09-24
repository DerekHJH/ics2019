#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

//In order to use atoi
#include<stdlib.h>

enum {
  TK_NOTYPE = 256, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, 
	TK_LP, TK_RP, 
	TK_EQ, TK_NEQ, TK_LE, TK_L, TK_GE, TK_G, 
	TK_AND, TK_OR, TK_NOT,
	TK_DIGNUM, TK_HEXNUM, TK_REG,
  TK_DEREF //Originally, all * is regarded as the multipication sign and shall be identified as dereference sign later.	
		//I changed the order of TK_EQ, which is originally at position2. And changed the symbol of "+";

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

	{"0x[0-9a-f]*", TK_HEXNUM},    //hex number  
	{"[0-9]+", TK_DIGNUM},   //digital numbers
	{"$[a-z]*", TK_REG},    //registers

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
  int type;
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
        if(rules[i].token_type>=TK_PLUS&&rules[i].token_type<=TK_NOT)
				{
					tokens[++nr_token].type=rules[i].token_type;
				  tokens[nr_token].num=0;
					if(rules[i].token_type==TK_MUL)
					{
						if(nr_token==1||!(tokens[nr_token-1].type>=TK_DIGNUM&&tokens[nr_token-1].type<=TK_REG))tokens[nr_token].type=TK_DEREF;
					}
					else  if(rules[i].token_type==TK_DIGNUM)
					{		
						uint32_t pow10=1;
						for(int i=substr_len-1;i>=0;i--)
						{
							tokens[nr_token].num+=((*(substr_start+i))-'0')*pow10;
							pow10*=10;
						}
					}
					else if(rules[i].token_type==TK_HEXNUM)
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
					}
					else if(rules[i].token_type==TK_REG)
          {
                //We need to extract the value in those regesters
					}
				}
				else
				{
                    //for NOTYPE operations
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

uint32_t eval(int l,int r)///////Add more operations here.
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
			for(int i=1;i<=nr_token;i++)
				if(tokens[i].type==TK_DIGNUM)printf("%u",tokens[i].num);
				else printf("+");
			printf("\n%u\n",nr_token);
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
    int op=0;
    for(op=r;op>=l;op--)
			if(tokens[op].type==TK_RP)
			{
				while(tokens[op].type!=TK_LP&&op>=l)op--;
				if(op<l)
				{
        	printf("Bad expresion in debug at line %d\n",__LINE__);
        	assert(0);                                                 
				}
			}
	  	else if(tokens[op].type==TK_PLUS||tokens[op].type==TK_MINUS)break;
   
	 	if(op<l)
		{
       for(op=r;op>=l;op--)
	    	 if(tokens[op].type==TK_RP)
         {
       	   while(tokens[op].type!=TK_LP&&op>=l)op--;
					 if(op<l)
					 {
             printf("Bad expresion in debug at line %d\n",__LINE__);
             assert(0);                                                 
				   }
         }
         else if(tokens[op].type==TK_MUL||tokens[op].type==TK_DIV)break;
		}

		uint32_t val1=eval(l,op-1);
		uint32_t val2=eval(op+1,r);

		switch(tokens[op].type)
		{
			case TK_PLUS:return val1+val2;
			case TK_MINUS:return val1-val2;
			case TK_MUL:return val1*val2;
			case TK_DIV:return val1/val2;
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

  return eval(1,nr_token);
}
