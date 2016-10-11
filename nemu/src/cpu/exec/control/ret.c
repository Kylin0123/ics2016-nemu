/*************************************************************************
	> File Name: ret.c
	> Author: 
	> Mail: 
	> Created Time: Wed 21 Sep 2016 12:19:48 AM PDT
 ************************************************************************/


#include "cpu/exec/helper.h"

/*#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE
*/

make_helper(ret_i_w){
    printf("esp:%x\n", cpu.esp);
    int16_t temp = swaddr_read(eip, 2);
    printf("temp:%d\n", temp);
    cpu.eip = swaddr_read(cpu.esp, 4);
    cpu.esp += 4;
    cpu.esp += temp;
    printf("esp:%x\n", cpu.esp);
    return 1;
}

make_helper(ret){
    swaddr_t addr = cpu.esp;
    swaddr_t temp = swaddr_read(addr, 4);
    cpu.esp += 4;
    cpu.eip = temp;
    return 1;
}

