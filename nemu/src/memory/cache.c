/*************************************************************************
	> File Name: cache.c
	> Author: 
	> Mail: 
	> Created Time: Thu Nov  3 01:53:13 2016
 ************************************************************************/

#include "common.h"

typedef struct {
    struct {
        uint32_t valid_bit : 1;
        uint32_t tag : 23;
    };
    uint8_t data[64];
} Cache_block;

typedef struct Cache{
    Cache_block cache_block[128][8];
    uint32_t (*read)(struct Cache* this, hwaddr_t addr, uint32_t *success, size_t len);
    void (*write)(struct Cache* this, hwaddr_t addr, uint32_t data, uint32_t *success, size_t len);
} Cache;

Cache cache;

void init_cache() {
    int i,j;
    for(i = 0; i < 128; i++)
        for(j = 0; j < 8; j++){
            cache.cache_block[i][j].valid_bit = 0;
        }
}

uint32_t read_cache(struct Cache* this, hwaddr_t addr, uint32_t *success, size_t len){
    uint32_t temp_tag = addr >> 13;
    temp_tag &= 0x7ffff;
    uint32_t temp_group = addr >> 6;
    temp_group &= 0x7f;
    uint32_t temp_addr = addr & 0x3f;
    *success = 0;
    uint8_t temp[16];
    int i;
    for(i = 0; i < 8; i++){
    printf("cccc\n");
        if(this->cache_block[temp_group][i].tag == temp_tag){
            if(this->cache_block[temp_group][i].valid_bit == 1){
                memcpy(temp, &this->cache_block[temp_group][i].data[temp_addr], len);
                *success = 1;
                if(temp_addr + len > 8){
                    memcpy(temp + 8, &this->cache_block[temp_group][i].data[temp_addr] + 8, 8);
                }
            }
        }
    }
    return unalign_rw(temp + temp_addr, 4);
}

void write_cache(struct Cache* this, hwaddr_t addr, uint32_t data, uint32_t *success, size_t len){
   return;
}

