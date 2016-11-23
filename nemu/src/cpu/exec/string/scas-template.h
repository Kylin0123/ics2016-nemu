/*************************************************************************
	> File Name: scas-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 11 Oct 2016 11:45:30 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
    DATA_TYPE src = swaddr_read(cpu.edi, 4, R_ES);
    DATA_TYPE temp = cpu.eax - src;
    //printf("src:%x\n", src);
    //printf("op_dest:%x\n", op_dest->val);
    //printf("temp:%x\n", temp);
    //printf("ecx:%x\n", cpu.ecx);
    if(MSB(cpu.eax) == 0 && MSB(src) == 1 && MSB(temp) == 1)
        cpu.eflags._of = 1;
    else if(MSB(cpu.eax) == 1 && MSB(src) == 0 && MSB(temp) == 0)
        cpu.eflags._of = 1;
    else
        cpu.eflags._of = 0;

    cpu.eflags._zf = !temp;
    //printf("zf:%d\n", cpu.eflags._zf);
    cpu.eflags._sf = MSB(temp);
    if(cpu.eax >= src)
        cpu.eflags._cf = 0;
    else
        cpu.eflags._cf = 1;
    if(cpu.eflags._df == 0){
        cpu.edi += DATA_BYTE;
    }
    else{
        cpu.edi -= DATA_BYTE;
    }
    print_asm_template2();
}

make_helper(concat(scas_m_, SUFFIX)){
    /*op_dest->type = OP_TYPE_REG;
    op_dest->reg = R_EAX;
    op_dest->val = REG(R_EAX);
    op_src->type = OP_TYPE_REG;
    op_src->reg = R_EDI;
    op_src->val = REG(R_EDI);*/
    do_execute();
    return 1;
}

#include "cpu/exec/template-end.h"
