// FUNC 80069bf8 240 MAIN0
// MATCHING 80069bf8 240
// FLAGS -O2 -G0
// AS gnu
/* one command step of the pad driver (US func_800694FC, libpad/padmain.c, with the PAL stop-condition). Crutches: `index`/`count` register pins and the $a0 clobber only steer register allocation. */
#include "padlocal.h"

void FUN_80069bf8(padPort* port)
{
    int* index = &D_800981F0;
    register int count asm("$3");
    register int result;
    register int (*next)(void) asm("$2");
    asm("" ::: "$4");
    count = *index;
    next = D_80098230[count];
    *index = count + 1;
    result = next();
    if (result >= 0) {
        if (D_800981F0 != 0 && (D_800981F0 != 3 || *port->response_3c != 0x80)) {
            setRC2wait(60);
            if (!_padClrIntSio0()) {
                D_800981AC(-3);
            }
        }
        if (D_800981F0 >= 5) {
            D_800981F0--;
        }
    } else {
        D_800981AC(result);
    }
}
