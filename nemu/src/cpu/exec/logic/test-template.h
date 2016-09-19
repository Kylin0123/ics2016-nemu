/*************************************************************************
	> File Name: test-template.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 05:58:25 AM PDT
 ************************************************************************/

#include "cpu/exec/temlate-start.h"

#define instr test

static void do_execute(){
    DATA_TYPE temp = op_dest->val & op_src->val;
    cpu.eflags._sf = MSB(temp);
    
    if(temp == 0)
        cpu.eflags._zf = 1;
    else
        cpu.eflags._zf = 0;
    int temp_pf = temp & 1;
    int temp_test = 2;
    int i;
    for(i = 1; i < 8; i++){
        int temp_k = (temp & temp_test) >> i;
        temp_pf = !(temp_k ^ temp_fp);
        temp_test << 1;
    }
    cpu.eflags._pf = temp_pf;
    cpu.eflags._cf = 0;
    cpu.eflags._of = 0;
    
    print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

