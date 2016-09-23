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
    if(temp == 0)
        cpu.eflags._zf = 1;
    else
        cpu.eflags._zf = 0;
    
    if(temp < 0)
        cpu.eflags._cf = 1;
    else if(temp > 0){
        cpu.eflags._cf = 0;
    }
     printf("dest:%d\n", op_dest->val);
    printf("src:%d\n", op_src->val);
     printf("zf:%d\n", cpu.eflags._zf); 
     printf("cf:%d\n", cpu.eflags._cf); 
     printf("temp:%d\n", temp); 
    
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

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
