#include "common.h"
#include "nemu.h"
#include "misc.h"
#include <stdlib.h>

/* Now we are going to make a cache!" */

#define CACHE_WIDTH 16    // 64KB
#define BLO_WIDTH 6       // 64B
#define WAY_WIDTH 3       // 8 WAY
#define SET_WIDTH (CACHE_WIDTH - BLO_WIDTH - WAY_WIDTH)

#define DRAM_WIDTH 27     // 128MB
#define TAG_WIDTH (DRAM_WIDTH - BLO_WIDTH - SET_WIDTH)

typedef union{
    struct{
        uint32_t blo      : BLO_WIDTH;
        uint32_t set      : SET_WIDTH;
        uint32_t tag      : TAG_WIDTH;
    };
    uint32_t addr;
} cache_addr;


#define NR_BLO (1 << BLO_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_TAG (1 << TAG_WIDTH)

//#define HW_MEM_SIZE (1 << DRAM_WIDTH)

typedef struct{
    uint8_t data[NR_BLO];
    uint32_t tag    : TAG_WIDTH;
    uint32_t valid  : 1;
} CACHE;

CACHE cache[NR_SET][NR_WAY];

extern void l2_cache_read(hwaddr_t addr, void *data);
extern void l2_cache_write(hwaddr_t addr, void *data, uint8_t *mask);
extern void init_l2_cache();

void init_cache(){
    int i, j;
    for(i=0; i < NR_SET; i++){
        for(j=0; j < NR_WAY; j++){
            cache[i][j].valid = false;
            cache[i][j].tag = 0;
        }
    }

    init_l2_cache();
}

void cache_read_data(hwaddr_t addr, void *data){
    Assert(addr < HW_MEM_SIZE, "eip: 0x%08x\nPhysical address 0x%08x is outside of the physical memory!", cpu.eip, addr);

    cache_addr temp;
    temp.addr = addr;
    uint32_t set = temp.set;
    uint32_t tag = temp.tag;
    uint32_t way;

    for(way=0; way < NR_WAY; way++){
        if((cache[set][way].tag == tag) && cache[set][way].valid){
            /* Hit, read a block into block buffer */
            memcpy(data, cache[set][way].data, NR_BLO);
            return;
        }
    }

    /* Miss, read a block data into cache */
    way = rand() % NR_WAY;
    l2_cache_read(addr, cache[set][way].data);
    cache[set][way].valid = true;
    cache[set][way].tag = tag;

    /* Again, read it into block buffer */
    memcpy(data, cache[set][way].data, NR_BLO);

}


uint32_t cache_read(hwaddr_t addr, size_t len){
    uint8_t temp[2 * NR_BLO];
    uint32_t block_off = addr & (NR_BLO - 1);

    cache_read_data(addr, temp);

    if(block_off + len > NR_BLO){
        /* data cross the block boundary */
        cache_read_data(addr + NR_BLO, temp + NR_BLO);
    }
    
    return unalign_rw(temp + block_off, 4);
}


void cache_write_data(hwaddr_t addr, void *data, uint8_t *mask){
    Assert(addr < HW_MEM_SIZE, "eip: 0x%x\nPhysical address 0x%x is outside of the physical memory!", cpu.eip, addr);

    cache_addr temp;
    temp.addr = addr;
    uint32_t set = temp.set;
    uint32_t tag = temp.tag;
    uint32_t way;

    /* Hit, write the cache */
    for(way = 0; way < NR_WAY; way++){
        if((cache[set][way].tag == tag) && cache[set][way].valid){
            memcpy_with_mask(cache[set][way].data, data, NR_BLO, mask);
        }
    }

    /* Whatever write the l2_cache */
    l2_cache_write(addr, data, mask);
}



void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
    uint32_t block_off = addr & (NR_BLO - 1);
    uint8_t temp[2 * NR_BLO];
    uint8_t mask[2 * NR_BLO];
    memset(mask, 0, 2 * NR_BLO);
    
    *(uint32_t *)(temp + block_off) = data;
    memset(mask + block_off, 1, len);

    cache_write_data(addr, temp, mask);

    if(block_off + len > NR_BLO){
        /* data cross the block boundary */
        cache_write_data(addr + NR_BLO, temp + NR_BLO, mask + NR_BLO);
    }

}


void print_cache(hwaddr_t addr){
    Assert(addr < HW_MEM_SIZE, "Physical address 0x%x is outside of the physical memory!", addr);

    cache_addr temp;
    temp.addr = addr;
    uint32_t set = temp.set;
    uint32_t tag = temp.tag;
    uint32_t way;

    /* Hit, print the cache */
    for(way = 0; way < NR_WAY; way++){
        if((cache[set][way].tag == tag) && cache[set][way].valid){
            printf("Tag: 0x%08x\n", tag);
            printf("0x");
            int i;
            uint32_t val;
            for(i=0; i<NR_BLO; i++){
                if(i%4 == 0){ printf(" "); }
                val = cache[set][way].data[i];
                printf("%02x", val);
            }
            printf("\n");
            return;
        }
    }

    /* Miss, print out the message */
    printf("No such addr in the cache\n");
}


#undef CACHE_WIDTH 
#undef BLO_WIDTH 
#undef WAY_WIDTH 
#undef SET_WIDTH 

#undef DRAM_WIDTH 
#undef TAG_WIDTH 

#undef NR_BLO 
#undef NR_WAY 
#undef NR_SET 
#undef NR_TAG 
