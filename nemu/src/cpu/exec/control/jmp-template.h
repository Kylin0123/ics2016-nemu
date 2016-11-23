/*************************************************************************
	> File Name: jmp-template.h
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 08:35:14 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
    cpu.eip = cpu.eip + op_src->val;
    print_asm_template1();
}

make_instr_helper(si)

make_helper(concat(jmp_rm_, SUFFIX)){
    concat(decode_rm_, SUFFIX)(eip + 1);
    cpu.eip = op_src->val;
    return 0;
}

extern void sreg_load(uint8_t);

make_helper(concat(ljmp_, SUFFIX)){
    swaddr_t addr = instr_fetch(eip + 1, 4);
    uint16_t sreg = instr_fetch(eip + 5, 2);
    cpu.cs = sreg;
    sreg_load(R_CS);
    cpu.eip = addr - 7;
    print_asm("ljmp" str(SUFFIX) " $0x%x,$0x%x", sreg, addr);
    return 7;
}

#include "cpu/exec/template-end.h"


