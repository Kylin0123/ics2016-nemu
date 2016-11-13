#include "common.h"
#include "misc.h"
#include <stdlib.h>

#define L2_CACHE_WID 22     // 4MB
#define L2_BLO_WID 6        // 64B
#define L2_WAY_WID 4        // 16 WAY
#define L2_SET_WID (L2_CACHE_WID - L2_BLO_WID - L2_WAY_WID)

#define DRAM_WID 27         // 128MB
#define L2_TAG_WID (DRAM_WID - L2_BLO_WID - L2_SET_WID)

typedef union{
    struct{
        uint32_t blo    : L2_BLO_WID;
        uint32_t set    : L2_SET_WID;
        uint32_t tag    : L2_TAG_WID;
    };
    uint32_t addr;
} l2_cache_addr;

#define L2_NR_BLO (1 << L2_BLO_WID)
#define L2_NR_WAY (1 << L2_WAY_WID)
#define L2_NR_SET (1 << L2_SET_WID)
#define L2_NR_TAG (1 << L2_TAG_WID)

#define HW_MEM_SIZE (1 << DRAM_WID)

typedef struct{
    uint8_t data[L2_NR_BLO];
    uint32_t tag   : L2_TAG_WID;
    uint32_t valid : 1;
    uint32_t dirty : 1;
} L2_CACHE;

L2_CACHE l2_cache[L2_NR_SET][L2_NR_BLO];

extern uint8_t *hw_mem;
typedef uint8_t DRAM_CACHE[L2_NR_SET][L2_NR_BLO];

void init_l2_cache(){
    int i, j;
    for(i=0; i < L2_NR_SET; i++){
        for(j=0; j < L2_NR_WAY; j++){
            l2_cache[i][j].valid = false;
            l2_cache[i][j].dirty = false;
        }
    }
}


void l2_cache_read(hwaddr_t addr, void *data){

    DRAM_CACHE *dram_cache = (void *)hw_mem;

    l2_cache_addr temp;
    temp.addr = addr;
    uint32_t set = temp.set;
    uint32_t tag = temp.tag;
    uint32_t way;

    for(way=0; way < L2_NR_WAY; way++){
        if((l2_cache[set][way].tag == tag) && l2_cache[set][way].valid){
            memcpy(data, l2_cache[set][way].data, L2_NR_BLO);
            return;
        }
    }

    way = rand() % L2_NR_WAY;

    if(l2_cache[set][way].dirty && l2_cache[set][way].valid){
        uint32_t new_tag = l2_cache[set][way].tag;
        memcpy(dram_cache[new_tag][set], l2_cache[set][way].data, L2_NR_BLO);
    } 

    memcpy(l2_cache[set][way].data, dram_cache[tag][set], L2_NR_BLO);
    l2_cache[set][way].valid = true;
    l2_cache[set][way].tag = tag;
    l2_cache[set][way].dirty = false;

    memcpy(data, l2_cache[set][way].data, L2_NR_BLO);
}



void l2_cache_write(hwaddr_t addr, void *data, uint8_t *mask){
    uint8_t block[L2_NR_BLO];
    l2_cache_read(addr, block);

    l2_cache_addr temp;
    temp.addr = addr;
    uint32_t set = temp.set;
    uint32_t tag = temp.tag;
    uint32_t way;

    for(way=0; way < L2_NR_WAY; way++){
        if(l2_cache[set][way].tag == tag && l2_cache[set][way].valid){
            memcpy_with_mask(l2_cache[set][way].data, data, L2_NR_BLO, mask);
            l2_cache[set][way].dirty = true;
            return;
        }
    }
}
