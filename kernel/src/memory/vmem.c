#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PTE_VEDIO ((VMEM_ADDR + SCR_SIZE) / PAGE_SIZE + 10)

/* Use the function to get the start address of user page directory. */
PDE* get_updir();

static PTE vptable[NR_PTE] align_to_page;

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	//panic("please implement me");
    
    PDE *pdir = (PDE *)va_to_pa(get_updir);
    PTE *ptable = (PTE *)va_to_pa(vptable);
    
    memset(ptable, 0, NR_PTE_VEDIO * sizeof(PTE));
    
    uint32_t pdir_idx = VMEM_ADDR >> 22;
    uint32_t ptable_idx = (VMEM_ADDR << 10) >> 22;

    if(pdir[pdir_idx].val != 0){
        PDE * pde = (PDE *)(pdir[pdir_idx].val & 0xfffff);
        memcpy(ptable, pde, NR_PTE_VEDIO * sizeof(PTE));
    }

    pdir[pdir_idx].val = make_pde(ptable);

    uint32_t addr = VMEM_ADDR;

    int i;
    for(i = 0; i < SCR_SIZE / PAGE_SIZE + 1; i++){
        ptable[ptable_idx + i].val = make_pte(addr);
        addr += PAGE_SIZE;
    }

}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

