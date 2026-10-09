// FUNC 800752ec 656 MAIN0
// MATCHING 800752ec 656
// Portado de psx_tomba (psyq/libspu/spu.c, _spu_t); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/stdarg.h"
#include "libspu_internal.h"

#define SPU_CTRL_MASK_TRANSFER_DMA_READ (3 << 4)
#define SPU_CTRL_MASK_TRANSFER_DMA_WRITE (2 << 4)
extern volatile unsigned* dma_spu_madr;
extern volatile unsigned* dma_spu_bcr;
extern volatile unsigned* dma_spu_chcr;
extern int D_80098934;
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

int _spu_t(int arg0, ...) {
    unsigned channelControl;
    unsigned i;
    unsigned addr;
    va_list args;
    unsigned arg;
    u16 mode;
    u16 cnt;

    va_start(args, arg0);
    switch (arg0) {
    case 2:
        arg = va_arg(args, unsigned);
        _spu_tsa = arg >> _spu_mem_mode_plus;
        _spu_RXX->rxx.trans_addr = _spu_tsa;
        break;
    case 1:
        D_80098934 = 0;
        i = 0;
        while (_spu_RXX->rxx.trans_addr != _spu_tsa) {
            if (++i > 0xF00) {
                return -2;
            }
        }
        cnt = _spu_RXX->rxx.spucnt;
        cnt &= ~SPU_CTRL_MASK_SRAM_TRANSFER_MODE;
        cnt |= SPU_CTRL_MASK_TRANSFER_DMA_WRITE;
        _spu_RXX->rxx.spucnt = cnt;
        break;
    case 0:
        D_80098934 = 1;
        i = 0;
        while (_spu_RXX->rxx.trans_addr != _spu_tsa) {
            if (++i > 0xF00) {
                return -2;
            }
        }
        cnt = _spu_RXX->rxx.spucnt;
        cnt &= ~SPU_CTRL_MASK_SRAM_TRANSFER_MODE;
        cnt |= SPU_CTRL_MASK_TRANSFER_DMA_READ;
        _spu_RXX->rxx.spucnt = cnt;
        break;
    case 3:
        if (D_80098934 == 1) {
            mode = SPU_CTRL_MASK_TRANSFER_DMA_READ;
        } else {
            mode = SPU_CTRL_MASK_TRANSFER_DMA_WRITE;
        }
        i = 0;
        while (
            (_spu_RXX->rxx.spucnt & SPU_CTRL_MASK_SRAM_TRANSFER_MODE) != mode) {
            if (++i > 0xF00) {
                return -2;
            }
        }
        if (D_80098934 == 1) {
            _spu_FsetDelayR();
        } else {
            _spu_FsetDelayW();
        }
        arg = va_arg(args, unsigned);
        spu_madr = arg;
        arg = va_arg(args, unsigned);
        spu_bcr = arg / 0x40;
        spu_bcr += (arg % 0x40) ? 1 : 0;
        *dma_spu_madr = spu_madr;
        *dma_spu_bcr = (spu_bcr << 0x10) | 0x10;
        if (D_80098934 == 1) {
            channelControl = 0x01000200;
        } else {
            channelControl = 0x01000201;
        }
        *dma_spu_chcr = channelControl;
        break;
    }
    return 0;
}

s32 _spu_write(u8* arg0, u32 size);

s32 _spu_read(s32 arg0, u32 size);

void _spu_FsetRXX(s32 arg0, u32 arg1, s32 arg2);

u32 _spu_FsetRXXa(s32 arg0, u32 arg1);

u32 _spu_FgetRXXa(s32 arg0, s32 arg1);

INCLUDE_ASM("asm/scus_942.36/nonmatchings/psyq/libspu/spu", _spu_FsetPCR);

void _spu_FsetDelayW(void);

void _spu_FsetDelayR(void);

void _spu_FwaitFs(void);
