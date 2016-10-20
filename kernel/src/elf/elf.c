#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	/*const uint32_t elf_magic = 0x7f454c46;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);
    */
	/* Load each program segment */
	//panic("please implement me");
    int i;
	for(i = 0; i < elf->e_phnum; i++) {
		/* Scan the program header table, load each segment into memory */
        ph = (void *)buf + elf->e_phoff + i*elf->e_phentsize;
		if(ph->p_type == PT_LOAD) {
			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
             //nemu_assert(0);
             uint8_t mybuf[8192];
             ramdisk_read(mybuf, 0, 8192);
             nemu_assert(mybuf[4096] == 0xbd);
             nemu_assert(ph->p_offset == 0x1000);
             nemu_assert(ph->p_vaddr == 0x100000);
             nemu_assert(ph->p_filesz == 0x240);
             //nemu_assert(ph->p_align == 0x1000);
             //uint8_t mybuf[4096];
			 //ramdisk_read(mybuf, *buf + ph->p_offset, ph->p_filesz);
             int i;
             for(i = 0; i < ph->p_filesz; i++)
                memcpy( (void *)ph->p_vaddr + 2*i, &mybuf[ph->p_offset + i], 1);
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
             memset((void *)ph->p_vaddr + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);
#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t cur_brk, max_brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(cur_brk < new_brk) { max_brk = cur_brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;
    //nemu_assert(entry == 0x8000ca);
#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif
    
	return entry;
}
