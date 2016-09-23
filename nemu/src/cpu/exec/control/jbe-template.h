/*************************************************************************
	> File Name: jbe-template.h
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 09:07:28 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute(){
    uint32_t rel;
    if(op_src->val >> 7 == 0)
        rel = op_src->val & 0x000000ff;
    else
        rel = op_src->val | 0xffffff00;
    //printf("aaaa:%d\n", rel);
    if(cpu.eflags._zf == 1 || cpu.eflags._cf == 1)
        cpu.eip = cpu.eip + rel;
    print_asm_template1();
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"
