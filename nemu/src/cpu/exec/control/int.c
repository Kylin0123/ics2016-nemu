/*************************************************************************
	> File Name: int.c
	> Author: 
	> Mail: 
	> Created Time: Fri 09 Dec 2016 03:33:16 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

extern void raise_intr(uint8_t NO);

make_helper(Int_i_b){
    int NO = instr_fetch(eip + 1, 1);
    cpu.eip += 2;
    raise_intr(NO);
    print_asm("int");
    return 0;
}
