#include "common.h"
#include "cpu/reg.h"

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg);

hwaddr_t page_translate(lnaddr_t addr);

//int32_t dram_read(hwaddr_t, size_t);
//void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
    return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
    
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
    cache_write(addr, len, data);

}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	//return hwaddr_read(addr, len);
    if(cpu.cr0.protect_enable == 0 || cpu.cr0.paging == 0){
        printf("no page!\n");
        return hwaddr_read(addr, len);
    }
	assert(len == 1 || len == 2 || len == 4);
    if((addr & 0xfff) + len > 0x1000)
        assert(0);
    else{
        hwaddr_t hwaddr = page_translate(addr);
        return hwaddr_read(hwaddr, len);
    }
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	//hwaddr_write(addr, len, data);
	assert(len == 1 || len == 2 || len == 4);
    if((addr & 0xfff) + len > 0x1000)
        assert(0);
    else{
        hwaddr_t hwaddr = page_translate(addr);
        hwaddr_write(hwaddr, len, data);
    }
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}

