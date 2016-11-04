/*************************************************************************
	> File Name: cache.c
	> Author: 
	> Mail: 
	> Created Time: Thu Nov  3 01:53:13 2016
 ************************************************************************/

#include "common.h"

extern int32_t dram_read(hwaddr_t, size_t);

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
    uint8_t temp[128];
    int i;
    //printf("%d\n", this->cache_block[0][0].valid_bit);
    if(temp_addr + len  <= 64){
        for(i = 0; i < 8; i++){
            if(this->cache_block[temp_group][i].tag == temp_tag){
                if(this->cache_block[temp_group][i].valid_bit == 1){
                    //printf("aaaa\n");
                    memcpy(temp, this->cache_block[temp_group][i].data, 64);
                    *success = 1;
                }
            }
        }
    }
    else{
        int i,j;
        for(i = 0; i < 8; i++)
            for(j = 0;j <8; j++){
                if(this->cache_block[temp_group][i].tag == temp_tag && this->cache_block[temp_group+1][j].tag == temp_tag){
                    if(this->cache_block[temp_group][i].valid_bit == 1 && this->cache_block[temp_group+1][j].valid_bit == 1){
                        *success = 1;
                        memcpy(temp, this->cache_block[temp_group][i].data, 64);
                        memcpy(temp + 64, this->cache_block[temp_group+1][j].data, 64);
                    }
                }
            }
    }
    if(*success == 0){
        int i;
        for(i = 0; i < 8; i++){
            if(this->cache_block[temp_group][i].valid_bit == 0){
                this->cache_block[temp_group][i].valid_bit = 1;
                uint32_t temp2[16];
                uint32_t align_addr = addr & 0xffffffc0;
                int j;
                for(j = 0; j < 16; j++){
                    temp2[j] = dram_read(align_addr + 4*j, 4);
                }
                memcpy( this->cache_block[temp_group][i].data, temp2, 64);
                for(j = 0; j < 16; j++){
                    printf("%x ", temp2[j]);
                }
                printf("\n");
                break;
            }
        }
    }
    //printf("over\n");
    return unalign_rw(temp + temp_addr, 4);
}

void write_cache(struct Cache* this, hwaddr_t addr, uint32_t data, uint32_t *success, size_t len){
   return;
}

