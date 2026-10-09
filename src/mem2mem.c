// FUNC 80067fe8 52 MAIN0
// MATCHING 80067fe8 52
// Portado de psx_tomba (psyq/libcd/c_011.c, mem2mem); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

typedef char Result_t[8];
extern volatile s32* D_800963C0;
extern volatile s32* D_800963C4;
extern volatile s32* D_800963D0;
extern volatile s32* D_800963E0;
extern s32 D_800963F8;
extern volatile u16* D_8009B2D8;
extern s32 D_8009B69C;
extern s16 D_8009BC74;
extern s32 D_8009BC78;
extern s32 D_8009BC7C;
extern void (*D_8009C860)(void);
extern s32 D_8009C95C;
extern s32 D_8009C960;
extern s32 D_8009C9FC;
extern s32 D_8009CA08;
extern s32 D_8009EB48;
extern s32 D_800A15CC;
extern s32 D_800A15D0;
extern s32 D_800A3028;
extern u32 D_800A302C;
extern s32 D_800A3060;
extern u16* D_800A3268;
extern StHEADER* D_800A326C;
extern volatile u8* D_800963B0;
extern volatile u8* D_800963B8;
extern volatile u8* D_800963BC;
extern bool D_8009C8AC;
extern s32 D_800A3340;

void StCdInterrupt(void);

void mem2mem(s32* dst, s32* src, u32 num) {
    u32 i;
    for (i = 0; i < num; i++) {
        *dst++ = *src++;
    }
}

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libcd/c_011", dma_execute);
