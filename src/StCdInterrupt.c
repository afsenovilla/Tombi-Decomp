// FUNC 80067678 2416 MAIN0
// MATCHING 80067678 2416
// Portado de psx_tomba (psyq/libcd/c_011.c, StCdInterrupt); licencia MIT del proyecto original.
#include "gv6.h"
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

typedef char Result_t[8];
extern volatile s32* D_8009705C;
extern volatile s32* D_80097060;
extern volatile s32* D_8009706C;
extern volatile s32* D_8009707C;
extern s32 D_80097094;
extern volatile u16* D_8009BF70;
extern s32 D_8009C334;
extern s16 D_8009C90C;
extern s32 ST_MODE;
extern s32 D_8009C914;
extern void (*ST_FUNC2)(void);
extern s32 D_8009D5F4;
extern s32 ST_START_FRAME;
extern s32 D_8009F7E0;
extern s32 D_800A2264;
extern s32 D_800A2268;
extern s32 D_800A3CC0;
extern u32 ST_END_FRAME;
extern s32 ST_MASK;
extern u16* D_800A3F00;
extern StHEADER* ST_RING_ADDR;
extern volatile u8* D_8009704C;
extern volatile u8* D_80097054;
extern volatile u8* D_80097058;
extern bool D_8009D544;
extern s32 ST_RING_SIZE;

