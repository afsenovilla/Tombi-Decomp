// FUNC 80076228 208 MAIN0
// MATCHING 80076228 208
// Portado de psx_tomba (psyq/libspu/s_sr.c, SpuSetReverb); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

s32 SpuSetReverb(s32 on_off) {
    u16 cnt;
    switch (on_off) {
    case 0:
        _spu_rev_flag = 0;
#ifdef VERSION_PC
        cnt = read_16(0x1F801DAA, __FILE__, __LINE__); // spucnt
        cnt &= ~0x80;
        write_16(0x1F801DAA, cnt, __FILE__, __LINE__); // spucnt
#else
        cnt = _spu_RXX->rxx.spucnt;
        cnt &= ~0x80;
        _spu_RXX->rxx.spucnt = cnt;
#endif
        break;

    case 1:
        if ((_spu_rev_reserve_wa != on_off) &&
            _SpuIsInAllocateArea_(_spu_rev_offsetaddr)) {
            _spu_rev_flag = 0;
#ifdef VERSION_PC
            cnt = read_16(0x1F801DAA, __FILE__, __LINE__);
            cnt &= ~0x80;
            write_16(0x1F801DAA, cnt, __FILE__, __LINE__);
#else
            cnt = _spu_RXX->rxx.spucnt;
            cnt &= ~0x80;
            _spu_RXX->rxx.spucnt = cnt;
#endif
        } else {
            _spu_rev_flag = on_off;
#ifdef VERSION_PC
            cnt = read_16(0x1F801DAA, __FILE__, __LINE__);
            cnt |= 0x80;
            write_16(0x1F801DAA, cnt, __FILE__, __LINE__);
#else
            cnt = _spu_RXX->rxx.spucnt;
            cnt |= 0x80;
            _spu_RXX->rxx.spucnt = cnt;
#endif
        }
        break;
    }

    return _spu_rev_flag;
}

int _SpuIsInAllocateArea(unsigned arg0);

int _SpuIsInAllocateArea_(unsigned arg0);
