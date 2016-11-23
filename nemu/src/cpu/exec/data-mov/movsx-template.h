/*************************************************************************
	> File Name: mov_s-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 11 Oct 2016 06:34:23 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
    DATA_TYPE temp = MEM_R(cpu.esi, R_DS);
    MEM_W(cpu.edi, temp, R_ES);
    /*printf("ecx:%d\n", cpu.ecx);
    printf("edi:%x\n", cpu.edi);
    printf("esi:%x\n", cpu.esi);
    printf("op_dest:%x\n", op_dest->val);
    printf("op_src:%x\n", op_src->val);*/
    //printf("%d\n", DATA_BYTE);
    //printf("ecx:%d ", cpu.ecx);
    //cpu.eflags._zf = !temp;
    if(cpu.eflags._df == 0){
        cpu.esi += DATA_BYTE;
        cpu.edi += DATA_BYTE;
    }
    else{
        cpu.esi -= DATA_BYTE;
        cpu.edi -= DATA_BYTE;
    }
    print_asm_template2();
}

make_helper(concat(movsx_m_, SUFFIX)){
    /*op_dest->type = OP_TYPE_REG;
    op_dest->reg = R_EDI;
    op_dest->val = REG(R_EDI);
    op_src->type = OP_TYPE_REG;
    op_src->reg = R_ESI;
    op_src->val = REG(R_ESI);*/
    do_execute();
    return 1;
}

#include "cpu/exec/template-end.h"

