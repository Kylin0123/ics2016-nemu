/*************************************************************************
	> File Name: jmp-template.h
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 08:35:14 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
    cpu.eip = cpu.eip + op_src->val;
    print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"


