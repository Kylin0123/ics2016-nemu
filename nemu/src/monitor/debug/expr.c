#include "nemu.h"
#include "common.h"
#include <stdlib.h>
#include <elf.h>

extern char *mystrtab;
extern Elf32_Sym *mysymtab;
extern int mynr;

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM, REG, NEQ, AND, OR, NOT, DEREF, MINUS, SYMBOL

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"\\-", '-'},					// minus
	{"\\*", '*'}, 				// multiply or deref
	{"\\/", '/'},					// divide
	{"[(]", '('},					// leftParentheses
	{"[)]", ')'},					// rightParentheses
	{"\\b0[xX][0-9a-fA-F]+\\b", NUM},	// number
	{"\\$[a-z]{2,3}", REG},  // register_name
	{"[!][=]", NEQ},          // not equal
	{"[&]{2}", AND},          // and
	{"[|]{2}", OR},           // or
	{"[!]", NOT},           // not
    {"[A-z0-9_]+", SYMBOL},    // symbol


};
#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				
				switch(rules[i].token_type) {
					case '+':
									 tokens[nr_token].type = '+';
									 tokens[nr_token++].str[0] = 4;
									 break;
					case '-':
									 tokens[nr_token].type = '-';
									 tokens[nr_token++].str[0] = 4;
									 break;
					case '*':
									 tokens[nr_token].type = '*';
									 tokens[nr_token++].str[0] = 3;
									 break;
					case '/':
									 tokens[nr_token].type = '/';
									 tokens[nr_token++].str[0] = 3;
									 break;
					case '(':
									 tokens[nr_token].type = '(';
									 tokens[nr_token++].str[0] = 1;
									 break;
					case ')':
									 tokens[nr_token].type = ')';
									 tokens[nr_token++].str[0] = 1;
									 break;
					case NOTYPE:
									    break;
					case EQ:
									tokens[nr_token].type = EQ;
									 tokens[nr_token++].str[0] = 7;
									break;
					case NUM:
									 tokens[nr_token].type = NUM;
									 char *tempStr = (char *)malloc((substr_len + 1)*sizeof(char));
									 strncpy(tempStr, substr_start, substr_len);
									 tempStr[substr_len] = '\0';
									 //printf("tempStr:%s\n", tempStr);//test
									 strcpy(tokens[nr_token].str, tempStr);
									 //printf("tokenStr%d:%s\n", nr_token, tokens[nr_token].str);//test
									 nr_token++;
									 break;
					case REG:
									 tokens[nr_token].type = REG;
									 strncpy(tokens[nr_token].str, substr_start, substr_len);
									 tokens[nr_token].str[substr_len] = '\0';
									 nr_token++;
									 break;
					case NEQ:
									 tokens[nr_token].type = NEQ;
									 tokens[nr_token++].str[0] = 7;
									 break;
					case AND:
									 tokens[nr_token].type = AND;
									 tokens[nr_token++].str[0] = 11;
									 break;
					case OR:
									 tokens[nr_token].type = OR;
									 tokens[nr_token++].str[0] = 12;
									 break;
					case NOT:
									 tokens[nr_token].type = NOT;
									 tokens[nr_token++].str[0] = 2;
									 break;
					case DEREF:
									 tokens[nr_token].type = DEREF;
									 tokens[nr_token++].str[0] = 2;
									 break;
                    case SYMBOL:
                                     tokens[nr_token].type = SYMBOL;
                                     //printf("yes\n");
									 char *tempStr2 = (char *)malloc((substr_len + 1)*sizeof(char));
									 strncpy(tempStr2, substr_start, substr_len);
									 tempStr2[substr_len] = '\0';
                                     strcpy(tokens[nr_token++].str, tempStr2);
                                     break;
					default: panic("please implement me");
				}
				break;
			}
		}
		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}

bool check_parentheses(int p, int q){
	if(tokens[p].type != '(' || tokens[q].type != ')')
        return false;
    int top = -1;
	char stack[30] = {0};
	int i;
	for(i = p+1; i <= q-1; i++){
		if(tokens[i].type == '(' && top <= 29){
			stack[++top] = '(';
		}
		else if(tokens[i].type == ')'){
			if(stack[top] == '('){
				if(top >= 0) top--;
				else return false;
			}
			else{
				stack[++top] = ')';
			}
		}
		else{
			continue;
		}
	}
	if(top == -1){
		return true;
	}
	else return false;
}

