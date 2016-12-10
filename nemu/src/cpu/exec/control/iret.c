/*************************************************************************
	> File Name: iret.c
	> Author: 
	> Mail: 
	> Created Time: Fri 09 Dec 2016 09:21:30 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

extern void sreg_load(uint8_t sreg);

make_helper(iret){
    /*pop eip*/
    cpu.eip = swaddr_read(cpu.esp, 4, R_SS) - 1;
    printf("eip:%x\n",cpu.eip);
    cpu.esp += 4;

    /*pop cs*/
    cpu.sr[R_CS].val = swaddr_read(cpu.esp, 4, R_SS);
    cpu.esp += 4;
    sreg_load(R_CS);

    /*pop eflags*/
    cpu.eflags.val = swaddr_read(cpu.esp, 4, R_SS);
    cpu.esp += 4;

    print_asm("iret");
    return 1;
}
