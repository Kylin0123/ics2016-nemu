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

typedef struct {
    Cache_block cache_block[128][8];
    int (*read)(Cache_block* this, hwaddr_t addr, uint32_t *data, size_t len);
    int (*write)(Cache_block* this, hwaddr_t addr, uint32_t data, size_t len);
} Cache;

int read_cache(Cache* this, hwaddr_t addr, uint32_t *data, size_t len){
    uint32_t temp_tag = addr >> 13;
    temp_tag &= 0x7ffff;
    uint32_t temp_group = addr >> 6;
    temp_group &= 0x7f;
    uint32_t temp_addr = addr & 0x3f;
    int i;
    for(i = 0; i < 8; i++){
        if(this->cache_block[temp_group][i].tag == temp_tag){
            if(this->cache_block[temp_group][i].valid_bit == 1){
                memcpy(data, &this->cache_block[temp_group][i].data[temp_addr], len);
                return 1;
            }
        }
    }
    return 0;
}

int write_cache(Cache* this, hwaddr_t addr, uint32_t data, size_t len){
   return 0;
}

