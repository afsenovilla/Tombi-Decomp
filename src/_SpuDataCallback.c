// FUNC 800758ac 36 MAIN0
// MATCHING 800758ac 36
// Portado de psx_tomba (psyq/libspu/s_dcb.c, _SpuDataCallback); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

void _SpuDataCallback(void (*arg0)()) { DMACallback(4, arg0); }
