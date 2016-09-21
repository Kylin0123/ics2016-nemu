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
    uint32_t data2 = data + 5;
    printf("eip:%x\n", data2);
    cpu.esp -= 4;
    MEM_W(cpu.esp, data2);
    cpu.eip = data + op_src->val;
    print_asm_template1();
}

#if DATA_BYTE == 4 || DATA_BYTE == 2 || DATA_BYTE == 1
make_helper(concat(call_rel_, SUFFIX)){
    DATA_TYPE rel = instr_fetch(eip + 1, DATA_BYTE);
    op_src->val = rel;
    do_execute();
    return 1 + DATA_BYTE;
}
#endif

#include "cpu/exec/template-end.h"
