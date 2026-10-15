// FUNC 8006b62c 244 MAIN0
// MATCHING 8006b62c 244
// FLAGS -O2 -G0
// AS gnu
/* func_8006ABB4 (US psx_tomba libpad/padportd.c). */
#include "padlocal.h"

int func_8006B62C(int result) {
    register int callbackResult asm("$5") = result;
    padPort *objects;
    int ignoredResult;
    int *channelResults;
    padPort *obj;
    int index;
    padSio *control;
    int limit;
    int offset;
    int nextResult;
    int cleared;

    objects = &D_8009C038[0];
    ignoredResult = -9;
    channelResults = &D_80098204[0];

    do {
        index = D_800981EC;
        offset = index * sizeof(padPort);
        obj = (padPort *)(offset + (int)objects);

        if (callbackResult != ignoredResult) {
            cleared = callbackResult == 0;
            if (cleared) {
                channelResults[index] = 0;
            } else {
                _dirFailAuto(obj);
                func_8006ACA8(obj);
            }
        }

        asm("" : "=r"(control), "=r"(index) : "0"(D_80098244), "1"(D_800981EC));
        D_800981F0 = 0;
        asm volatile("" : : : "memory");
        control->ctrl = 0;
        limit = D_80098200;
        index += 1;
        D_800981EC = index;
        asm volatile("" : : : "memory");
        if (limit >= index) {
            nextResult = _padInitSioMode(
                (padPort *)((((index << 4) - index) << 4) + (int)objects));
        } else {
            nextResult = 1;
        }
        callbackResult = 0xFFFF;
        asm("" : "=r"(callbackResult) : "0"(callbackResult));
    } while (nextResult == 0);

    return nextResult;
}
