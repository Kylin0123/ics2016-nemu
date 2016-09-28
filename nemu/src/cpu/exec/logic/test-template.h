/*************************************************************************
	> File Name: test-template.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 05:58:25 AM PDT
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    DATA_TYPE temp = op_dest->val & op_src->val;
    cpu.eflags._sf = MSB(temp);
    cpu.eflags._zf = !temp; 
    int temp_pf = temp & 1;
    int temp_test = 2;
    int i;
    for(i = 1; i < 8; i++){
        int temp_k = (temp & temp_test) >> i;
        temp_pf = !(temp_k ^ temp_pf);
        temp_test = temp_test << 1;
    }
    cpu.eflags._pf = temp_pf;
    printf("test_eax:%x\n", cpu.eax);
    //printf("pf:%d\n", cpu.eflags._pf);
    cpu.eflags._cf = 0;
    cpu.eflags._of = 0;
    
    print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"

