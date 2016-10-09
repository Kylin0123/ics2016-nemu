/*************************************************************************
	> File Name: movs-template.h
	> Author: 
	> Mail: 
	> Created Time: Sat 08 Oct 2016 11:27:31 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr movs

static void do_execute(){
    DATA_TYPE_S temp = op_src->val;
    printf("sign:%d\n", MSB(op_src->val));
    printf("1:%x\n", op_src->val);
    //int32_t temp = (int)op_src->val;
    printf("2:%x\n\n", temp);
    OPERAND_W(op_dest, temp);
    print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(movs_rmb2r_, SUFFIX)){
    op_src->size = 1;
    int len = read_ModR_M(eip + 1, op_src, op_dest);
    op_dest->val = REG(op_dest->reg);
    do_execute();
    return DATA_BYTE + len - 3;
}
#endif

#if DATA_BYTE == 4
make_helper(movs_rmw2r_l){
    op_src->size = 2;
    int len = read_ModR_M(eip + 1, op_src, op_dest);
    op_dest->val = REG(op_dest->reg);
    do_execute();
    return DATA_BYTE + len - 3;
}
#endif

#include "cpu/exec/template-end.h"
