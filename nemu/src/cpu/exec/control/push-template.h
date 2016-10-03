/*************************************************************************
	> File Name: push-template.h
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 12:51:40 AM PDT
 ************************************************************************/


#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
    if(ops_decoded.is_operand_size_16)
        cpu.esp -= 2;
    else
        cpu.esp -= 4;
    //printf("pre_esp:%x\n", cpu.esp);
    int32_t temp = op_src->val;
    MEM_W(cpu.esp, temp);
    print_asm_template1();
    if(cpu.eip == 0x100178){
        printf("op_src:%d\n", temp);
        uint32_t test = MEM_R(cpu.esp);
        printf("esp_val:%x\n", test);
    }
    //printf("esp:%x\n\n", cpu.esp);
}

make_instr_helper(i)
make_instr_helper(r)
//make_instr_helper(i2rm)
/*
make_helper(concat(push_ebp_, SUFFIX)){
    op_src->val = cpu.ebp;
    do_execute();
    return 1;
}

make_helper(concat(push_edx_, SUFFIX)){
    op_src->val = cpu.edx;
    do_execute();
    return 1;
}

make_helper(concat(push_eax_, SUFFIX)){
    op_src->val = cpu.eax;
    do_execute();
    return 1;
}
*/


make_helper(concat(push_m_, SUFFIX)){
    concat(decode_rm_, SUFFIX)(eip + 1);
    //printf("pre_op_src:%x\n", op_src->val);
    int temp = instr_fetch(eip + 2, DATA_BYTE);
    if(DATA_BYTE == 2){
        if(MSB(temp) == 0)
            temp = temp & 0x00ff;
        else
            temp = temp | 0xff00;
    }
    else if(DATA_BYTE == 4){
        if(MSB(temp) == 0)
            temp = temp & 0x000000ff;
        else
            temp = temp | 0xffffff00;
    }
    op_src->val += temp;
    do_execute();
    //printf("testValue:%d\n", DATA_BYTE);
    //printf("op_src:%x\n", op_src->val);
    //printf("temp:%d\n", temp);
    return 3;
}


#include "cpu/exec/template-end.h"
