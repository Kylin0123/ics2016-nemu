/*************************************************************************
	> File Name: setcc-template.h
	> Author: 
	> Mail: 
	> Created Time: Sun 02 Oct 2016 07:41:19 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute(){
    if(cpu.eflags._zf == 0)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr sete

static void do_execute(){
    if(cpu.eflags._zf == 1)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#include "cpu/exec/template-end.h"


