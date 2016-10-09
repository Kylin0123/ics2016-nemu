/*************************************************************************
	> File Name: sbb.c
	> Author: 
	> Mail: 
	> Created Time: Wed 28 Sep 2016 05:28:39 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "sbb-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "sbb-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sbb-template.h"
#undef DATA_BYTE

//make_helper_v(sbb_i2r)
make_helper_v(sbb_i2a)
make_helper_v(sbb_i2rm)
make_helper_v(sbb_si2rm)
make_helper_v(sbb_r2rm)
make_helper_v(sbb_rm2r)
