/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 06:44:36 AM PDT
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "test-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "test-template.h"
#undef DATA_BYTE

make_helper_v(test_i2r)
make_helper_v(test_i2a)
make_helper_v(test_i2rm)
make_helper_v(test_r2rm)
