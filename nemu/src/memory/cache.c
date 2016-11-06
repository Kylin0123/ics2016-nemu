/*************************************************************************
	> File Name: cache.c
	> Author: 
	> Mail: 
	> Created Time: Thu Nov  3 01:53:13 2016
 ************************************************************************/

#include "common.h"
#include <stdlib.h>

extern int32_t dram_read(hwaddr_t, size_t);
extern void dram_write(hwaddr_t, size_t, uint32_t);

extern struct Cache2 cache2;

extern uint32_t read_cache2(struct Cache2*, hwaddr_t addr, uint32_t*, size_t);
extern void write_cache2(struct Cache2*, hwaddr_t, uint32_t, uint32_t*, size_t);

typedef struct {
    uint32_t valid_bit;    //length:1
    uint32_t tag;   //length:19
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
                    *success = 1;
                    //printf("cache_tag:%x ", this->cache_block[temp_group][i].tag);
                    //printf("addr:%x ", addr);
                    //printf("tag:%x group:%x addr:%x\n",temp_tag,temp_group, temp_addr);
                    memcpy(temp, this->cache_block[temp_group][i].data, 64);
                    break;
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
                        printf("\nb1\n");    //todo:not test
                        memcpy(temp, this->cache_block[temp_group][i].data, 64);
                        memcpy(temp + 64, this->cache_block[temp_group+1][j].data, 64);
                        goto L1;
                    }
                }
            }
    }
L1: 
    if(*success == 0){
        int i;
        int flag = 0;
        int result_i;
        for(i = 0; i < 8; i++){
            if(this->cache_block[temp_group][i].valid_bit == 0){
                result_i = i;
                flag = 1;
            }
        }
        if(flag == 0)
            result_i = rand()%8;
        //printf("result_i:%d\n", result_i);
        this->cache_block[temp_group][result_i].valid_bit = 1;
        this->cache_block[temp_group][result_i].tag = temp_tag;
        uint32_t temp2[16];
        uint32_t align_addr = addr & 0xffffffc0;
        int j;
        //printf("cache:\n");
        for(j = 0; j < 16; j++){
            temp2[j] = read_cache2(&cache2, align_addr + 4*j, success, 4);
            memcpy(this->cache_block[temp_group][result_i].data + 4*j, temp2 + j, 4);
            //printf("%x ", temp2[j]);
        }
        /*printf("temp2[0]:0x%x\n", temp2[0]);
        printf("temp2[1]:0x%x\n", temp2[1]);
        printf("temp2[2]:0x%x\n", temp2[2]);
        printf("temp2[3]:0x%x\n", temp2[3]);
        */
        return unalign_rw((uint8_t*)temp2 + temp_addr, 4);
    }
    /*
    if(*success == 0){
        int i;
        for(i = 0; i < 8; i++){
            if(this->cache_block[temp_group][i].valid_bit == 0){
                this->cache_block[temp_group][i].valid_bit = 1;
                this->cache_block[temp_group][i].tag = temp_tag;
                uint32_t temp2[16];
                uint32_t align_addr = addr & 0xffffffc0;
                int j;
                for(j = 0; j < 16; j++){
                    temp2[j] = read_cache2(&cache2, addr, success, 4);
                    memcpy(this->cache_block[temp_group][i].data + 4*j, temp2 + j, 4);
                    if(align_addr + j == 0x7ffefb8){
                        printf("wooooooooooooow\n");
                        printf("dram:%x\n", dram_read(0x7ffefb8, 4));
                    }
                }
                temp = (uint8_t*)temp2;
                //memcpy( this->cache_block[temp_group][i].data, temp2, 64);
                //printf("xxxxxxxxxxxxxxxxxxxxxx:0x%x ", align_addr);
                //printf("dram:%x\n", dram_read(0x7ffefb8, 4));
                
                for(j = 0; j < 64; j++){
                    printf("%x ", temp2[j]);
                }
                printf("\n");
                
                break;
            }
        }
    }
    */
    //printf("over ");
    //printf("read_cache hit!\n");  //hit!
    return unalign_rw(temp + temp_addr, 4);
}

void write_cache(struct Cache* this, hwaddr_t addr, uint32_t data, uint32_t *success, size_t len){
    uint32_t temp_tag = addr >> 13;
    temp_tag &= 0x7ffff;
    uint32_t temp_group = addr >> 6;
    temp_group &= 0x7f;
    uint32_t temp_addr = addr & 0x3f;
    *success = 0;
    int i;
    for(i = 0; i < 8; i++){
        if(this->cache_block[temp_group][i].tag == temp_tag && this->cache_block[temp_group][i].valid_bit == 1){
            //printf("write_cache hit!\n");   //hit
            *success = 1;
            memcpy( this->cache_block[temp_group][i].data + temp_addr, &data, 4);
            //printf("data:%x",data);
            write_cache2(&cache2, addr, data, success, len);
            return;
        }
    } 
    
    /*int flag = 0;
    int result_i;
    for(i = 0; i < 8; i++){
        if(this->cache_block[temp_group][i].valid_bit == 0){
            result_i = i;
            flag = 1;
        }
    }
    if(flag == 0){
        result_i = rand()%8;
    }
    if(this->cache_block[temp_group][result_i].valid_bit == 1){
        
    }*/
    write_cache2(&cache2, addr, data, success, len);
    return;
}

