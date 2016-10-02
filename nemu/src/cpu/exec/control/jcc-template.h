/*************************************************************************
	> File Name: jcc-template.h
	> Author: 
	> Mail: 
	> Created Time: Sun 02 Oct 2016 08:10:47 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr je
static void do_execute(){
    if(cpu.eflags._zf == 1)
        cpu.eip += op_src->val;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jle
static void do_execute(){
    if(cpu.eflags._zf == 1 || cpu.eflags._of != cpu.eflags._sf)
        cpu.eip += op_src->val;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jbe
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._zf == 1 || cpu.eflags._cf == 1)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jne
static void do_execute(){
    int32_t rel = op_src->val;
    //printf("rel:%d\n", rel);
    if(cpu.eflags._zf == 0)
        cpu.eip += rel;
    //printf("eip:%x\n", cpu.eip);
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#include "cpu/exec/template-end.h"

