/*************************************************************************
	> File Name: page_translate.c
	> Author: 
	> Mail: 
	> Created Time: Fri 25 Nov 2016 07:05:38 PM CST
 ************************************************************************/

#include"nemu.h"

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
    //assert(pde.present == 1);
    //assert(pte.present == 1);
    return pte;
}

hwaddr_t page_translate(lnaddr_t addr){
    PTE pte = page_fetch(addr);
    return (pte.page_frame << 12) + (addr & 0xfff);
}

