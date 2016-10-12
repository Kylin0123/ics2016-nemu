/*************************************************************************
	> File Name: stos-template.h
	> Author: 
	> Mail: 
	> Created Time: Wed 12 Oct 2016 04:16:18 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute(){
    DATA_TYPE temp = MEM_R(cpu.edi);
    MEM_W(temp, cpu.eax);
    if(cpu.eflags._df == 0){
        cpu.edi += DATA_BYTE;
    }
    else{
        cpu.edi -= DATA_BYTE;
    }
    print_asm_template2();
}

make_helper(concat(stos_m_, SUFFIX)){
    do_execute();
    return 1;
}

#include "cpu/exec/template-end.h"


