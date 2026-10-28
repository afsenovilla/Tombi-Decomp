// FUNC 80073eac 180 MAIN0
// MATCHING 80073eac 180
// Ported from dcb_decomp src/main/psyq/libsnd_vm_stav.c (MIT, (c) 2026 ReGame Labs), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
#include "common.h"
#include "psyq/libsnd.h"

typedef struct SvmCur {
    /* 0x00 */ char tones;
    /* 0x01 */ u_char vab;
    /* 0x02 */ char note;
    /* 0x03 */ u8 unk3[3];
    /* 0x06 */ u_char program;
    /* 0x07 */ char prog;
} SvmCur;

extern SvmCur D_8009F120;
extern VagAtr *D_8009D68C;

u_char _SsVmSelectToneAndVag(u_char *tones, u_char *vags) {
    char i;
    u_char n = 0;

    for (i = 0; i < D_8009F120.tones; i++) {
        if (D_8009D68C[i + D_8009F120.prog * 16].min <= D_8009F120.note &&
            D_8009D68C[i + D_8009F120.prog * 16].max >= D_8009F120.note) {
            vags[n] = D_8009D68C[i + D_8009F120.prog * 16].vag; /* port: dcb reads a byte, PAL lhu */
            tones[n++] = i;
        }
    }
    return n;
}

