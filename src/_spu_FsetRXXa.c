// FUNC 800756b4 164 MAIN0
// MATCHING 800756b4 164
// Portado de psx_tomba (psyq/libspu/spu.c, _spu_FsetRXXa); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/stdarg.h"
#include "libspu_internal.h"

#define SPU_CTRL_MASK_TRANSFER_DMA_READ (3 << 4)
#define SPU_CTRL_MASK_TRANSFER_DMA_WRITE (2 << 4)
extern volatile unsigned* dma_spu_madr;
extern volatile unsigned* dma_spu_bcr;
extern volatile unsigned* dma_spu_chcr;
extern int D_80097C98;
extern int spu_madr;
extern int spu_bcr;
void _spu_FsetDelayW(void);
void _spu_FsetDelayR(void);

#define SPU_CTRL_MASK_SPU_ENABLE (1 << 15)
#define DMA_PRIORITY_HIGH 3
#define LEN(x) ((s32)(sizeof(x) / sizeof(*(x))))
#define DMA_DPCR_DMA4_PRIORITY_SHIFT 16
#define DMA_TIMEOUT (0xF00U)
#define NUM_VOICES 24
#define DMA_DPCR_SPU_PRIORITY_HIGH (DMA_PRIORITY_HIGH << DMA_DPCR_DMA4_PRIORITY_SHIFT)
#define DMA_DPCR_MASK_DMA4_ENABLE (1 << 19)
#define SPU_CTRL_MASK_MUTE_SPU (1 << 14)
extern volatile u16 _spu_RQ[10];
void _spu_writeByIO(unsigned char* addr, u_long size);
extern volatile unsigned* dma_dpcr;
extern int _spu_addrMode;
extern int _spu_mem_mode;
extern int _spu_mem_mode_unit;
extern s8 _spu_dummy[16];
#define SPU_CTRL_MASK_SRAM_TRANSFER_MODE ((1 << 4) | (1 << 5))
#define SPUR(field) (_spu_RXX->rxx.field)
#define SPUW(field,val) _spu_RXX->rxx.field = (val)
#define SPU_CTRL_MASK_TRANSFER_MANUAL_WRITE (1 << 4)
void _spu_FwaitFs(void);
extern s32 _spu_mem_mode;
extern s32 _spu_mem_mode_unit;
extern volatile s32* D_80097C5C;

int _spu_init(int bHot);

void _spu_writeByIO(unsigned char* addr, u_long size);

void _spu_FiDMA(void);

void _spu_r_(s32 arg0, u16 arg1, s32 arg2);

int _spu_t(int arg0, ...);

s32 _spu_write(u8* arg0, u32 size);

s32 _spu_read(s32 arg0, u32 size);

void _spu_FsetRXX(s32 arg0, u32 arg1, s32 arg2);

u32 _spu_FsetRXXa(s32 arg0, u32 arg1) {
    u32 temp_a3;
    u32 var_a1;

    var_a1 = arg1;
    if ((_spu_mem_mode != 0) && ((var_a1 % _spu_mem_mode_unit) != 0)) {
        var_a1 += _spu_mem_mode_unit;
        var_a1 &= ~_spu_mem_mode_unitM;
    }
    temp_a3 = var_a1 >> _spu_mem_mode_plus;

    switch (arg0) {
    case -1:
        return temp_a3 & 0xFFFF;
    case -2:
        return var_a1;
    default:
        _spu_RXX->raw[arg0] = temp_a3;
        return var_a1;
    }
}

u32 _spu_FgetRXXa(s32 arg0, s32 arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libspu/spu", _spu_FsetPCR);

void _spu_FsetDelayW(void);

void _spu_FsetDelayR(void);

void _spu_FwaitFs(void);
