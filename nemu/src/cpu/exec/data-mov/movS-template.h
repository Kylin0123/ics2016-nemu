/*************************************************************************
	> File Name: mov_s-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 11 Oct 2016 06:34:23 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr movS

static void do_execute(){
    DATA_TYPE temp = MEM_R(op_src->val);
    MEM_W(op_dest->val, temp);
    print_asm_template2();
}

make_helper(concat(movS_m_, SUFFIX)){
    op_dest->type = OP_TYPE_REG;
    op_dest->reg = R_EDI;
    op_dest->val = REG(R_EDI);
    op_src->type = OP_TYPE_REG;
    op_src->reg = R_ESI;
    op_src->val = REG(R_ESI);
    do_execute();
    return 1;
}

#include "cpu/exec/template-end.h"

