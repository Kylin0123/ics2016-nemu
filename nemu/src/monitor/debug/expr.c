#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM

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
	{"\\*", '*'}, 				// multiply
	{"\\/", '/'},					// divide
	{"\\(", '('},					// leftParentheses
	{"\\)", ')'},					// rightParentheses
	{"\\b0[xX][0-9a-fA-F]+\\b", NUM},					// number

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
									tokens[nr_token++].type = '+';
									 break;
					case '-':
									 tokens[nr_token++].type = '-';
									 break;
					case '*':
									 tokens[nr_token++].type = '*';
									 break;
					case '/':
									 tokens[nr_token++].type = '/';
									 break;
					case NOTYPE:
									    break;
					case EQ:
									tokens[nr_token++].type = EQ;
									break;
					case NUM:
									 tokens[nr_token].type = NUM;
									 char *tempStr = (char *)malloc((substr_len + 1)*sizeof(char));
									 strncpy(tempStr, substr_start, substr_len);
									 tempStr[substr_len] = '\0';
									 printf("tempStr:%s\n", tempStr);//test
									 strcpy(tokens[nr_token].str, tempStr);
									 printf("tokenStr%d:%s\n", nr_token, tokens[nr_token].str);//test
									 nr_token++;
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
	int top = -1;
	char stack[30] = {0};
	int i;
	for(i = p; i <= q; i++){
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
	if(top == -1) return true;
	else return false;
}

uint32_t eval(int p, int q){
	if(p > q){
		return 0;
	}
	else if(p == q){
		uint32_t temp = 0;
		sscanf(tokens[p].str, "%x", &temp);
		printf("temp:%d\n", temp);
		return temp;
	}
	else if(check_parentheses(p,q)==true){
		return eval(p + 1, q - 1);
	}
	else{
		int op = q;
		int flag_parentheses = 0;
		int i;
		for(i = q; i >= p; i--){
			if(tokens[i].type == ')'){
				flag_parentheses++;
				continue;
			}
			else if(tokens[i].type == '('){
				flag_parentheses--;
			}
			else if(flag_parentheses == 0 && (tokens[i].type == '+' || tokens[i].type == '-')){
				op = i;
			}	
			else if(flag_parentheses == 0 && (tokens[i].type == '*' || tokens[i].type == '/') && (tokens[op].type != '+' || tokens[op].type != '-')){
				op = i;
			}
		
		}

		if(tokens[op].type == '+'){
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
		return 0;
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	//return 0;
	printf("nr_token:%d\n", nr_token);
	printf("str0:%s\n", tokens[0].str);
	printf("str1:%s\n", tokens[1].str);
	return eval(0, nr_token - 1);
}




