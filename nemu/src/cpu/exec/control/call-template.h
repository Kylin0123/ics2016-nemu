/*************************************************************************
	> File Name: call-template.h
	> Author: 
	> Mail: 
	> Created Time: Fri 16 Sep 2016 05:53:48 PM PDT
 ************************************************************************/
#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
    uint32_t data = cpu.eip;
    cpu.esp -= 4;
    MEM_W(cpu.esp, data);
    cpu.eip = data + op_src->val;
    print_asm_template1();
}

#if DATA_BYTE == 4 || DATA_BYTE == 2 || DATA_BYTE == 1
make_helper(concat(call_rel_, SUFFIX)){
    uint32_t rel = instr_fetch(eip + 1, DATA_BYTE);
    op_src->val = rel;
    do_execute();
    return 1 + DATA_BYTE;
}
#endif

#include "cpu/exec/template-end.h"
