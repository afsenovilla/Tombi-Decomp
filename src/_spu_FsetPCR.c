// FUNC 80075794 96 MAIN0
// MATCHING 80075794 96
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libspu_spu_3.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* SPDX-License-Identifier: MIT
   Parts of this file match code of psyz (decomp/src/libspu/spu.c),
   MIT-licensed: the notices are in LICENSES/, and NOTICE.md says more. */
/* PSY-Q 4.5 libspu, member spu.o, part 3 of 3: _spu_Fr_, _spu_t, _spu_Fw,
   _spu_Fr, _spu_FsetRXX, _spu_FsetRXXa, _spu_FgetRXXa, _spu_FsetPCR,
   func_8009E900, func_8009E928, _spu_Fw1ts. The first split had it in
   psyq_misc_y.c. The tree compiles this run of the member's functions with
   other flags than the runs beside it (config/cc_flags.txt), so the member
   is in several files.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
#include "libspu_internal.h"
typedef union SpuUnion SpuUnion;
typedef void (*SpuTransferCallbackProc)(void);
extern u16 D_800B7BC8;              /* _spu_tsa */
extern s32 D_800B7BD8;              /* _spu_mem_mode_plus */
/* libspu's register block as the library declares it (psyz's libspu_private.h, `union
   SpuUnion`): the registers by name, or as 0x100 16-bit words for the functions that are
   handed a register number. SpuRXX is libspu_spu_1.c's. */
extern SpuUnion *D_800B7BB0;        /* _spu_RXX -> 0x1F801C00 */
extern volatile s32 *D_800B7BB4;    /* SPU.OBJ .data+4, static: -> 0x1F8010C0, DMA channel 4 MADR */
extern volatile s32 *D_800B7BB8;    /* SPU.OBJ .data+8, static: -> 0x1F8010C4, DMA channel 4 BCR */
extern volatile s32 *D_800B7BBC;    /* SPU.OBJ .data+0xC, static: -> 0x1F8010C8, DMA channel 4 CHCR */
extern s32 D_800B7C00;              /* SPU.OBJ .data+0x50, static: 1 = the DMA transfer reads */
void _spu_Fw1ts(void);  /* TU defines this below */
void func_8009E928(void);  /* TU defines this below */

/* libspu _spu_Fr_ (SPU.OBJ): start a DMA read of SPU RAM into addr.  tsa is
   the transfer start address as the SPU register takes it, blocks the number
   of 64-byte blocks.  Writes the address register, waits one _spu_Fw1ts,
   sets SPUCNT's transfer mode (bits 4-5) to 3, DMA read, waits again, sets
   the bus delay for reading (func_8009E928) and programs DMA channel 4: MADR
   = addr, BCR = blocks << 16 | 0x10 (16 words a block), the direction flag
   D_800B7C00 = 1, CHCR = 0x01000200 (start, sync by request, to memory).
   No caller in this program. */
/* func_8009E374 */
void _spu_Fr_(s32 addr, u16 tsa, s32 blocks);
extern s32 D_800B7C04;          /* SPU.OBJ .data+0x54 (static): the DMA's memory address */
extern s32 D_800B7C08;          /* SPU.OBJ .data+0x58 (static): the DMA's size in 64-byte blocks */
void func_8009E900(void);  /* TU defines this below */

/* libspu _spu_t(mode, ...) (SPU.OBJ): the steps of an SPU-RAM DMA transfer, one per call.
   mode 2 (address): _spu_tsa = address >> _spu_mem_mode_plus, also written to the SPU's
   transfer-address register (+0x1A6).
   mode 1 (write) / mode 0 (read): record the direction in D_800B7C00 (0 / 1), wait until
   the register reads back _spu_tsa, then set SPUCNT (+0x1AA) bits 4-5 to 2 (DMA write) /
   3 (DMA read).
   mode 3 (buffer, size): wait until SPUCNT's bits 4-5 show that mode, set the SPU bus
   delay for the direction (func_8009E928 reading, func_8009E900 writing), then start DMA
   channel 4: MADR = buffer, BCR = (size rounded up to 64-byte blocks) << 16 | 0x10, CHCR =
   0x01000200 (read) or 0x01000201 (write).
   Each wait gives up after 0xF01 polls and returns -2; everything else returns 0, an
   unknown mode included. Callers: _spu_Fw, _spu_Fr and
   SpuClearReverbWorkArea. */
