/*************************************************************************
	> File Name: stos.c
	> Author: 
	> Mail: 
	> Created Time: Wed 12 Oct 2016 04:30:45 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "stos-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "stos-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "stos-template.h"
#undef DATA_BYTE

make_helper_v(stos_m)
