/*************************************************************************
	> File Name: cmp.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Sep 2016 04:15:29 AM PDT
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmp-template.h"
#undef DATA_BYTE

make_helper_v(cmp_i2r)
make_helper_v(cmp_i2rm)
make_helper_v(cmp_r2rm)
make_helper_v(cmp_rm2r)
