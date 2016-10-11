/*************************************************************************
	> File Name: lods.c
	> Author: 
	> Mail: 
	> Created Time: Tue 11 Oct 2016 11:39:54 PM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "lods-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "lods-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "lods-template.h"
#undef DATA_BYTE

make_helper_v(lods_m)
