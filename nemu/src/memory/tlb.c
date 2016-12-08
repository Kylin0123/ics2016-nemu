/*************************************************************************
	> File Name: tlb.c
	> Author: 
	> Mail: 
	> Created Time: Wed 07 Dec 2016 05:16:45 PM CST
 ************************************************************************/

#include "common.h"
#include <stdlib.h>
#include <time.h>

#define NR_LINE 64
#define ADDR_WIDTH 32
#define OFFSET_WIDTH 12
#define TAG_WIDTH (ADDR_WIDTH - OFFSET_WIDTH)


typedef union {
    struct{
        uint32_t offset : OFFSET_WIDTH;
        uint32_t tag : TAG_WIDTH;
    };
    uint32_t val;
} tlbAddr;

typedef struct{
    PTE pte;
    uint32_t tag : TAG_WIDTH;
    uint32_t valid : 1;

} tlb_block;

tlb_block TLB[NR_LINE];

void init_tlb(){
    int i;
    for(i = 0; i < NR_LINE; i++){
        TLB[i].valid = 0;
    }
}

PTE page_fetch(lnaddr_t addr);

void debug_tlb();

int hits = 0;
int miss = 0;

hwaddr_t tlb_read(lnaddr_t addr){
    //divide addr
    tlbAddr tlb_addr;
    tlb_addr.val = addr;
    //uint32_t offset = tlb_addr.offset;
    //uint32_t tag = tlb_addr.tag;
    
    //matching tag
    int i;
    for(i = 0; i < NR_LINE; i++){
        if(TLB[i].valid == 1 && TLB[i].tag == tlb_addr.tag){             //tlb hit
            printf("hits:%d %x %x\n", hits++,tlb_addr.tag,TLB[i].tag);
            return (TLB[i].pte.page_frame << 12) + tlb_addr.offset;
        }

    }
    //printf("miss:%d %x\n", miss++,tlb_addr.tag);
    //tlb miss
    /*search for i to set or to replace*/
    int result_i = 0;
    int flag = 0;
    for(i = 0; i < NR_LINE; i++){
        if(TLB[i].valid == 0){
            result_i = i;
            flag = 1;
            break;
        }
    }
    if(!flag){                              //random replace
        srand(time(0));
        result_i = rand()%NR_LINE;
    }

    TLB[result_i].valid = 1;
    TLB[result_i].tag = tlb_addr.tag;
    TLB[result_i].pte = page_fetch(addr);
    //debug_tlb();
    
    return (TLB[result_i].pte.page_frame << 12) + tlb_addr.offset;

}

void debug_tlb(){
    int i;
    for(i = 0; i < NR_LINE; i++){
        printf("%d: valid:%d tag:%d\n", i,TLB[i].valid,TLB[i].tag);
    }

}
