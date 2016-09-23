/*************************************************************************
	> File Name: add-template.h
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 11:39:36 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr add

static void do_execute(){
    DATA_TYPE result = op_dest->val + op_src->val;
    OPERAND_W(op_dest, result);
    //todo : eflags is unfinished.
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

