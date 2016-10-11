/*************************************************************************
	> File Name: jmp.c
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 08:48:45 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

make_helper_v(jmp_si)
make_helper_v(jmp_rm)
