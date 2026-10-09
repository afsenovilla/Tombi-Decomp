// FUNC 80067678 2416 MAIN0
// MATCHING 80067678 2416
// Portado de psx_tomba (psyq/libcd/c_011.c, StCdInterrupt); licencia MIT del proyecto original.
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
extern s32 D_8009C910;
extern s32 D_8009C914;
extern void (*D_8009D4F8)(void);
extern s32 D_8009D5F4;
extern s32 D_8009D5F8;
extern s32 D_8009D694;
extern s32 D_8009D6A0;
extern s32 D_8009F7E0;
extern s32 D_800A2264;
extern s32 D_800A2268;
extern s32 D_800A3CC0;
extern u32 D_800A3CC4;
extern s32 D_800A3CF8;
extern u16* D_800A3F00;
extern StHEADER* D_800A3F04;
extern volatile u8* D_8009704C;
extern volatile u8* D_80097054;
extern volatile u8* D_80097058;
extern bool D_8009D544;
extern s32 D_800A3FD8;

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

    if (D_8009D6A0 == 1) {
        return;
    }
    if ((D_8009C910 != 0) && (*D_8009706C & 0x01000000)) {
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
    D_8009BF70 = (u16*)&D_800A3F04[D_800A2264];
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
    if ((D_800A3CF8 == 1) && (D_8009D5F8 != 0)) {
        if (D_8009D5F8 != D_8009BF70[4]) {
            D_8009BF70[0] = 0;
            if (D_800A3CC0 != 0) {
                D_8009F7E0++;
            }
            return;
        }
        D_800A3CF8 = 0;
    }
    if ((D_8009BF70[0] != 0x160) || (((D_8009BF70[1] >> 0xA) & 0x1F) != D_8009D694)) {
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
        if ((D_800A3CC4 != 0) && (D_8009C334 >= D_800A3CC4)) {
            D_8009C334 = 0;
            D_8009C90C = 0;
            init_ring_status(D_800A2268, D_800A2264 - D_800A2268);
            D_800A2264 = D_800A2268;
            D_8009BF70[0] = 0;
            D_800A3CF8 = 1;
            if (D_8009D4F8 != NULL) {
                D_8009D4F8();
            }
            if (D_800A3CC0 != 0) {
                D_8009F7E0++;
            }
            D_80097094 = 7;
            return;
        }
        if ((u32) (D_800A3FD8 - D_800A2264 - 1) < D_8009BF70[3]) {
            if (D_800A3CC4 == 0) {
                D_8009BF70[0] = 1;
                D_800A3CF8 = 1;
                if (D_8009D4F8 != NULL) {
                    D_8009D4F8();
                }
                if (D_800A3CC0 != 0) {
                    D_8009F7E0++;
                }
                D_80097094 = 8;
                return;
            }
            if ((short)D_800A3F04->id != 0) {
                D_8009BF70[0] = 0;
                if (D_800A3CC0 != 0) {
                    D_8009F7E0++;
                }
                D_80097094 = 9;
                return;
            }
            D_8009BF70[0] = 1;
            var_a1 = D_800A3F04;
            var_a0 = D_8009BF70;
            D_800A2264 = 0;
            for (var_v1_3 = 0; var_v1_3 < 8; var_v1_3++) {
                *var_a1++ = *var_a0++;
            }
            D_8009BF70 = D_800A3F04;
        }
        D_800A2268 = D_800A2264;
    }
    D_80097094 = 10;
    D_8009C90C++;
    D_800A3F00 = &D_800A3F04[D_800A3FD8] + (D_800A2264 * 0x3F);
    
    if (D_8009C910 != 0) {
        var_t0 = 0x11000000;
        *D_8009705C = 0x20943;
        *D_80097060 = 0x1323;
    } else {
        *D_8009705C = 0x21020843;
        var_t0 = 0x11400100;
    }
    if ((D_8009BF70[3] - 1) == D_8009BF70[2]) {
        D_8009D6A0 = 1;
        if (D_800A3CC0 != 0) {
            mem2mem(D_800A3F00, D_800A3CC0 + (D_8009F7E0 << 0xB) + 0x20, 0x1F8, 1);
            D_8009F7E0++;
        } else {
            dma_execute(3, D_800A3F00, 0, 0x1F8, var_t0, 1, 0);
        }
        D_8009C90C = 0;
        D_8009C334 = 0;
        D_8009D694 = D_8009D5F4;
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
    if ((D_800A3CC0 != 0) && (D_8009D6A0 != 0)) {
        data_ready_callback();
    }
}

void mem2mem(s32* dst, s32* src, u32 num);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libcd/c_011", dma_execute);
