#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me*******");
	if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 1 && MSB(result) == 1)
        cpu.eflags._of = 1;
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 0 && MSB(result) == 0)
        cpu.eflags._of = 1;
    else
        cpu.eflags._of = 0;

    cpu.eflags._zf = !result;
    cpu.eflags._sf = MSB(result);
    if(op_dest->val >= op_src->val)
        cpu.eflags._cf = 0;
    else
        cpu.eflags._cf = 1;
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
