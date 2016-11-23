/*************************************************************************
	> File Name: ret-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 11 Oct 2016 09:00:06 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute(){
    cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
    cpu.esp += 4;
    cpu.esp += op_src->val;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