uint32_t eval(int p, int q){
	if(p > q){
		return 0;
	}
	else if(p == q){
		uint32_t temp = 0;
		if(tokens[p].type == NUM){
			sscanf(tokens[p].str, "%x", &temp);
		}
        else if(tokens[p].type == SYMBOL){
            
            int i;
            for(i = 0; i < mynr; i++){
                if(mysymtab[i].st_info == 0x11){
                    if(strcmp(mystrtab + mysymtab[i].st_name, tokens[p].str) == 0){
                        //printf("hahahah\n");
                        //printf("%x\n\n", mysymtab[i].st_info);
                        temp = (uint32_t)mysymtab[i].st_value;
                    }
                }
            }
            
        }
		else if(tokens[p].type == REG){
			if(strcmp(tokens[p].str, "$eax")==0){
				temp = cpu.gpr[0]._32;
			}
			else if(strcmp(tokens[p].str, "$ecx")==0){
				temp = cpu.gpr[1]._32;
			}
			else if(strcmp(tokens[p].str, "$edx")==0){
				temp = cpu.gpr[2]._32;
			}
			else if(strcmp(tokens[p].str, "$ebx")==0){
				temp = cpu.gpr[3]._32;
			}
			else if(strcmp(tokens[p].str, "$esp")==0){
				temp = cpu.gpr[4]._32;
			}
			else if(strcmp(tokens[p].str, "$ebp")==0){
				temp = cpu.gpr[5]._32;
			}
			else if(strcmp(tokens[p].str, "$esi")==0){
				temp = cpu.gpr[6]._32;
			}
			else if(strcmp(tokens[p].str, "$edi")==0){
				temp = cpu.gpr[7]._32;
			}
			else if(strcmp(tokens[p].str, "$ax")==0){
				temp = cpu.gpr[0]._16;
			}
			else if(strcmp(tokens[p].str, "$cx")==0){
				temp = cpu.gpr[1]._16;
			}
			else if(strcmp(tokens[p].str, "$dx")==0){
				temp = cpu.gpr[2]._16;
			}
			else if(strcmp(tokens[p].str, "$bx")==0){
				temp = cpu.gpr[3]._16;
			}
			else if(strcmp(tokens[p].str, "$sp")==0){
				temp = cpu.gpr[4]._16;
			}
			else if(strcmp(tokens[p].str, "$bp")==0){
				temp = cpu.gpr[5]._16;
			}
			else if(strcmp(tokens[p].str, "$si")==0){
				temp = cpu.gpr[6]._16;
			}
			else if(strcmp(tokens[p].str, "$di")==0){
				temp = cpu.gpr[7]._16;
			}
			else if(strcmp(tokens[p].str, "$ah")==0){
				temp = cpu.gpr[0]._8[1];
			}
			else if(strcmp(tokens[p].str, "$al")==0){
				temp = cpu.gpr[0]._8[0];
			}
			else if(strcmp(tokens[p].str, "$ch")==0){
				temp = cpu.gpr[1]._8[1];
			}
			else if(strcmp(tokens[p].str, "$cl")==0){
				temp = cpu.gpr[1]._8[0];
			}
			else if(strcmp(tokens[p].str, "$dh")==0){
				temp = cpu.gpr[2]._8[1];
			}
			else if(strcmp(tokens[p].str, "$dl")==0){
				temp = cpu.gpr[2]._8[0];
			}
			else if(strcmp(tokens[p].str, "$bh")==0){
				temp = cpu.gpr[3]._8[1];
			}
			else if(strcmp(tokens[p].str, "$bl")==0){
				temp = cpu.gpr[3]._8[0];
			}
            else if(strcmp(tokens[p].str, "$eip")==0){
                temp = cpu.eip;
            }
            else{
                printf("No such register!\n");
            }
		}
		return temp;
	}
	else if(check_parentheses(p,q)==true){
		return eval(p + 1, q - 1);
	}
	else if(q - p == 1 && tokens[p].type == MINUS){
		return -eval(q,q);
	}
	else if(q - p == 1 && tokens[p].type == DEREF){
		return swaddr_read( eval(q,q), 4);
	}
	else{
		int op = -1;
		int flag_parentheses = 0;
		int i;
		for(i = p; i <= q; i++){
			if(tokens[i].type == '('){
				flag_parentheses++;
				continue;
			}
			else if(tokens[i].type == ')'){
				flag_parentheses--;
			}
			else if(flag_parentheses == 0 && op == -1 && tokens[i].type != NUM && tokens[i].type != REG && tokens[i].type != SYMBOL){
				op = i;
			}
			else if(flag_parentheses == 0 && tokens[i].type != NUM && tokens[i].type != REG && tokens[i].type != SYMBOL && (tokens[i].str[0] >= tokens[op].str[0])){
				op = i;
			}	
		
		}
		printf("op:%d\n", op);//test
		if(tokens[op].type == '+'){
            printf("+++++++\n");
			return eval(p, op - 1) + eval(op + 1, q);
		}
		else if(tokens[op].type == '-'){
			return eval(p, op - 1) - eval(op + 1, q);
		}
		else if(tokens[op].type == '*'){
			return eval(p, op - 1) * eval(op + 1, q);
		}
		else if(tokens[op].type == '/'){
			return eval(p, op - 1) / eval(op + 1, q);
		}
		else if(tokens[op].type == EQ){
			if(eval(p, op - 1) == eval(op + 1, q))
				return 1;
			else
				return 0;
		}
		else if(tokens[op].type == NEQ){
			if(eval(p, op - 1) != eval(op + 1, q))
				return 1;
			else
				return 0;
		}
		else if(tokens[op].type == AND){
			return eval(p, op - 1) && eval(op + 1, q);	
		}
		else if(tokens[op].type == OR){
			return eval(p, op - 1) || eval(op + 1, q);	
		}
		else if(tokens[op].type == NOT){
			if(eval(op + 1, q) == 0)
				return 1;
			else
				return 0;
        }
        else if(tokens[op].type == DEREF){
            printf("*******\n");
		    return swaddr_read( eval(op + 1,q), 4);
        }
        else if(tokens[op].type == MINUS){
		    return -eval(q,q);
        }
		else
			return 0;
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	
	int i;
	for(i = 0; i < nr_token; i++){
		if(tokens[i].type == '*' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != REG))){
			tokens[i].type = DEREF;
            tokens[i].str[0] = 2;
		}
	}
	
	for(i = 0; i < nr_token; i++){
		if(tokens[i].type == '-' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != REG))){
			tokens[i].type = MINUS;
            tokens[i].str[0] = 2;
		}
	}

	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	//printf("nr_token:%d\n", nr_token);
	//printf("str0:%s\n", tokens[0].str);
	//printf("str2%s\n", tokens[2].str);

	return eval(0, nr_token - 1);
}




