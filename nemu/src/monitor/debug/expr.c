#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

//In order to use atoi
#include<stdlib.h>

enum {
  TK_NOTYPE = 256, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_LP, TK_RP, TK_NUM, TK_EQ   
		//I chaged the order of TK_EQ, which is originally at position2. And changed the symbol of "+";

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
	{"/", TK_DIV},    //divide
	{"\\(", TK_LP},    //left parenthsis
	{"\\)", TK_RP},    //right parenthesis
	{"[0-9]+", TK_NUM},   //numbers
  {"==", TK_EQ}         // equal
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
  char str[32];
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
        if(rules[i].token_type>=TK_PLUS&&rules[i].token_type<=TK_NUM)
				{
					tokens[++nr_token].type=rules[i].token_type;
				  memset(tokens[nr_token].str,0,sizeof(tokens[nr_token].str));
				  if(rules[i].token_type==TK_NUM)
					{
            //Check if the number is too long
						if(substr_len>31||substr_len<0)
						{
							printf("The number in the expression is too large!!!\n");
							assert(0);
						}
							
						for(int i=0;i<substr_len;i++)
						{
							tokens[nr_token].str[i]=*(substr_start+i);
						}
						tokens[nr_token].str[substr_len]='\0';
					}

				}
				else
				{

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

void Bad_Expr()
{
	printf("Bad expression\n");
	assert(0);
	return;
}

bool check_parenthsis(int l,int r)
{
  if(!(tokens[l].type==TK_LP&&tokens[r].type==TK_RP))return false;
	else
	{
		for(int i=l+1;i<r;i++)
		  if(tokens[i].type==TK_RP)return false;
	}
	return true;
}	 

int eval(int l,int r)
{
  if(l>r)Bad_Expr();
	else if(l==r)
	{
		if(tokens[l].type!=TK_NUM)Bad_Expr();
		else return atoi(tokens[l].str);
	}
	else if(check_parenthsis(l,r))return eval(l+1,r-1);
	else
	{
    int op=0;
    for(op=r;op>=l;op--)
			if(tokens[op].type==TK_RP)
			{
				while(tokens[op].type!=TK_LP&&op>=l)op--;
				if(op<l)Bad_Expr();
			}
	  	else if(tokens[op].type==TK_PLUS||tokens[op].type==TK_MINUS)break;
   
	 	if(op<l)
		{
       for(op=r;op>=l;op--)
	    	 if(tokens[op].type==TK_RP)
         {
       	   while(tokens[op].type!=TK_LP&&op>=l)op--;
					 if(op<l)Bad_Expr();
         }
         else if(tokens[op].type==TK_MUL||tokens[op].type==TK_DIV)break;
		}

		int val1=eval(l,op-1);
		int val2=eval(op+1,r);

		switch(tokens[op].type)
		{
			case TK_PLUS:return val1+val2;
			case TK_MINUS:return val1-val2;
			case TK_MUL:return val1*val2;
			case TK_DIV:return val1/val2;
			default:
				printf("There is no correct operation\n");
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
  eval(1,nr_token);
	return 0;
}
