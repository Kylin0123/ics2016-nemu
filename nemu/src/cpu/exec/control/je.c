/*************************************************************************
	> File Name: je.c
	> Author: 
	> Mail: 
	> Created Time: Mon 19 Sep 2016 07:30:23 AM PDT
 ************************************************************************/

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "je-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "je-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "je-template.h"
#undef DATA_BYTE

make_helper_v(je_rel)
