/*************************************************************************
	> File Name: scas.c
	> Author: 
	> Mail: 
	> Created Time: Wed 12 Oct 2016 12:11:48 AM CST
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "scas-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "scas-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "scas-template.h"
#undef DATA_BYTE

make_helper_v(scas_m)
