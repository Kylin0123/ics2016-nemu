/*************************************************************************
	> File Name: adc-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 27 Sep 2016 08:35:11 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
    DATA_TYPE result = op_dest->val + op_src->val + cpu.eflags._cf;
    OPERAND_W(op_dest, result);
    //todo : eflags
    cpu.eflags._zf = !result;
    cpu.eflags._sf = MSB(result);
    if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 0 && MSB(result) == 0){
        cpu.eflags._cf = 0;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 0 && MSB(result) == 1){
        cpu.eflags._cf = 0;
        cpu.eflags._of = 1;
    }
    else if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 1 && MSB(result) == 0){
        cpu.eflags._cf = 1;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 1 && MSB(result) == 1){
        cpu.eflags._cf = 0;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 0 && MSB(result) == 0){
        cpu.eflags._cf = 1;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 0 && MSB(result) == 1){
        cpu.eflags._cf = 0;
        cpu.eflags._of = 0;
    }
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 1 && MSB(result) == 0){
        cpu.eflags._cf = 1;
        cpu.eflags._of = 1;
    }
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 1 && MSB(result) == 1){
        cpu.eflags._cf = 1;
        cpu.eflags._of = 0;
    }
}
make_instr_helper(i2r)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

