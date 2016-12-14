/*************************************************************************
	> File Name: out.c
	> Author: 
	> Mail: 
	> Created Time: Wed 14 Dec 2016 05:25:26 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "out-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "out-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "out-template.h"
#undef DATA_BYTE

make_helper_v(out_a2i)
make_helper_v(out_a2d)

