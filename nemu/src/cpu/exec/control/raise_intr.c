/*************************************************************************
	> File Name: raise_intr.c
	> Author: 
	> Mail: 
	> Created Time: Thu 08 Dec 2016 10:20:42 PM CST
 ************************************************************************/
#include "cpu/exec/helper.h"
#include <setjmp.h>

extern jmp_buf jbuf;

typedef struct myGateDescriptor {
    union{
        struct{
            uint32_t offset_15_0        : 16;
            uint32_t segement           : 16;
        };
        uint32_t val1;
    };
    union{
        struct{
            uint32_t pad0               : 8;
            uint32_t type               : 4;
            uint32_t system             : 1;
            uint32_t privilege_level    : 2;
            uint32_t present            : 1;
            uint32_t offset_31_16       : 16;
        };
        uint32_t val2;
    };
} myGateDesc;

void raise_intr(uint8_t NO) {
    /*push eflags*/
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, R_SS, cpu.eflags.val);

    /*set flags*/
    cpu.eflags._if = 0;
    cpu.eflags._tf = 0;

    /*push cs sreg register*/
    cpu.esp -= 4;
    swaddr_write(cpu.eip, 4, R_SS, cpu.cs);

    /*push eip register*/
    cpu.esp -= 4;
    swaddr_write(cpu.eip, 4, R_SS, cpu.eip);

    print_asm("int 0x%x", NO);
    Assert(NO <= cpu.idtr.limit, "NO %d is out if range\n", NO);
    uint32_t base = cpu.idtr.base;
    uint32_t addr = base + 8 * NO;
    
    myGateDesc gate;
    gate.val1 = lnaddr_read(addr, 4);
    gate.val2 = lnaddr_read(addr + 4, 4);

    uint32_t offset = (gate.offset_31_16 << 16) | gate.offset_15_0;
    
    cpu.eip = (gate.segement << 16) + offset;

    /*jmp back to cpu_exec()*/
    longjmp(jbuf, 1);
}