void StCdInterrupt(void) {
    volatile s16 subroutine_arg8[4];
    CdlLOC loc;
    Result_t result;
    u32* var_a1;
    s32 var_t0;
    u32* var_a0;
    u32 var_v1_2;
    u32 var_v1_3;
    u8* var_v1;

    if (GV6.ia0 == 1) {
        return;
    }
    if ((ST_MODE != 0) && (*D_8009706C & 0x01000000)) {
        D_8009D544 = true;
        if (D_800A3CC0 != 0) {
            D_8009F7E0++;
        }
        D_80097094 = 1;
        return;
    }
    if (CdReady(1, &result) == CdlDiskError) {
        return;
    }
    subroutine_arg8[1] = result[0];
    subroutine_arg8[2] = result[1];
    if (subroutine_arg8[1] & 4) {
        D_80097094 = 3;
        return;
    }
    D_8009BF70 = (u16*)&ST_RING_ADDR[D_800A2264];
    if (D_8009BF70[0] != 0) {
        if (D_800A3CC0 != 0) {
            D_8009F7E0++;
        }
        D_80097094 = 4;
        return;
    }
    *D_8009704C = 0;
    *D_80097058 = 0;
    *D_8009704C = 0;
    *D_80097058 = 0x80;
    *D_8009705C = 0x20943;
    *D_80097060 = 0x1323;
    if (D_8009C914 == 0) {
        var_v1 = (u8*)&subroutine_arg8[4];
        do {
            *var_v1++ = *D_80097054;
        } while (var_v1 < &subroutine_arg8[6]);
        for (var_v1_2 = 0; var_v1_2 < 8; var_v1_2++) {
            *D_80097054;
        }
    }
    var_t0 = 0x11000000;
    if (D_800A3CC0 != 0) {
        mem2mem(D_8009BF70, D_800A3CC0 + (D_8009F7E0 << 0xB), 8, 0);
    } else {
        dma_execute(3, D_8009BF70, 0, 8, var_t0, 0, 0);
    }
    while (*D_8009707C & 0x01000000) {
    }
    ((StHEADER*)D_8009BF70)->loc = loc;
    *D_8009705C = 0x20843;
    *D_80097060 = 0x1325;
    if ((ST_MASK == 1) && (ST_START_FRAME != 0)) {
        if (ST_START_FRAME != D_8009BF70[4]) {
            D_8009BF70[0] = 0;
            if (D_800A3CC0 != 0) {
                D_8009F7E0++;
            }
            return;
        }
        ST_MASK = 0;
    }
    if ((D_8009BF70[0] != 0x160) || (((D_8009BF70[1] >> 0xA) & 0x1F) != GV6.i94)) {
        if (D_800A3CC0 != 0) {
            D_8009F7E0 = 0;
        } else {
            D_8009BF70[0];
        }
        D_80097094 = 5;
        D_8009BF70[0] = 0;
        return;
    }
    if ((D_8009C90C != D_8009BF70[2]) || ((D_8009C334 != 0) && (D_8009C334 != D_8009BF70[4]))) {
        D_8009C334 = 0;
        D_8009C90C = 0;
        init_ring_status(D_800A2268, D_800A2264 - D_800A2268);
        D_800A2264 = D_800A2268;
        D_8009BF70[0] = 0;
        if (D_800A3CC0 != 0) {
            D_8009F7E0++;
        }
        D_80097094 = 6;
        return;
    }
    if (D_8009BF70[2] == 0) {
        D_8009C90C = 0;
        D_8009C334 = D_8009BF70[4];
        if ((ST_END_FRAME != 0) && (D_8009C334 >= ST_END_FRAME)) {
            D_8009C334 = 0;
            D_8009C90C = 0;
            init_ring_status(D_800A2268, D_800A2264 - D_800A2268);
            D_800A2264 = D_800A2268;
            D_8009BF70[0] = 0;
            ST_MASK = 1;
            if (ST_FUNC2 != NULL) {
                ST_FUNC2();
            }
            if (D_800A3CC0 != 0) {
                D_8009F7E0++;
            }
            D_80097094 = 7;
            return;
        }
        if ((u32) (ST_RING_SIZE - D_800A2264 - 1) < D_8009BF70[3]) {
            if (ST_END_FRAME == 0) {
                D_8009BF70[0] = 1;
                ST_MASK = 1;
                if (ST_FUNC2 != NULL) {
                    ST_FUNC2();
                }
                if (D_800A3CC0 != 0) {
                    D_8009F7E0++;
                }
                D_80097094 = 8;
                return;
            }
            if ((short)ST_RING_ADDR->id != 0) {
                D_8009BF70[0] = 0;
                if (D_800A3CC0 != 0) {
                    D_8009F7E0++;
                }
                D_80097094 = 9;
                return;
            }
            D_8009BF70[0] = 1;
            var_a1 = ST_RING_ADDR;
            var_a0 = D_8009BF70;
            D_800A2264 = 0;
            for (var_v1_3 = 0; var_v1_3 < 8; var_v1_3++) {
                *var_a1++ = *var_a0++;
            }
            D_8009BF70 = ST_RING_ADDR;
        }
        D_800A2268 = D_800A2264;
    }
    D_80097094 = 10;
    D_8009C90C++;
    D_800A3F00 = &ST_RING_ADDR[ST_RING_SIZE] + (D_800A2264 * 0x3F);
    
    if (ST_MODE != 0) {
        var_t0 = 0x11000000;
        *D_8009705C = 0x20943;
        *D_80097060 = 0x1323;
    } else {
        *D_8009705C = 0x21020843;
        var_t0 = 0x11400100;
    }
    if ((D_8009BF70[3] - 1) == D_8009BF70[2]) {
        GV6.ia0 = 1;
        if (D_800A3CC0 != 0) {
            mem2mem(D_800A3F00, D_800A3CC0 + (D_8009F7E0 << 0xB) + 0x20, 0x1F8, 1);
            D_8009F7E0++;
        } else {
            dma_execute(3, D_800A3F00, 0, 0x1F8, var_t0, 1, 0);
        }
        D_8009C90C = 0;
        D_8009C334 = 0;
        GV6.i94 = D_8009D5F4;
    } else {
        if (D_800A3CC0 != 0) {
            mem2mem(D_800A3F00, D_800A3CC0 + (D_8009F7E0 << 0xB) + 0x20, 0x1F8, 0);
            D_8009F7E0++;
        } else {
            dma_execute(3, D_800A3F00, 0, 0x1F8, var_t0, 0, 0);
        }
    }
    *D_80097060 = 0x1325;
    D_8009BF70[0] = 3;
    D_800A2264 += 1;
    if ((D_800A3CC0 != 0) && (GV6.ia0 != 0)) {
        data_ready_callback();
    }
}

void mem2mem(s32* dst, s32* src, u32 num);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libcd/c_011", dma_execute);
