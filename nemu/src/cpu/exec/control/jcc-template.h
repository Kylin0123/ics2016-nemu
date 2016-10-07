/*************************************************************************
	> File Name: jcc-template.h
	> Author: 
	> Mail: 
	> Created Time: Sun 02 Oct 2016 08:10:47 PM CST
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr je
static void do_execute(){
    if(cpu.eflags._zf == 1)
        cpu.eip += op_src->val;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jle
static void do_execute(){
    if(cpu.eflags._zf == 1 || cpu.eflags._of != cpu.eflags._sf)
        cpu.eip += op_src->val;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jbe
static void do_execute(){
    int32_t rel = op_src->val;
    //printf("jbe/rel:%d\n", rel);
    if(cpu.eflags._zf == 1 || cpu.eflags._cf == 1)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jne
static void do_execute(){
    int32_t rel = op_src->val;
    //printf("rel:%d\n", rel);
    if(cpu.eflags._zf == 0)
        cpu.eip += rel;
    //printf("eip:%x\n", cpu.eip);
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jg
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._zf == 0 && cpu.eflags._sf == cpu.eflags._of)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jl
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._sf != cpu.eflags._of)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr ja
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._cf == 0 && cpu.eflags._zf == 0)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jae
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._cf == 0)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jb
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._cf == 1)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jcxz
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.gpr[1]._16 == 0)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jge
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._sf == cpu.eflags._of)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jno
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._of == 0)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jnp
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._pf == 0)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jns
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._sf == 0)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jo
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._of == 1)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr jp
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._pf == 1)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#define instr js
static void do_execute(){
    int32_t rel = op_src->val;
    if(cpu.eflags._sf == 1)
        cpu.eip += rel;
    print_asm_template1();
}
make_instr_helper(si)
#undef instr

#include "cpu/exec/template-end.h"

