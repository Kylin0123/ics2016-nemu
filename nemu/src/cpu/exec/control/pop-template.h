/*************************************************************************
	> File Name: pop-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Sep 2016 11:26:48 PM PDT
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
    MEM_W(cpu.ebp, cpu.esp);
    cpu.esp += 4;
}

#if DATA_BYTE == 1 || DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(pop_ebp_, SUFFIX)){
    do_execute();
    return 1;
}
#endif

#include "cpu/exec/template-end.h"
