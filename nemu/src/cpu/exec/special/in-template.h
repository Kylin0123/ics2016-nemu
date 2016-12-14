/*************************************************************************
	> File Name: in-template.h
	> Author: 
	> Mail: 
	> Created Time: Wed 14 Dec 2016 06:23:18 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"
#include "device/port-io.h"

#define instr in

make_helper(concat(in_i2a_, SUFFIX)){
    uint8_t addr = instr_fetch(eip + 1, 1);
    DATA_TYPE val = pio_read(addr, DATA_BYTE);
    REG(R_EAX) = val;
    print_asm("in 0x%x", val);
    return 2;
}

make_helper(concat(in_d2a_, SUFFIX)){
    uint16_t addr = cpu.edx;
    DATA_TYPE val = pio_read(addr, DATA_BYTE);
    REG(R_EAX) = val;
    print_asm("in 0x%x", val);
    return 1;
}

#include "cpu/exec/template-end.h"
