/*************************************************************************
	> File Name: push-template.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 12:51:40 AM PDT
 ************************************************************************/

#ifndef _PUSH-TEMPLATE_H
#define _PUSH-TEMPLATE_H

#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
    cpu.esp -= 4;
    MEM_W(cpu.esp, cpu.ebp);
}

//#define DATA_BYTE == 4
make_helper(concat(push_ebp_, SUFFIX)){
    do_execute();
    return 1;
}
//#endif

#include "cpu/exec/template-end.h"

#endif
