/*************************************************************************
	> File Name: seg_translate.c
	> Author: 
	> Mail: 
	> Created Time: Wed 23 Nov 2016 04:15:45 PM CST
 ************************************************************************/

#include "nemu.h"

void sreg_load(uint8_t sreg){
    uint8_t temp[8];
    int i;
    for(i = 0; i < 8; i++){
        temp[i] = lnaddr_read(cpu.gdtr.base + (cpu.sr[sreg].index << 3) + i, 1);
    }
    SegDesc *segdesc = (SegDesc *)temp;
    cpu.sr_cache[sreg].valid = true;
    cpu.sr_cache[sreg].base = (segdesc->base_31_24 << 24) | (segdesc->base_23_16 << 16) | segdesc->base_15_0;
    cpu.sr_cache[sreg].limit = (segdesc->limit_19_16 << 16) | segdesc->limit_15_0;
    //cpu.sr_cache[sreg].dpl = segdesc->privilege_level;
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg){
    if(cpu.cr0.protect_enable == 0){
        printf("aaaaaaaaaaaaaaa\n");
        return addr;
    }
    else{
        if(cpu.sr_cache[sreg].valid == 0)
            sreg_load(sreg);
        return addr + cpu.sr_cache[sreg].base;
    }
}
