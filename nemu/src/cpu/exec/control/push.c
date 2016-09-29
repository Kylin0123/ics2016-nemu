/*************************************************************************
	> File Name: push.c
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 01:48:48 AM PDT
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

make_helper_v(push_i)
make_helper_v(push_r)
//make_helper_v(push_rm)
make_helper_v(push_m)
