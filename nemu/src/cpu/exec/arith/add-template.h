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
    cpu.eflags._zf = !result;
    cpu.eflags._sf = MSB(result);
    if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 0 && MSB(result) == 0){
        cpu.eflags._cf = 0;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 0 && MSB(result) == 1){
        cpu.eflags._cf = 0;
        cpu.eflags._of = 1;
    }
    else if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 1 && MSB(result) == 0){
        cpu.eflags._cf = 1;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 1 && MSB(result) == 1){
        cpu.eflags._cf = 0;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 0 && MSB(result) == 0){
        cpu.eflags._cf = 1;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 0 && MSB(result) == 1){
        cpu.eflags._cf = 0;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 1 && MSB(result) == 0){
        cpu.eflags._cf = 1;
        cpu.eflags._of = 1;
    }
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 1 && MSB(result) == 1){
        cpu.eflags._cf = 1;
        cpu.eflags._of = 0;
    }
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
make_instr_helper(si2rm)
/*
make_helper(concat(add_i_b2rm_, SUFFIX)){
    int len = concat(decode_rm_, SUFFIX)(eip + 1);
    op_dest->type = op_src->type;
    op_dest->reg = op_src->reg;
    op_dest->val = op_src->val;
    int len2 = decode_i_b(eip + 1 + len);
    if(DATA_BYTE == 2){
        if(op_src->val >> 7 == 0)
            op_src->val = op_src->val & 0x00ff;
        else
            op_src->val = op_src->val | 0xff00;
    }
    else if(DATA_BYTE == 4){
        if(op_src->val >> 7 == 0)
            op_src->val = op_src->val & 0x000000ff;
        else
            op_src->val = op_src->val | 0xffffff00;
    }
    do_execute();
    return 1 + len + len2;
}
*/
#endif

#include "cpu/exec/template-end.h"

