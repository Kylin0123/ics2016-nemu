/*************************************************************************
	> File Name: cltd.c
	> Author: 
	> Mail: 
	> Created Time: Sat 08 Oct 2016 11:51:35 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

make_helper(cltd){
    if(ops_decoded.is_operand_size_16){
        if(cpu.gpr[0]._16 >> 15 == 1)
            cpu.gpr[2]._16 = 0xffff;
        else
            cpu.gpr[2]._16 = 0;
    }
    else{
        if(cpu.eax >> 31 == 1)
            cpu.edx = 0xffffffff;
        else
            cpu.edx = 0;
    }
    return 1;
}
