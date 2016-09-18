/*************************************************************************
	> File Name: call.c
	> Author: 
	> Mail: 
	> Created Time: Sun 18 Sep 2016 12:13:00 AM PDT
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "call-template.h"
#undef DATA_BYTE


#define DATA_BYTE 2
#include "call-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "call-template.h"
#undef DATA_BYTE

make_helper_v(call_rel)
