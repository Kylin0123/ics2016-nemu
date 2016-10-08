/*************************************************************************
	> File Name: movs.c
	> Author: 
	> Mail: 
	> Created Time: Sat 08 Oct 2016 11:35:41 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "movs-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movs-template.h"
#undef DATA_BYTE

make_helper_v(movs_rmb2r)
