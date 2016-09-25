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
    /*printf("op_dest:%d\n", op_dest->val);
    printf("op_src:%d\n", op_src->val);
    printf("result:%d\n", result);
    print_asm_template2();
    */
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE == 2 || DATA_BYTE == 4

make_helper(concat(add_i_b2rm_, SUFFIX)){
    int len = concat(decode_rm_, SUFFIX)(eip + 1);
    op_dest->type = op_src->type;
    op_dest->reg = op_src->reg;
    op_dest->val = op_src->val;
    int len2 = decode_i_b(eip + 1 + len);
    do_execute();
    return 1 + len + len2;
}

#endif

#include "cpu/exec/template-end.h"

