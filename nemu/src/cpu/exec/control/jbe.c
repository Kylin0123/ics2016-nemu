/*************************************************************************
	> File Name: jbe.c
	> Author: 
	> Mail: 
	> Created Time: Fri 23 Sep 2016 09:12:13 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jbe-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jbe-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jbe-template.h"
#undef DATA_BYTE

make_helper_v(jbe_si)
