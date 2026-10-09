// FUNC 8006ef18 64 MAIN0
// MATCHING 8006ef18 64
// Portado de psx_tomba (psyq/libsnd/ut_roff.c, _SsTrapIntrVSync); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libsnd_i.h"

typedef struct {
    u_short currentVal;
    short : 16;
    u_short mode;
    short : 16;
    u_short targetVal;
    short : 16;
    int : 32;
} RootCounter;
extern int (*_interruptReg)[2];
extern volatile RootCounter (*_rootCounter0)[3];
extern long _interruptMasks[4];

void SsStart2(void);

void _SsTrapIntrVSync(void) {
    struct SndSeqTickEnv* env = &_snd_seq_tick_env;

    if (env->unk12) {
        env->unk12();
    }
    env->unk8();
}

void _SsSeqCalledTbyT_1per2(void);

long SetRCnt(u_long counter, u_short targetVal, long mode);

long GetRCnt(u_long spec);

long StartRCnt(u_long spec);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libsnd/ut_roff", func_8006E5F4);

long ResetRCnt(u_long spec);

