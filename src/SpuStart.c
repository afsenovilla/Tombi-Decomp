// FUNC 80074c84 124 MAIN0
// MATCHING 80074c84 124
// Portado de psx_tomba (psyq/libspu/s_ini.c, SpuStart); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

extern s32 _spu_trans_mode, _spu_transMode, _spu_keystat, _spu_RQmask, _spu_RQvoice, _spu_env;

void _SpuInit(s32 arg0);

void SpuStart(void) {
    if (_spu_isCalled == 0) {
        _spu_isCalled = 1;
        EnterCriticalSection();
        _SpuDataCallback(_spu_FiDMA);
        _spu_EVdma = OpenEvent(HwSPU, EvSpCOMP, EvMdNOINTR, NULL);
        EnableEvent(_spu_EVdma);
        ExitCriticalSection();
    }
}
