/*************************************************************************
	> File Name: push-template.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 12:51:40 AM PDT
 ************************************************************************/


#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
    if(ops_decoded.is_operand_size_16){
        cpu.esp -= 2;
        swaddr_write(cpu.esp, 2, op_src->val, R_SS);
    }
    else{
        cpu.esp -= 4;
        swaddr_write(cpu.esp, 4, op_src->val, R_SS);
    }
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
