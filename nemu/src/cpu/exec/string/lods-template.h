/*************************************************************************
	> File Name: lods-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 11 Oct 2016 11:25:36 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute(){
    DATA_TYPE temp = MEM_R(op_src->val);
    cpu.eax = temp;
    if(cpu.eflags._df == 0){
        cpu.esi += DATA_BYTE;
    }
    else{
        cpu.esi -= DATA_BYTE;
    }
    print_asm_template2();
}

make_helper(concat(lods_m_, SUFFIX)){
    op_dest->type = OP_TYPE_REG;
    op_dest->reg = R_EAX;
    op_dest->val = REG(R_EAX);
    op_src->type = OP_TYPE_REG;
    op_src->reg = R_ESI;
    op_src->val = REG(R_ESI);
    do_execute();
    return 1;
}

#include "cpu/exec/template-end.h"
