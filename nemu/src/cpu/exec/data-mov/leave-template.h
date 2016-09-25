/*************************************************************************
	> File Name: leave-template.h
	> Author: 
	> Mail: 
	> Created Time: Sat 24 Sep 2016 11:27:02 AM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute(){
    cpu.esp = cpu.ebp;
    cpu.ebp = MEM_R(cpu.esp);
    cpu.esp += 4;
}

make_helper(leave){
    do_execute();
    return 1;
}

#include "cpu/exec/template-end.h"


