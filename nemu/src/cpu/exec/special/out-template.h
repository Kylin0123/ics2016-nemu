/*************************************************************************
	> File Name: out-template.h
	> Author: 
	> Mail: 
	> Created Time: Wed 14 Dec 2016 05:06:28 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"
#include "device/port-io.h"

#define instr out

make_helper(concat(out_a2i_, SUFFIX)){
    uint8_t addr = instr_fetch(eip + 1, 1);
    DATA_TYPE val = REG(R_EAX);
    pio_write(addr, DATA_BYTE, val);
    print_asm("out 0x%x", val);
    return 2;
}

make_helper(concat(out_a2d_, SUFFIX)){
    uint16_t addr = cpu.edx;
    DATA_TYPE val = REG(R_EAX);
    pio_write(addr, DATA_BYTE, val);
    print_asm("out 0x%x", val);
    return 1;
}

#include "cpu/exec/template-end.h"
