#include "common.h"
#include <stdlib.h>

//extern struct Cache cache;
extern struct Cache2 cache2;

//extern uint32_t read_cache(struct Cache * this, hwaddr_t addr, uint32_t *success, size_t len);
//extern void write_cache(struct Cache *this, hwaddr_t addr, uint32_t data, uint32_t *success, size_t len);

extern uint32_t read_cache2(struct Cache2 * this, hwaddr_t addr, uint32_t *success2, size_t len);
extern void write_cache2(struct Cache2 *this, hwaddr_t addr, uint32_t data, uint32_t *success2, size_t len);

int32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
    
    /*uint32_t *success = malloc(sizeof(uint32_t));
    *success = 0;
    uint32_t temp = read_cache(&cache, addr, success, len);
    if(*success == 1){
        //printf("hit!");
        //printf(" temp:%x\n", temp);
        free(success);
        return temp & (~0u >> ((4 - len) << 3));
    }
    else{
        temp = read_cache2(&cache2, addr, success, len);
        if(*success == 1){
            free(success);
            return temp & (~0u >> ((4 - len) << 3));
        }
        free(success);
	    return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
    }*/

    uint32_t *success = malloc(sizeof(uint32_t));
    *success = 0;
    return read_cache2(&cache2, addr, success, len) & (~0u >> ((4 - len) << 3));
    
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
    
        printf("xxxxxxxxxxxxxxxxxx\n");
        int j;
        uint32_t temp3[16];
        for(j = 0; j < 16; j++){
            temp3[j] = dram_read((addr&0xffffffc0) + 4*j, 4);
            printf("%x ", temp3[j]);
        }
        printf("\n");
        printf("xxxxxxxxxxxxxxxxxx\n");
    
    uint32_t *success = malloc(sizeof(uint32_t));
    *success = 0;
    write_cache2(&cache2, addr, data, success, len);
    
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

