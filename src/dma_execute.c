// FUNC 8006801c 444 MAIN0
// MATCHING 8006801c 444
// HEADERS tomba24
// Portado de psx_tomba (../psyq/libcd/c_011.c, dma_execute); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

typedef char Result_t[8];
typedef struct {
    s32 madr;
    s32 bcr;
    s32 chcr;
    s32 unk;
} DMA_CONTROL;
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
extern volatile u8* D_8009704C;
extern volatile s32* D_80097064;
extern volatile u8* D_80097068;
extern volatile u8* D_800963B8;
extern volatile u8* D_800963BC;
extern bool D_8009C8AC;
extern s32 D_800A3340;

void StCdInterrupt(void);

void mem2mem(s32* dst, s32* src, u32 num);

void dma_execute(s32 id, void* addr, u32 blocks, u32 size, u32 control,
                 u_char arg5, u32 arg6) {
    s32 i;
    s32 temp;
    s32 temp2;
    u8 v;
    volatile s32 ret;
    volatile s32* ptr;

    for (i = 0; *(volatile u32*)(0x1F801088 + (id << 4)) & 0x01000000; i++) {
        if (i == 0x10000) {
            printf("DMA STATUS ERROR %x\n",
                   *(volatile u32*)(0x1F801088 + (id << 4)));
            break;
        }
    }
    if (arg5 == 1) {
        u8* q = (u8*)D_80097068;
        v = q[2];
        v |= 1 << id;
        q[2] = v;
    } else {
        u8* q = (u8*)D_80097068;
        v = q[2];
        v &= ~(1 << id);
        q[2] = v;
    }
    do {
        ret = *(volatile s32*)D_80097068;
    } while (0);
    temp2 = id << 2;
    temp2 = temp2 + 3;
    temp = 1;
    temp = temp << temp2;
    ptr = &((volatile DMA_CONTROL*)0x1F801080)[id].madr;
    temp2 = *D_80097064;
    temp2 |= temp;
    *D_80097064 = temp2;
    *ptr++ = (s32)addr;
    *ptr++ = (blocks << 16) | size;
    while (!(*D_8009704C & 0x40)) {
    }
    *ptr = control;
    do {
        ret = *ptr;
    } while (0);
}
