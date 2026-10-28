// FUNC 80074d00 660 MAIN0
// MATCHING 80074d00 660
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libspu_spu_1.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* PSY-Q 4.5 libspu, member spu.o, part 1 of 3: _spu_init, func_8009E0F8.
   The first split had it in psyq_misc_w.c. The tree compiles this run of
   the member's functions with other flags than the runs beside it
   (config/cc_flags.txt), so the member is in several files.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
#include "libspu_internal.h"
typedef union SpuUnion SpuUnion;
typedef void (*SpuTransferCallbackProc)(void);
/* libspu's register block as the library declares it (psyz's libspu_private.h, `union
   SpuUnion`): the registers by name, or as 0x100 16-bit words for the functions that are
   handed a register number. SpuRXX is libspu_spu_1.c's. */
extern SpuUnion *D_800988E4;        /* _spu_RXX -> 0x1F801C00 */
extern volatile s32 *D_800988F4;            /* SPU.OBJ .data+0x10, static: -> 0x1F8010F0, DPCR */
extern u16 D_800988FC;                      /* _spu_tsa */
extern s32 D_80098900;                      /* _spu_transMode */
extern s32 D_80098904;                      /* _spu_addrMode */
extern s32 D_80098908;                      /* _spu_mem_mode */
extern s32 D_8009890C;                      /* _spu_mem_mode_plus */
extern s32 D_80098910;                      /* _spu_mem_mode_unit */
extern s32 D_80098914;                      /* _spu_mem_mode_unitM */
extern s32 D_80098918;                      /* _spu_inTransfer */
extern volatile SpuTransferCallbackProc D_8009891C;    /* _spu_transferCallback */
extern volatile SpuIRQCallbackProc D_80098920;         /* _spu_IRQCallback */
extern u8 D_80098924[];                     /* SPU.OBJ .data+0x40, static: sixteen bytes of 7 */
extern volatile u16 D_8009D5E0[];           /* _spu_RQ[10] */
extern char D_8001636C[];                   /* "SPU:T/O [%s]\n" */
extern char D_8001637C[];                   /* "wait (reset)" */
void _spu_Fw1ts(void);
void func_8009E0F8(u8 *, u32);              /* _spu_FwriteByIO */
s32 printf(const char *, ...);

/* libspu _spu_init (SPU.OBJ): reset the SPU; bHot != 0 keeps the voices.
   Enables DMA channel 4 at priority 3 in DPCR (|= 0xB0000), zeroes the main
   volume and SPUCNT, clears _spu_transMode, _spu_addrMode and _spu_tsa, waits
   one _spu_Fw1ts, zeroes the main volume again and polls SPUSTAT's low eleven
   bits until they read 0 (0xF01 polls, then the "wait (reset)" time-out
   print).  Then the memory unit (_spu_mem_mode 2, _plus 3, _unit 8, _unitM
   7), the transfer control word 4, reverb volume 0, key off for all 24
   voices, reverb-on mask 0 and the ten words of the register queue _spu_RQ.
   Cold only: FM and noise masks and CD / external volume 0, the sixteen
   dummy bytes D_80098924 written to SPU RAM 0x1000 by I/O (_spu_tsa = 0x200,
   _spu_FwriteByIO), every voice given volume 0, pitch 0x3FFF, start address
   0x200 and ADSR 0, then keyed on and, four _spu_Fw1ts later, off, with four
   more to finish.  Last _spu_inTransfer = 1 (idle), SPUCNT = 0xC000 (enable,
   unmute) and both callbacks cleared.  Returns 0.  Caller: _SpuInit
   . */
/* func_8009DE78 */
s32 _spu_init(s32 bHot)
{
    s32 i;
    u32 n;

    *D_800988F4 |= 0xB0000;
    D_800988E4->rxx.main_vol.left = 0;
    D_800988E4->rxx.main_vol.right = 0;
    D_800988E4->rxx.spucnt = 0;
    D_80098900 = 0;
    D_80098904 = 0;
    D_800988FC = 0;
    _spu_Fw1ts();
    D_800988E4->rxx.main_vol.left = 0;
    D_800988E4->rxx.main_vol.right = 0;
    n = 0;
    while (D_800988E4->rxx.spustat & 0x7FF) {
        if (++n > 0xF00) {
            printf(D_8001636C, D_8001637C);
            break;
        }
    }
    D_80098908 = 2;
    D_8009890C = 3;
    D_80098910 = 8;
    D_80098914 = 7;
    D_800988E4->rxx.data_trans = 4;
    D_800988E4->rxx.rev_vol.left = 0;
    D_800988E4->rxx.rev_vol.right = 0;
    D_800988E4->rxx.key_off[0] = 0xFFFF;
    D_800988E4->rxx.key_off[1] = 0xFFFF;
    D_800988E4->rxx.rev_mode[0] = 0;
    D_800988E4->rxx.rev_mode[1] = 0;
    for (i = 0; i < 10; i++) {
        D_8009D5E0[i] = 0;
    }
    if (bHot == 0) {
        D_800988E4->rxx.chan_fm[0] = 0;
        D_800988E4->rxx.chan_fm[1] = 0;
        D_800988E4->rxx.noise_mode[0] = 0;
        D_800988E4->rxx.noise_mode[1] = 0;
        D_800988E4->rxx.cd_vol.left = 0;
        D_800988E4->rxx.cd_vol.right = 0;
        D_800988E4->rxx.ex_vol.left = 0;
        D_800988E4->rxx.ex_vol.right = 0;
        D_800988FC = 0x200;
        func_8009E0F8(D_80098924, 0x10);
        for (i = 0; i < 24; i++) {
            D_800988E4->rxx.voice[i].volume.left = 0;
            D_800988E4->rxx.voice[i].volume.right = 0;
            D_800988E4->rxx.voice[i].pitch = 0x3FFF;
            D_800988E4->rxx.voice[i].addr = 0x200;
            D_800988E4->rxx.voice[i].adsr[0] = 0;
            D_800988E4->rxx.voice[i].adsr[1] = 0;
        }
        D_800988E4->rxx.key_on[0] = 0xFFFF;
        D_800988E4->rxx.key_on[1] = 0xFF;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        D_800988E4->rxx.key_off[0] = 0xFFFF;
        D_800988E4->rxx.key_off[1] = 0xFF;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
    }
    D_80098918 = 1;
    D_800988E4->rxx.spucnt = 0xC000;
    D_8009891C = 0;
    D_80098920 = 0;
    return 0;
}
extern char D_800138F4[];                   /* "wait (wrdy H -> L)" */
extern char D_80013908[];                   /* "wait (dmaf clear/W)" */

/* libspu _spu_FwriteByIO (SPU.OBJ, static): write size bytes at addr to SPU
   RAM through the data port, without DMA.  Keeps SPUSTAT's low eleven bits,
   sets the transfer address register to _spu_tsa (D_800988FC) and waits one
   _spu_Fw1ts.  Then per block of at most 0x40 bytes: the words go into the
   transfer FIFO, SPUCNT's transfer mode (bits 4-5) is set to 1, manual
   write, and after one _spu_Fw1ts SPUSTAT bit 10 (busy) is polled until it
   clears (0xF01 polls, then the "wait (wrdy H -> L)" time-out print); two
   more _spu_Fw1ts end the block.  Afterwards the transfer mode goes back to
   0 and SPUSTAT is polled until its low eleven bits equal the value kept at
   entry ("wait (dmaf clear/W)" on time-out).  Callers: _spu_init (the dummy
   block) and _spu_Fw when _spu_transMode is not 0. */
void func_8009E0F8(u8 *addr, u32 size);
