/*************************************************************************
	> File Name: movz-template.h
	> Author: 
	> Mail: 
	> Created Time: Thu 29 Sep 2016 03:32:54 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr movz

static void do_execute(){
    uint32_t temp = op_src->val;
    OPERAND_W(op_dest, temp);
    print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(movz_rmb2r_, SUFFIX)){
    printf("op_src:%x\n", op_src->val);
    printf("op_dest:%x\n\n", op_dest->val);
    decode_rm_b(eip);
    if(DATA_BYTE == 4){
        op_dest->type = OP_TYPE_REG;
        op_dest->reg = R_EAX;
        op_dest->val = REG(R_EAX);
    }
    else{
        op_dest->type = OP_TYPE_REG;
        op_dest->reg = R_AX;
        op_dest->val = REG(R_AX);
    }
    snprintf(op_dest->str, OP_STR_SIZE, "%s", REG_NAME(R_EAX));
    do_execute();
    return 1;
}
#endif

#if DATA_BYTE == 4
make_helper(movz_rmw2r_l){
    decode_rm_w(eip);
    op_dest->type = OP_TYPE_REG;
    op_dest->reg = R_EAX;
    op_dest->val = REG(R_EAX);
    snprintf(op_dest->str, OP_STR_SIZE, "%s", REG_NAME(R_EAX));
    do_execute();
    return 1;
}
#endif

#include "cpu/exec/template-end.h"
