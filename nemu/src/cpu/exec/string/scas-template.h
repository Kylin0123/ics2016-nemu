/*************************************************************************
	> File Name: scas-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 11 Oct 2016 11:45:30 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
    DATA_TYPE src = MEM_R(op_src->val);
    DATA_TYPE temp = op_dest->val - src;
    printf("temp:%x\n", temp);
    printf("ecx:%x\n", cpu.ecx);
    if(MSB(op_dest->val) == 0 && MSB(src) == 1 && MSB(temp) == 1)
        cpu.eflags._of = 1;
    else if(MSB(op_dest->val) == 1 && MSB(src) == 0 && MSB(temp) == 0)
        cpu.eflags._of = 1;
    else
        cpu.eflags._of = 0;

    cpu.eflags._zf = !temp;
    cpu.eflags._sf = MSB(temp);
    if(op_dest->val >= src)
        cpu.eflags._cf = 0;
    else
        cpu.eflags._cf = 1;
    print_asm_template2();
}

make_helper(concat(scas_m_, SUFFIX)){
    op_dest->type = OP_TYPE_REG;
    op_dest->reg = R_EAX;
    op_dest->val = REG(R_EAX);
    op_src->type = OP_TYPE_REG;
    op_src->reg = R_EDI;
    op_src->val = REG(R_EDI);
    do_execute();
    return 1;
}

#include "cpu/exec/template-end.h"
