#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include <elf.h>
#include "common.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

extern char *mystrtab;
extern Elf32_Sym *mysymtab;
extern int mynr;

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
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

static int cmd_help(char *args);

static int cmd_si(char *args){
	char *arg = strtok(NULL, " ");
	int N = 1;
	if(arg == NULL){
		cpu_exec(1);
		return 0;
	}
	else{
		N = atoi(arg);
		cpu_exec(N);
		return 0;
	}				
}

static int cmd_info(char *args){
	char *arg = strtok(NULL, " ");
	if(strcmp(arg, "r")==0){
		printf("eax\t%d\t0x%x\n",cpu.gpr[0]._32, cpu.eax);
		printf("ecx\t%d\t0x%x\n",cpu.gpr[1]._32, cpu.ecx);
		printf("edx\t%d\t0x%x\n",cpu.gpr[2]._32, cpu.edx);
		printf("ebx\t%d\t0x%x\n",cpu.gpr[3]._32, cpu.ebx);
		printf("esp\t%d\t0x%x\n",cpu.gpr[4]._32, cpu.esp);
		printf("ebp\t%d\t0x%x\n",cpu.gpr[5]._32, cpu.ebp);
		printf("esi\t%d\t0x%x\n",cpu.gpr[6]._32, cpu.esi);
		printf("edi\t%d\t0x%x\n\n",cpu.gpr[7]._32, cpu.edi);
        printf("cf=%d pf=%d zf=%d sf=%d if=%d df=%d of=%d\n", cpu.eflags._cf, cpu.eflags._pf, cpu.eflags._zf, cpu.eflags._sf, cpu.eflags._if, cpu.eflags._df, cpu.eflags._of);
	}
	else if(strcmp(arg, "w")==0){
		WP *temp = getHead();
		if(temp == NULL){
			printf("No watchpoint!!!\n");
		}
		for(; temp != NULL; temp = temp->next){
			printf("%d:%s%I32u\n", temp->NO, temp->expr, temp->value);
		}
	}
	return 0;
}

static int cmd_x(char *args){
	char *arg1 = strtok(NULL, " ");
	char *charArg2 = strtok(NULL, "\0");
	bool *success = false;
	uint32_t arg2 = expr(charArg2, success);
	//printf("args:%s\narg2:%d\n",charArg2,arg2);//test
	//system("pause");//test
	if(arg1 != NULL && arg2 != 0){
			int n = atoi(arg1);
			int i;
			for(i = 0; i < n; i++){
				printf("0x%x:  ", arg2);
				uint32_t temp = swaddr_read( arg2, 4);
				arg2 += 0x4;			
				printf("0x%x\n", temp);
			}
	}
	else
	{
		printf("Unfinished commmand\n");
	}
	return 0;
}

static int cmd_w(char *args){
	char *arg = strtok(NULL, "\0");
	bool *success = false;
	WP *newWP = (WP *)new_wp();
	strcpy(newWP->expr, arg);
	newWP->value = expr(arg, success);
	printf("Have set watchpoint %d: %s\n", newWP->NO, newWP->expr);
	return 0;
}

static int cmd_d(char *args){
	char *arg = strtok(NULL, " ");
	int n = atoi(arg);
	WP *tempWP = getHead();
	for(; tempWP != NULL; tempWP = tempWP->next){
		if(n == tempWP->NO){
			free_wp(tempWP);
			printf("Have deleted watchpoint %d: %s\n", tempWP->NO, tempWP->expr);
			return 0;
		}
	}
	printf("Not found watchpoint: %d\n", n);
	return 0;
}

static int cmd_p(char *args){
    char *arg = strtok(NULL, "\0");
    bool * success = false;
    //printf("%s\n", arg);
    uint32_t result = expr(arg, success);
    printf("0x%x\n", result);
    return 0;
}

static int cmd_bt(char *args){
    swaddr_t ptr = cpu.ebp;
    swaddr_t temp_eip = cpu.eip;
    swaddr_t t1=0,t2=0,t3=0,t4=0;
    while(ptr != 0){
        printf("0x%x in ", ptr);
            t1 = swaddr_read(ptr + 8, 4);
            t2 = swaddr_read(ptr + 12, 4);
            t3 = swaddr_read(ptr + 16, 4);
            t4 = swaddr_read(ptr + 20, 4);
        int i;
        printf("aaaaaaaa\n");
        for(i = 0; i < mynr; i++){
            if(temp_eip >= mysymtab[i].st_value && temp_eip <= mysymtab[i].st_value + mysymtab[i].st_size){
                printf("%s (0x%x, 0x%x, 0x%x, 0x%x)\n", mystrtab + mysymtab[i].st_name, t1, t2, t3, t4);
            } 
        }
            temp_eip = swaddr_read(ptr + 4, 4);
            printf("eip:%x\n", temp_eip);
        ptr = swaddr_read(ptr, 4);
        
    }
    return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },

	/* TODO: Add more commands */
	{ "si", "Next step(s)", cmd_si },
	{ "info", "Print infomations of registers or watchpoints", cmd_info },
	{ "x", "Scanf memory", cmd_x },
	{ "w", "Set watchpoints", cmd_w},
	{ "d", "Delete watchpoints", cmd_d},
    { "p", "Print expr", cmd_p},
    { "bt", "Print stackframe", cmd_bt},
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop(int argc, char *argv[]) {
    while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
