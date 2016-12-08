/*************************************************************************
	> File Name: page_translate.c
	> Author: 
	> Mail: 
	> Created Time: Fri 25 Nov 2016 07:05:38 PM CST
 ************************************************************************/

#include"nemu.h"

hwaddr_t tlb_read(lnaddr_t addr);

typedef union{
    struct{
        uint32_t offset : 12;
        uint32_t page : 10;
        uint32_t dir : 10;
    };
    uint32_t val;
} lineAddr;

PTE page_fetch(lnaddr_t addr){
    lineAddr lnaddr;
    lnaddr.val = addr;
    PDE pde;
    pde.val = hwaddr_read( (cpu.cr3.page_directory_base << 12) + lnaddr.dir * 4, 4);
    PTE pte;
    pte.val = hwaddr_read( (pde.page_frame << 12) + lnaddr.page * 4, 4);
    Assert(pde.present == 1, "pde.val = 0x%x\n", pde.val);
    Assert(pte.present == 1, "pte.val = 0x%x\n", pte.val);
    return pte;
}

hwaddr_t page_translate(lnaddr_t addr){
    if(!(cpu.cr0.paging == 1 && cpu.cr0.protect_enable == 1))
        return addr;
    return tlb_read(addr);
}

