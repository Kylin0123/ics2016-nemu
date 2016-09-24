/*************************************************************************
	> File Name: sub-template.h
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 07:36:40 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr sub

static void do_execute(){
    DATA_TYPE result = op_dest->val - op_src->val;
    if(result == 0)
        cpu.eflags._zf = 1;
    else
        cpu.eflags._zf = 0;
    
    if(result < 0)
        cpu.eflags._cf = 1;
    else
        cpu.eflags._cf = 0;
    OPERAND_W(op_dest, result);
    print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(sub_i_b2rm_, SUFFIX)){
    int len = concat(decode_rm_, SUFFIX)(eip + 1);
    //printf("len:%d\n", len);
    len += decode_i_b(eip + 1 + len);
    //snprintf(op_dest->str, OP_STR_SIZE, "$0x%x", op_src->imm);
    do_execute();
    return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
