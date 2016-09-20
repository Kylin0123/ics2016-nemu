/*************************************************************************
	> File Name: cmp-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Sep 2016 02:35:15 AM PDT
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    DATA_TYPE temp = op_dest->val - cpu.eax;
    if(temp == 0)
        cpu.eflags._zf = 1;
    else
        cpu.eflags._zf = 0;
    
    if(temp < 0)
        cpu.eflags._cf = 1;
    else if(temp > 0){
        cpu.eflags._cf = 0;
    }
     /*
     printf("temp:%d\n", temp); 
     printf("op_dest:%d\n", op_dest->val); 
     printf("op_src:%d\n", op_src->val); 
    */
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
