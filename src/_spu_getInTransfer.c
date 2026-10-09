// FUNC 800773ec 24 MAIN0
// MATCHING 800773ec 24
// Portado de psx_tomba (psyq/libspu/s_it.c, _spu_getInTransfer); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

void _spu_setInTransfer(s32 arg0);

int _spu_getInTransfer(void) { return _spu_inTransfer != 1; }
