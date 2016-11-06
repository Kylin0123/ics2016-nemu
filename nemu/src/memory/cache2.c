/*************************************************************************
	> File Name: cache2.c
	> Author: 
	> Mail: 
	> Created Time: Sat 05 Nov 2016 04:58:06 PM CST
 ************************************************************************/

 #include "common.h"
 #include <stdlib.h>

int32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

typedef struct {
    uint32_t valid_bit;
    uint32_t dirty_bit;
    uint32_t tag;
    uint8_t data[64];    
} Cache_block2;

typedef struct Cache2{
    Cache_block2 cache_block2[4096][16];
} Cache2;

Cache2 cache2;

void init_cache2(){
    int i,j;
    for(i = 0; i < 4096; i++)
        for(j = 0; j < 16; j++){
            cache2.cache_block2[i][j].valid_bit = 0;
	    cache2.cache_block2[i][j].dirty_bit = 0;
        }
}

uint32_t read_cache2(struct Cache2* this, hwaddr_t addr, uint32_t *success2, size_t len){
    uint32_t temp_tag = addr >> 18;
    temp_tag &= 0x3ffff;
    uint32_t temp_group = addr >> 6;
    temp_group &= 0xfff;
    uint32_t temp_addr = addr & 0x3f;
    *success2 = 0;
    uint8_t temp[128];
    int i;
    if(temp_addr + len  <= 64){
        for(i = 0; i < 16; i++){
            if(this->cache_block2[temp_group][i].tag == temp_tag){
                if(this->cache_block2[temp_group][i].valid_bit == 1){
                    *success2 = 1;
                    memcpy(temp, this->cache_block2[temp_group][i].data, 64);
                    break;
                }
            }
        }
    }
    else{
        int i,j;
        for(i = 0; i < 16; i++)
            for(j = 0;j < 16; j++){
                if(this->cache_block2[temp_group][i].tag == temp_tag && this->cache_block2[temp_group+1][j].tag == temp_tag){
                    if(this->cache_block2[temp_group][i].valid_bit == 1 && this->cache_block2[temp_group+1][j].valid_bit == 1){
                        *success2 = 1;
                        //printf("\nb2\n"); //not test
                        memcpy(temp, this->cache_block2[temp_group][i].data, 64);
                        memcpy(temp + 64, this->cache_block2[temp_group+1][j].data, 64);
                        /*int f;
                        for(f = 0; f < 128; f++)
                            printf("%x ", temp[f]);
                        printf("\n");
                        printf("%x\n", unalign_rw(temp + temp_addr, 4));
                        */
                        goto L2;
                    }
                }
            }
    }
    L2: 
    if(*success2 == 0){
        int i;
        int flag = 0;
        int result_i;
        for(i = 0; i < 16; i++){
            if(this->cache_block2[temp_group][i].valid_bit == 0){
                result_i = i;
                flag = 1;
            }
        }
        if(flag == 0){
            result_i = rand()%16;
        }
        if(this->cache_block2[temp_group][result_i].valid_bit == 1 && this->cache_block2[temp_group][result_i].dirty_bit == 1){
            uint32_t dram_addr = (uint32_t)((this->cache_block2[temp_group][result_i].tag << 18) | (temp_group << 6) | temp_addr);
            for(i = 0; i < 16; i++){
                uint32_t *dram_data = (uint32_t*)this->cache_block2[temp_group][result_i].data + 4*i;
                dram_write(dram_addr, 4, *dram_data);
            }
        }
        this->cache_block2[temp_group][result_i].valid_bit = 1;
        this->cache_block2[temp_group][result_i].tag = temp_tag;
        uint8_t temp2[64];
        uint32_t align_addr = addr & 0xffffffc0;
        int j;
        //printf("cache2:\n");
        //printf("addr:%x\n",addr);
        //if(addr == 0x8001bc)
            //printf("0x8001bc:%x\n", dram_read(addr, 4));
        for(j = 0; j < 64; j++){
            temp2[j] = dram_read(align_addr + j, 1);
            memcpy(this->cache_block2[temp_group][result_i].data + j, temp2 + j, 1);
            printf("%x ", temp2[j]);
        }
        printf("\n");
        //printf("\nread_cache2 miss!\n");
        
        return unalign_rw(temp2 + temp_addr, 4);
    }
    //printf("read_cache2 hit!\n"); //hit!
    return unalign_rw(temp + temp_addr, 4);
}

void write_cache2(struct Cache2* this, hwaddr_t addr, uint32_t data, uint32_t *success2, size_t len){
    uint32_t temp_tag = addr >> 18;
    temp_tag &= 0x3ffff;
    uint32_t temp_group = addr >> 6;
    temp_group &= 0xfff;
    uint32_t temp_addr = addr & 0x3f;
    *success2 = 0;
    int k;
    for(k = 0; k < 16; k++){
        if(this->cache_block2[temp_group][k].tag == temp_tag && this->cache_block2[temp_group][k].valid_bit == 1){
            *success2 = 1;
            //if(temp_addr + len > 64)
                //printf("tell me!\n");
            memcpy(this->cache_block2[temp_group][k].data + temp_addr, &data, 4);
            this->cache_block2[temp_group][k].dirty_bit = 1;
            //printf("write_cache2 hit!\n");   //hit!
            return;
        }
    }
    if(*success2 == 0){
        int i = 0;
        int flag = 0;
        int result_i;
        for(i = 0; i < 16; i++){
            if(this->cache_block2[temp_group][i].valid_bit == 0){
                result_i = i;
                flag = 1;
            }
        }
        if(flag == 0){
            result_i = rand()%16;
        }
        //printf("result_i:%d\n", result_i);
        if(this->cache_block2[temp_group][result_i].valid_bit == 1 && this->cache_block2[temp_group][result_i].dirty_bit == 1){
            printf("dirty\n");
            uint32_t dram_addr = (uint32_t)((this->cache_block2[temp_group][result_i].tag << 18) | (temp_group << 6) | temp_addr);
            for(i = 0; i < 16; i++){
                uint32_t *dram_data = (uint32_t*)this->cache_block2[temp_group][result_i].data + i;
                dram_write(dram_addr, 4, *dram_data);
            
            }
        
        }
        this->cache_block2[temp_group][result_i].valid_bit = 1;
        this->cache_block2[temp_group][result_i].tag = temp_tag;
        uint8_t temp2[64];
        uint32_t align_addr = addr & 0xffffffc0;
        //printf("addr:0x%x\n", align_addr);
        //printf("dram_read:0x%x\n", dram_read(align_addr,4));
        int j;
        for(j = 0; j < 64; j++){
            temp2[j] = dram_read(align_addr + j, 1);
            memcpy(this->cache_block2[temp_group][result_i].data + j, temp2 + j, 1);
            //printf("%x ", temp2[j]);
        }
        //printf("\n");
        memcpy(this->cache_block2[temp_group][result_i].data + temp_addr, &data, 4);
        this->cache_block2[temp_group][result_i].dirty_bit = 1;
    }
}
