// FUNC 8006ae54 72 MAIN0
// MATCHING 8006ae54 72
// FLAGS -O2 -G0
// AS gnu
/* US func_8006A40C (libpad/padif.c) plus the PAL `*response = 0` store. */
#include "padlocal.h"

int _padIfSioRW(padPort *port) {
    D_8009822C = D_800981C4(port);
    *port->response_3c = 0;
    return _padSioRW(port, -2);
}
