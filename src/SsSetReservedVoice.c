// FUNC 800745dc 48 MAIN0
// MATCHING 800745dc 48
// Portado de psx_tomba (psyq/libsnd/vs_srv.c, SsSetReservedVoice); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

char SsSetReservedVoice(char voices) {
    if ((voices >= 0x19) || (voices == 0)) {
        return -1;
    }
    spuVmMaxVoice = voices;
    return spuVmMaxVoice;
}
