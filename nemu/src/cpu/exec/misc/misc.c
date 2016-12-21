#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
#include "device/i8259.h"

void raise_intr(uint8_t NO);

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}

make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}

make_helper(lgdt) {
    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    int len = load_addr(eip + 1, &m, op_src);
    cpu.gdtr.limit = lnaddr_read(op_src->addr, 2);
    cpu.gdtr.base = lnaddr_read(op_src->addr + 2, 4);
    print_asm("lgdt 0x%2x, 0x%x", cpu.gdtr.limit, cpu.gdtr.base);
    return len + 1;
}

make_helper(lidt) {
    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    int len = load_addr(eip + 1, &m, op_src);
    cpu.idtr.limit = lnaddr_read(op_src->addr, 2);
    cpu.idtr.base = lnaddr_read(op_src->addr + 2, 4);
    print_asm("lidt 0x%x, 0x%x", cpu.idtr.limit, cpu.idtr.base);
    return len + 1;
}

make_helper(std) {
    cpu.eflags._df = 1;
    print_asm("std");
    return 1;
}

make_helper(cld) {
    cpu.eflags._df = 0;
    print_asm("cld");
    return 1;
}

make_helper(cli) {
    cpu.eflags._if = 0;
    print_asm("cli");
    return 1;
}

make_helper(sti) {
    cpu.eflags._if = 1;
    print_asm("sti");
    return 1;
}

make_helper(hlt) {
    assert(cpu.eflags._if == 1);
    while(1){
        //Log("intr:%d eflags:%d",cpu.INTR,cpu.eflags._if);
        if(cpu.INTR & cpu.eflags._if){
            break;
        }

    }
    return 1;
}




