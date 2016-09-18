#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)

/*make_helper(mov_i2rm_l){
        uint32_t op_dest = instr_fetch(eip + 2, 4);
        uint32_t op_src = instr_fetch(eip + 6 , 4);
        uint32_t temp = MEM_R(op_dest);
        temp += op_src;
        MEM_W(op_dest, temp);
        return 10;
    }
*/

make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
