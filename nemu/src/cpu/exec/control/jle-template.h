/*************************************************************************
	> File Name: jle-template.h
	> Author: 
	> Mail: 
	> Created Time: Sun 25 Sep 2016 11:26:35 AM CST
 ************************************************************************/
#include "cpu/exec/template-start.h"

#define instr jle

static void do_execute(){
    if(cpu.eflags._zf == 1 || cpu.eflags._cf != cpu.eflags._sf){
        cpu.eip = cpu.eip + op_src->val;
    }
    /*
    printf("zf:%d\n", cpu.eflags._zf);
    printf("cf:%d\n", cpu.eflags._cf);
    printf("sf:%d\n\n", cpu.eflags._sf);
    */
    print_asm_template1();
}

make_instr_helper(si)

#include "cpu/exec/template-end.h"
