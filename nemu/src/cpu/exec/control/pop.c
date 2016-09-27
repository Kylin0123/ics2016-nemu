/*************************************************************************
	> File Name: pop.c
	> Author: 
	> Mail: 
	> Created Time: Wed 21 Sep 2016 12:04:23 AM PDT
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

make_helper_v(pop_r)
make_helper_v(pop_rm)
