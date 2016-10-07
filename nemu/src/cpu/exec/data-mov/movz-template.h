/*************************************************************************
	> File Name: movz-template.h
	> Author: 
	> Mail: 
	> Created Time: Thu 29 Sep 2016 03:32:54 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr movz

static void do_execute(){
    uint32_t temp = op_src->val;
    OPERAND_W(op_dest, temp);
    print_asm_template2();
}

/*#if DATA_BYTE == 2
make_helper(movz_rmb2r_w){
    
}
#endif

make_helper(movz_rmb2r_l){

}

make_helper(movz_rmw2r_l){
    
}*/
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(movz_rmb2r_, SUFFIX)){
    op_src->size = 1;
    int len = read_ModR_M(eip + 1, op_src, op_dest);
    op_dest->val = REG(op_dest->reg);
    //printf("len:%d\n", len);
    //printf("op_src:%x\n", op_src->val);
    //printf("op_dest:%x\n\n", op_dest->val);
    //snprintf(op_dest->str, OP_STR_SIZE, "%s", REG_NAME(R_EAX));
    do_execute();
    return DATA_BYTE + len - 3;
}
#endif


#if DATA_BYTE == 4
make_helper(movz_rmw2r_l){
    op_src->size = 2;
    int len = read_ModR_M(eip + 1, op_src, op_dest);
    op_dest->val = REG(op_dest->reg);
    do_execute();
    return DATA_BYTE + len - 3;
}
#endif

#include "cpu/exec/template-end.h"
