#include "monitor/monitor.h"
#include "cpu/helper.h"
#include <setjmp.h>
//my include
#include "monitor/watchpoint.h"
#include "monitor/expr.h"

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10000

int nemu_state = STOP;

int exec(swaddr_t);

char assembly[80];
char asm_buf[128];

/* Used with exception handling. */
jmp_buf jbuf;

void print_bin_instr(swaddr_t eip, int len) {
	int i;
	int l = sprintf(asm_buf, "%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(eip + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 50 - (12 + 3 * len), "");
}

/* This function will be called when an `int3' instruction is being executed. */
void do_int3() {
	printf("\nHit breakpoint at eip = 0x%08x\n", cpu.eip);
	nemu_state = STOP;
}

/* Simulate how the CPU works. */
void cpu_exec(volatile uint32_t n) {
	if(nemu_state == END) {
		printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
		return;
	}
	nemu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	setjmp(jbuf);

	for(; n > 0; n --) {
#ifdef DEBUG
		swaddr_t eip_temp = cpu.eip;
		if((n & 0xffff) == 0) {
			/* Output some dots while executing the program. */
			fputc('.', stderr);
		}
#endif

		/* Execute one instruction, including instruction fetch,
		 * instruction decode, and the actual execution. */
		int instr_len = exec(cpu.eip);

		cpu.eip += instr_len;

#ifdef DEBUG
		print_bin_instr(eip_temp, instr_len);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if(n_temp < MAX_INSTR_TO_PRINT) {
			printf("%s\n", asm_buf);
		}
#endif

		/* TODO: check watchpoints here. */
		WP *tempWP = getHead();
		for(; tempWP != NULL; tempWP = tempWP->next){
			bool *success = false;
			uint32_t exprValue = expr(tempWP->expr, success);
			uint32_t readValue = swaddr_read( exprValue, 4);
            printf("wp_test:exprValue:%x\nreadValue:%x\n", exprValue, readValue);
            printf("tempValue:%x\n", tempWP->value);
			if(tempWP->value != exprValue){
				printf("Watchpoint %d changed:%d->%d\n", tempWP->NO, tempWP->value, readValue);
				nemu_state = STOP;
			}
		}

#ifdef HAS_DEVICE
		extern void device_update();
		device_update();
#endif

		if(nemu_state != RUNNING) { return; }
	}

	if(nemu_state == RUNNING) { nemu_state = STOP; }
}
