/*************************************************************************
	> File Name: movz.c
	> Author: 
	> Mail: 
	> Created Time: Sat 01 Oct 2016 04:42:02 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "movz-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movz-template.h"
#undef DATA_BYTE

make_helper_v(movz_rmb2r)