/* func_8009E41C */
int _spu_t(int mode, ...);

extern s32 D_800B7BCC;              /* _spu_transMode: 0 = transfer by DMA */
void func_8009E0F8(u8 *, u32);      /* _spu_FwriteByIO (libspu_spu_1.c) */

/* libspu _spu_Fw(addr, size) (SPU.OBJ): write size bytes at addr to SPU RAM at the
   transfer address _spu_tsa. With _spu_transMode 0 the transfer is a DMA, started by
   three calls of _spu_t: the address (2, _spu_tsa << _spu_mem_mode_plus, a byte address
   again), the direction (1, write), the buffer and its size (3). Otherwise the bytes go
   through the data port (_spu_FwriteByIO). Returns size. Caller: SpuWrite
   . */
/* func_8009E69C */
u32 _spu_Fw(u8 *addr, u32 size);

/* libspu _spu_Fr(addr, size) (SPU.OBJ): read size bytes of SPU RAM at the transfer
   address _spu_tsa into addr, always by DMA: _spu_t with the address (2), the direction
   (0, read), the buffer and its size (3). Returns size. No caller in this program. */
/* func_8009E720 */
u32 _spu_Fr(u8 *addr, u32 size);

/* libspu _spu_FsetRXX(offset, value, mode) (SPU.OBJ): write one SPU register. offset is
   the register's word index in the block (0x1F801C00 + offset * 2). mode 0 stores value
   as it is; any other mode stores value >> _spu_mem_mode_plus, a byte address turned
   into the register's address unit. Callers: _SpuInit, SpuSetReverbModeParam and
   SpuSetReverbModeType, all with word 0xD1 (the reverb work area's start) and mode 0. */
/* func_8009E784 */
void _spu_FsetRXX(s32 offset, u32 value, s32 mode);
extern s32 D_800B7BD4;          /* _spu_mem_mode: non-zero = addresses are rounded to the unit */
extern s32 D_800B7BDC;          /* _spu_mem_mode_unit: the allocation unit in bytes (8) */
extern s32 D_800B7BE0;          /* _spu_mem_mode_unitM: the unit's mask (7) */

/* libspu _spu_FsetRXXa(offset, unit) (SPU.OBJ): write a byte address to an SPU address
   register. With _spu_mem_mode set, an address that is not a multiple of _spu_mem_mode_unit
   is first rounded up to the next one. The register value is the address shifted right by
   _spu_mem_mode_plus, as a u16. offset is the register's word index in the SPU block
   (0x1F801C00 + offset * 2); two offsets write nothing: -1 returns the register value, -2
   the rounded address. Every other offset stores the value and returns the rounded
   address. */
/* func_8009E7C8 */

/* libspu _spu_FgetRXXa(offset, mode) (SPU.OBJ): read one SPU address register. offset is
   the register's word index in the block. mode -1 returns the register word as read;
   any other mode returns it << _spu_mem_mode_plus, the byte address. No caller in this
   program. */
/* func_8009E86C */
s32 _spu_FgetRXXa(s32 offset, s32 mode);

extern volatile s32 *D_800988F4;    /* SPU.OBJ .data+0x10, static: -> 0x1F8010F0, DPCR */

/* libspu _spu_FsetPCR(flag) (SPU.OBJ): set DMA channel 4's priority in DPCR (bits
   16-18): the three bits are cleared, then 3 is written when flag is set, 5 when it is
   not. The channel's enable bit (19) is left alone. No caller in this program. */
/* func_8009E8A8 */
void _spu_FsetPCR(s32 flag)
{
    *D_800988F4 &= 0xFFF8FFFF;
    if (flag != 0) {
        *D_800988F4 |= 0x30000;
    } else {
        *D_800988F4 |= 0x50000;
    }
}

extern s32 *D_800B7BC4;

void func_8009E900(void);
void func_8009E928(void);

/* libspu _spu_Fw1ts() (SPU.OBJ): a short busy wait, used between SPU register writes and
   around transfers. Sixty turns of a loop whose counter and work value are both volatile
   stack variables (the value is 13 multiplied by 13 each turn and never used). */
/* func_8009E950 */
void _spu_Fw1ts(void);
