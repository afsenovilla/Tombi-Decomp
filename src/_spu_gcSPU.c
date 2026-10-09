// FUNC 80075c78 776 MAIN0
// MATCHING 80075c78 776
// Portado de psx_tomba (psyq/libspu/s_m_m.c, _spu_gcSPU); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

extern int _spu_AllocLastNum;

long SpuMalloc(long size);

void _spu_gcSPU(void) {
    int i, j;

    for (i = 0; i <= _spu_AllocLastNum;) {
        if (_spu_memList[i].addr & 0x80000000) {
            for (j = i + 1;; j++) {
                if (i) {
                }
                if (_spu_memList[j].addr != 0x2FFFFFFF) {
                    break;
                }
            }
            if ((_spu_memList[j].addr & 0x80000000) &&
                ((_spu_memList[j].addr & 0x0FFFFFFF) ==
                 (_spu_memList[i].addr & 0x0FFFFFFF) + _spu_memList[i].size)) {
                _spu_memList[j].addr = 0x2FFFFFFF;
                _spu_memList[i].size += _spu_memList[j].size;
                continue;
            }
        }
        i++;
    }
    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (_spu_memList[i].size == 0) {
            _spu_memList[i].addr = 0x2FFFFFFF;
        }
    }
    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (_spu_memList[i].addr & 0x40000000) {
            break;
        }
        for (j = i + 1; j <= _spu_AllocLastNum; j++) {
            if (_spu_memList[j].addr & 0x40000000) {
                break;
            }
            if ((_spu_memList[j].addr & 0x0FFFFFFF) <
                (_spu_memList[i].addr & 0x0FFFFFFF)) {
                u32 swapAddr = _spu_memList[i].addr;
                u32 swapSize = _spu_memList[i].size;
                _spu_memList[i].addr = _spu_memList[j].addr;
                _spu_memList[i].size = _spu_memList[j].size;
                _spu_memList[j].addr = swapAddr;
                _spu_memList[j].size = swapSize;
            }
        }
    }
    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (_spu_memList[i].addr & 0x40000000) {
            break;
        }
        if (_spu_memList[i].addr == 0x2FFFFFFF) {
            _spu_memList[i].addr = _spu_memList[_spu_AllocLastNum].addr;
            _spu_memList[i].size = _spu_memList[_spu_AllocLastNum].size;
            _spu_AllocLastNum = i;
            break;
        }
    }
    for (i = _spu_AllocLastNum - 1; i >= 0; i--) {
        if (!(_spu_memList[i].addr & 0x80000000)) {
            break;
        }
        _spu_memList[i].addr &= 0x0FFFFFFF;
        _spu_memList[i].addr |= 0x40000000;
        _spu_memList[i].size += _spu_memList[_spu_AllocLastNum].size;
        _spu_AllocLastNum = i;
    }
}
