/*************************************************************************
	> File Name: jle.c
	> Author: 
	> Mail: 
	> Created Time: Sun 25 Sep 2016 11:32:50 AM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jle-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jle-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jle-template.h"
#undef DATA_BYTE

make_helper_v(jle_si)
