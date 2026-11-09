// FUNC 80067678 2416 MAIN0
// MATCHING 80067678 2416
// Portado de psx_tomba (psyq/libcd/c_011.c, StCdInterrupt); licencia MIT del proyecto original.
#include "gv6.h"
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

typedef char Result_t[8];
extern volatile s32* c011_cd_delay;
extern volatile s32* c011_com_delay;
extern volatile s32* c011_d1_chcr;
extern volatile s32* c011_d3_chcr;
extern s32 debug_cause;
extern volatile u16* c011_header;
extern s32 Stframe_no;
extern s16 Stsector_offset;
extern s32 ST_MODE;
extern s32 StMode;
extern void (*ST_FUNC2)(void);
extern s32 StCHANNEL;
extern s32 ST_START_FRAME;
extern s32 D_8009F7E0;
extern s32 StRingIdx1;
extern s32 StRingIdx2;
extern s32 StEmu_Addr;
extern u32 ST_END_FRAME;
extern s32 ST_MASK;
extern u16* StRingBase;
extern StHEADER* ST_RING_ADDR;
extern volatile u8* c011_cd_reg0;
extern volatile u8* c011_cd_reg2;
extern volatile u8* c011_cd_reg3;
extern bool StCdIntrFlag;
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
    if ((ST_MODE != 0) && (*c011_d1_chcr & 0x01000000)) {
        StCdIntrFlag = true;
        if (StEmu_Addr != 0) {
            D_8009F7E0++;
        }
        debug_cause = 1;
        return;
    }
    if (CdReady(1, &result) == CdlDiskError) {
        return;
    }
    subroutine_arg8[1] = result[0];
    subroutine_arg8[2] = result[1];
    if (subroutine_arg8[1] & 4) {
        debug_cause = 3;
        return;
    }
    c011_header = (u16*)&ST_RING_ADDR[StRingIdx1];
    if (c011_header[0] != 0) {
        if (StEmu_Addr != 0) {
            D_8009F7E0++;
        }
        debug_cause = 4;
        return;
    }
    *c011_cd_reg0 = 0;
    *c011_cd_reg3 = 0;
    *c011_cd_reg0 = 0;
    *c011_cd_reg3 = 0x80;
    *c011_cd_delay = 0x20943;
    *c011_com_delay = 0x1323;
    if (StMode == 0) {
        var_v1 = (u8*)&subroutine_arg8[4];
        do {
            *var_v1++ = *c011_cd_reg2;
        } while (var_v1 < &subroutine_arg8[6]);
        for (var_v1_2 = 0; var_v1_2 < 8; var_v1_2++) {
            *c011_cd_reg2;
        }
    }
    var_t0 = 0x11000000;
    if (StEmu_Addr != 0) {
        mem2mem(c011_header, StEmu_Addr + (D_8009F7E0 << 0xB), 8, 0);
    } else {
        dma_execute(3, c011_header, 0, 8, var_t0, 0, 0);
    }
    while (*c011_d3_chcr & 0x01000000) {
    }
    ((StHEADER*)c011_header)->loc = loc;
    *c011_cd_delay = 0x20843;
    *c011_com_delay = 0x1325;
    if ((ST_MASK == 1) && (ST_START_FRAME != 0)) {
        if (ST_START_FRAME != c011_header[4]) {
            c011_header[0] = 0;
            if (StEmu_Addr != 0) {
                D_8009F7E0++;
            }
            return;
        }
        ST_MASK = 0;
    }
    if ((c011_header[0] != 0x160) || (((c011_header[1] >> 0xA) & 0x1F) != GV6.i94)) {
        if (StEmu_Addr != 0) {
            D_8009F7E0 = 0;
        } else {
            c011_header[0];
        }
        debug_cause = 5;
        c011_header[0] = 0;
        return;
    }
    if ((Stsector_offset != c011_header[2]) || ((Stframe_no != 0) && (Stframe_no != c011_header[4]))) {
        Stframe_no = 0;
        Stsector_offset = 0;
        init_ring_status(StRingIdx2, StRingIdx1 - StRingIdx2);
        StRingIdx1 = StRingIdx2;
        c011_header[0] = 0;
        if (StEmu_Addr != 0) {
            D_8009F7E0++;
        }
        debug_cause = 6;
        return;
    }
    if (c011_header[2] == 0) {
        Stsector_offset = 0;
        Stframe_no = c011_header[4];
        if ((ST_END_FRAME != 0) && (Stframe_no >= ST_END_FRAME)) {
            Stframe_no = 0;
            Stsector_offset = 0;
            init_ring_status(StRingIdx2, StRingIdx1 - StRingIdx2);
            StRingIdx1 = StRingIdx2;
            c011_header[0] = 0;
            ST_MASK = 1;
            if (ST_FUNC2 != NULL) {
                ST_FUNC2();
            }
            if (StEmu_Addr != 0) {
                D_8009F7E0++;
            }
            debug_cause = 7;
            return;
        }
        if ((u32) (ST_RING_SIZE - StRingIdx1 - 1) < c011_header[3]) {
            if (ST_END_FRAME == 0) {
                c011_header[0] = 1;
                ST_MASK = 1;
                if (ST_FUNC2 != NULL) {
                    ST_FUNC2();
                }
                if (StEmu_Addr != 0) {
                    D_8009F7E0++;
                }
                debug_cause = 8;
                return;
            }
            if ((short)ST_RING_ADDR->id != 0) {
                c011_header[0] = 0;
                if (StEmu_Addr != 0) {
                    D_8009F7E0++;
                }
                debug_cause = 9;
                return;
            }
            c011_header[0] = 1;
            var_a1 = ST_RING_ADDR;
            var_a0 = c011_header;
            StRingIdx1 = 0;
            for (var_v1_3 = 0; var_v1_3 < 8; var_v1_3++) {
                *var_a1++ = *var_a0++;
            }
            c011_header = ST_RING_ADDR;
        }
        StRingIdx2 = StRingIdx1;
    }
    debug_cause = 10;
    Stsector_offset++;
    StRingBase = &ST_RING_ADDR[ST_RING_SIZE] + (StRingIdx1 * 0x3F);
    
    if (ST_MODE != 0) {
        var_t0 = 0x11000000;
        *c011_cd_delay = 0x20943;
        *c011_com_delay = 0x1323;
    } else {
        *c011_cd_delay = 0x21020843;
        var_t0 = 0x11400100;
    }
    if ((c011_header[3] - 1) == c011_header[2]) {
        GV6.ia0 = 1;
        if (StEmu_Addr != 0) {
            mem2mem(StRingBase, StEmu_Addr + (D_8009F7E0 << 0xB) + 0x20, 0x1F8, 1);
            D_8009F7E0++;
        } else {
            dma_execute(3, StRingBase, 0, 0x1F8, var_t0, 1, 0);
        }
        Stsector_offset = 0;
        Stframe_no = 0;
        GV6.i94 = StCHANNEL;
    } else {
        if (StEmu_Addr != 0) {
            mem2mem(StRingBase, StEmu_Addr + (D_8009F7E0 << 0xB) + 0x20, 0x1F8, 0);
            D_8009F7E0++;
        } else {
            dma_execute(3, StRingBase, 0, 0x1F8, var_t0, 0, 0);
        }
    }
    *c011_com_delay = 0x1325;
    c011_header[0] = 3;
    StRingIdx1 += 1;
    if ((StEmu_Addr != 0) && (GV6.ia0 != 0)) {
        data_ready_callback();
    }
}

void mem2mem(s32* dst, s32* src, u32 num);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libcd/c_011", dma_execute);
