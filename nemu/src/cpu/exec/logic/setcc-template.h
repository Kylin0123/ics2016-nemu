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

#define instr seta

static void do_execute(){
    if(cpu.eflags._cf == 0 && cpu.eflags._zf == 0)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setbe

static void do_execute(){
    if(cpu.eflags._cf == 1 || cpu.eflags._zf == 1)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setae

static void do_execute(){
    if(cpu.eflags._cf == 0)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setb

static void do_execute(){
    if(cpu.eflags._cf == 1)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr seto

static void do_execute(){
    if(cpu.eflags._of == 1)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setno

static void do_execute(){
    if(cpu.eflags._of == 0)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setg

static void do_execute(){
    if(cpu.eflags._zf == 0 && cpu.eflags._sf == cpu.eflags._of)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setge

static void do_execute(){
    if(cpu.eflags._sf == cpu.eflags._of)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setl

static void do_execute(){
    if(cpu.eflags._sf != cpu.eflags._of)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setle

static void do_execute(){
    if(cpu.eflags._zf == 1 || cpu.eflags._sf != cpu.eflags._of)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setnp

static void do_execute(){
    if(cpu.eflags._pf == 0)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setns

static void do_execute(){
    if(cpu.eflags._sf == 0)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr setp

static void do_execute(){
    if(cpu.eflags._pf == 1)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#define instr sets

static void do_execute(){
    if(cpu.eflags._sf == 1)
        OPERAND_W(op_src, 1);
    else
        OPERAND_W(op_src, 0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#include "cpu/exec/template-end.h"


