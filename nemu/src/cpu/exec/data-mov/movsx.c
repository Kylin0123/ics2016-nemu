/*************************************************************************
	> File Name: movS.c
	> Author: 
	> Mail: 
	> Created Time: Tue 11 Oct 2016 08:31:34 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "movsx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movsx-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movsx-template.h"
#undef DATA_BYTE

make_helper_v(movsx_m)
