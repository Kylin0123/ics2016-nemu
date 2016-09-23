/*************************************************************************
	> File Name: sub-template.h
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 07:36:40 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute(){
    DATA_TYPE result = op_dest->val - op_src->val;
    if(result == 0)
        cpu.eflags._zf = 1;
    else
        cpu.eflags._zf = 0;
    
    if(result < 0)
        cpu.eflags._cf = 1;
    else if(result > 0)
        cpu.eflags._cf = 0;
    OPERAND_W(op_dest, result);
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
