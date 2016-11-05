/*************************************************************************
	> File Name: cache2.c
	> Author: 
	> Mail: 
	> Created Time: Sat 05 Nov 2016 04:58:06 PM CST
 ************************************************************************/

 #include "common.h"

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
        }
}
