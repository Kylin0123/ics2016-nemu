/*************************************************************************
	> File Name: add.c
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 11:54:34 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "add-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "add-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "add-template.h"
#undef DATA_BYTE

//make_helper_v(add_i2r)
make_helper_v(add_i2a)
make_helper_v(add_i2rm)
make_helper_v(add_r2rm)
make_helper_v(add_rm2r)
make_helper_v(add_si2rm)
