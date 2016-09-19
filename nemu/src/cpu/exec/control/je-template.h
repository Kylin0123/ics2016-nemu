/*************************************************************************
	> File Name: je-template.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 08:20:57 AM PDT
 ************************************************************************/
#include "cpu/exec/template-start.h"

#define instr je

static void do_execute(){
    assert(cpu.eflags._zf == 1);//test
    if(cpu.eflags._zf == 1)
        cpu.eip = cpu.eip + op_src->val;
    print_asm_template1();
}

#if DATA_BYTE == 4 || DATA_BYTE == 2 || DATA_BYTE == 1
make_helper(concat(je_rel_, SUFFIX)){
    DATA_TYPE rel = instr_fetch(eip + 1, DATA_BYTE);
    op_src->val = rel;
    do_execute();
    return 1 + DATA_BYTE;
}
#endif

#include "cpu/exec/template-end.h"
