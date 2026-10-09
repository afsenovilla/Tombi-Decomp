// FUNC 80076e94 36 MAIN0
// MATCHING 80076e94 36
// Portado de psx_tomba (psyq/libspu/s_srv.c, SpuSetReverbVoice); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

u_long SpuSetReverbVoice(long on_off, u_long voice_bit) {
    return _SpuSetAnyVoice(on_off, voice_bit, 0xCC, 0xCD);
}
