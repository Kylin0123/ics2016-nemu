/*************************************************************************
	> File Name: ret.c
	> Author: 
	> Mail: 
	> Created Time: Wed 21 Sep 2016 12:19:48 AM PDT
 ************************************************************************/


#include "cpu/exec/helper.h"

make_helper(ret){
    swaddr_t addr = cpu.esp;
    swaddr_t temp = swaddr_read(addr, 4);
    cpu.esp += 4;
    cpu.eip = temp;
    return 1;
}

