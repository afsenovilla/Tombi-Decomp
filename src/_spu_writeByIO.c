// FUNC 80074f94 460 MAIN0
// MATCHING 80074f94 460
// Portado de psx_tomba (psyq/libspu/spu.c, _spu_writeByIO); licencia MIT del proyecto original.
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

void _spu_writeByIO(unsigned char* addr, u_long size) {
    unsigned short spustat;
    int num_to_trans;
    unsigned short* cur_pos;
    int spustat_cur;
    int i;
    unsigned short cnt;
    unsigned timeout;

    cur_pos = (unsigned short*)addr;
    spustat = SPUR(spustat) & 0x7FF;
    SPUW(trans_addr, _spu_tsa);
    _spu_FwaitFs();
    while (size > 0) {
        num_to_trans = (size > 0x40) ? 0x40 : size;
        for (i = 0; i < num_to_trans; i += 2) {
            SPUW(trans_fifo, *cur_pos++);
        }
        cnt = SPUR(spucnt);
        cnt &= ~SPU_CTRL_MASK_SRAM_TRANSFER_MODE;
        cnt |= SPU_CTRL_MASK_TRANSFER_MANUAL_WRITE;
        SPUW(spucnt, cnt);
        _spu_FwaitFs();
        timeout = 0;
        while (SPUR(spustat) & 0x400) {
            timeout++;
            if (timeout > 0xF00) {
                printf("SPU:T/O [%s]\n", "wait (wrdy H -> L)");
                break;
            }
        }
        _spu_FwaitFs();
        _spu_FwaitFs();
        size -= num_to_trans;
    }
    cnt = SPUR(spucnt);
    cnt &= ~SPU_CTRL_MASK_SRAM_TRANSFER_MODE;
    SPUW(spucnt, cnt);
    timeout = 0;
    spustat_cur = SPUR(spustat) & 0x7FF;
    while (spustat_cur != spustat) {
        timeout++;
        if (timeout > 0xF00) {
            printf("SPU:T/O [%s]\n", "wait (dmaf clear/W)");
            return;
        }
        spustat_cur = SPUR(spustat) & 0x7FF;
    }
}

void _spu_FiDMA(void);

void _spu_r_(s32 arg0, u16 arg1, s32 arg2);

int _spu_t(int arg0, ...);

s32 _spu_write(u8* arg0, u32 size);

s32 _spu_read(s32 arg0, u32 size);

void _spu_FsetRXX(s32 arg0, u32 arg1, s32 arg2);

u32 _spu_FsetRXXa(s32 arg0, u32 arg1);

u32 _spu_FgetRXXa(s32 arg0, s32 arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libspu/spu", _spu_FsetPCR);

void _spu_FsetDelayW(void);

void _spu_FsetDelayR(void);

void _spu_FwaitFs(void);
