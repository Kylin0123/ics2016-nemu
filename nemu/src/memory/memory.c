#include "common.h"
#include "cpu/reg.h"
#include "device/mmio.h"

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg);

hwaddr_t page_translate(lnaddr_t addr);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
    int temp = is_mmio(addr);
    if(temp == -1)
        return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
    else
        return mmio_read(addr, len, temp);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
    int temp = is_mmio(addr);
    if(temp == -1)
        cache_write(addr, len, data);
    else
        mmio_write(addr, len, data, temp);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	//return hwaddr_read(addr, len);
	assert(len == 1 || len == 2 || len == 4);
    if((addr & 0xfff) + len > 0x1000){                /*cross pages*/
        //assert(0);
        uint32_t off = addr & 0xfff;
        hwaddr_t hwaddr1 = page_translate(addr);                    /*page 1*/
        hwaddr_t hwaddr2 = page_translate(addr + 0x1000 - off);     /*page 2*/
        return hwaddr_read(hwaddr1, 0x1000 - off) + (hwaddr_read(hwaddr2, len - 0x1000 + off) << (0x1000 - off) * 8);
    }
    else{
        hwaddr_t hwaddr = page_translate(addr);
        return hwaddr_read(hwaddr, len);
    }
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	//hwaddr_write(addr, len, data);
	assert(len == 1 || len == 2 || len == 4);
    if((addr & 0xfff) + len > 0x1000){               /*cross pages*/
        //assert(0);
        uint32_t off = addr & 0xfff;
        hwaddr_t hwaddr1 = page_translate(addr);
        hwaddr_t hwaddr2 = page_translate(addr + 0x1000 - off);
        uint32_t len1 = 0x1000 - off;
        uint32_t len2 = len - len1;
        hwaddr_write(hwaddr1, len1, data);
        hwaddr_write(hwaddr2, len2, data >> (len1 * 8));

    }
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

