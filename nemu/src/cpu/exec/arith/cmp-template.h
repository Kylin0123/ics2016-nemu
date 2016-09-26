/*************************************************************************
	> File Name: cmp-template.h
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Sep 2016 02:35:15 AM PDT
 ************************************************************************/

#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    DATA_TYPE temp = op_dest->val - op_src->val;
    /*if(op_src->val == 0x80000000 && op_dest->val == 0x80000000)
        temp = 0;
    else if(op_src->val == 0x80000000)
        temp = 1;
    else if(op_dest->val == 0x80000000)
        temp = -1;
    */
    if(MSB(op_dest->val) == 0 && MSB(op_src->val) == 1 && MSB(temp) == 1)
        cpu.eflags._of = 1;
    else if(MSB(op_dest->val) == 1 && MSB(op_src->val) == 0 && MSB(temp) == 0)
        cpu.eflags._of = 1;
    else
        cpu.eflags._of = 0;

    cpu.eflags._zf = !temp;
    cpu.eflags._sf = MSB(temp);
    cpu.eflags._cf = MSB(temp);
    print_asm_template2();
     
    printf("dest:%x\n", op_dest->val);
    printf("src:%x\n", op_src->val);
    printf("zf:%d\n", cpu.eflags._zf); 
    printf("cf:%d\n", cpu.eflags._cf); 
    printf("temp:%x\n", temp); 
    printf("eax:%d\n", cpu.eax);
    printf("ecx:%d\n\n", cpu.ecx);
    
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(cmp_i_b2rm_, SUFFIX)){
    int len = concat(decode_rm_, SUFFIX)(eip + 1);
    op_dest->type = op_src->type;
    op_dest->reg = op_src->reg;
    op_dest->val = op_src->val;
    int len2 = decode_i_b(eip + 1 + len);
    if(DATA_BYTE == 2){
        if(op_src->val >> 7 == 0){
            op_src->val = op_src->val & 0x00ff;
        }
        else
            op_src->val = op_src->val | 0xff00;
    }
    else if(DATA_BYTE == 4){
        if(op_src->val >> 7 == 0){
            op_src->val = op_src->val & 0x000000ff;
        }
        else
            op_src->val = op_src->val | 0xffffff00;
    }
    do_execute();
    return 1 + len + len2;
}
#endif

/*#if DATA_BYTE == 4 || DATA_BYTE == 2 
make_helper(concat(cmp_i2rm_, SUFFIX)){
    DATA_TYPE imm = instr_fetch(eip + 1, 1);
    op_src->val = imm;
    op_dest->type = OP_TYPE_REG;
    op_dest->reg = R_EAX;
    op_dest->val = REG(R_EAX);
    do_execute();
    return 1 + DATA_BYTE;
}
#endif
*/
#include "cpu/exec/template-end.h"
