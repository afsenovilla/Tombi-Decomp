// VERSION BETA_980203 beta980203 80067C50 200
// AS.BETA_980203 gnu
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/func_800694FC_scus94236.c (func_800694FC_scus94236, twin score 0.900) [-O2 -G0] (tools/demo_sweep.py)
// Portado de psx_tomba (psyq/libpad/padmain.c, func_800694FC); licencia MIT del proyecto original.
#include "padlocal_scus94236.h"
void func_800694FC_scus94236(padPort* port)
{
    register int* index asm("$5") = &_padSioState;
    register int count;
    register int result;
    register int (*next)(void) asm("$2");
    count = *index;
    next = padIntFunc[count];
    *index = count + 1;
    result = next();
    if (result >= 0) {
        if (_padSioState) {
            setRC2wait(60);
            if (!_padClrIntSio0()) {
                _padFuncNextPort(-3);
            }
        }
        if (_padSioState >= 5) {
            _padSioState--;
        }
    } else {
        _padFuncNextPort(result);
    }
}
#endif
