/*************************************************************************
	> File Name: int-template.h
	> Author: 
	> Mail: 
	> Created Time: Fri 09 Dec 2016 03:34:49 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"
#include "raise_intr.h"

#define instr Int

void raise_intr(uint8_t NO);

static void do_execute(){
    //cpu.eip += 2;
    printf("iiiiiiiiiiiint\n");
    raise_intr(op_src->val);
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
