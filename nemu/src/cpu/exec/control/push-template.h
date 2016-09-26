/*************************************************************************
	> File Name: push-template.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 12:51:40 AM PDT
 ************************************************************************/


#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
    cpu.esp -= 4;
    MEM_W(cpu.esp, op_src->val);
    print_asm_template1();
}

make_helper(concat(push_ebp_, SUFFIX)){
    op_src->val = cpu.ebp;
    do_execute();
    return 1;
}

make_helper(concat(push_edx_, SUFFIX)){
    op_src->val = cpu.edx;
    do_execute();
    return 1;
}

make_helper(concat(push_eax_, SUFFIX)){
    op_src->val = cpu.eax;
    do_execute();
    return 1;
}


make_helper(concat(push_m_, SUFFIX)){
    swaddr_t addr = instr_fetch(eip + 1, DATA_BYTE);
    op_src->val = addr;
    do_execute();
    return 1 + DATA_BYTE;
}

#include "cpu/exec/template-end.h"
