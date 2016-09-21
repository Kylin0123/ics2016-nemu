/*************************************************************************
	> File Name: ret.c
	> Author: 
	> Mail: 
	> Created Time: Wed 21 Sep 2016 12:19:48 AM PDT
 ************************************************************************/


#define DATA_BYTE 4
#include "cpu/exec/helper.h"
#include "ret.h"

make_helper(ret){
    swaddr_t addr = cpu.esp;
    cpu.esp += 4;
    cpu.eip = addr;
    return 1;
}

