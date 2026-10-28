// FUNC 80068828 340 MAIN0
// MATCHING 80068828 340
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libetc_intr_2.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
#define SKIP_ASM
/* SPDX-License-Identifier: MIT
   Parts of this file match code of sotn-decomp
   (src/main/psxsdk/libetc/intr.c), psyz (decomp/src/libetc/intr.c) and
   psy-q-decomp (src/etc/intr.c), all MIT-licensed: the notices are in
   LICENSES/, and NOTICE.md says more. */
/* PSY-Q 4.5 libetc, member intr.o, part 2 of 2: SetIntrMask, func_80094D6C,
   func_80094E44, func_80095014, func_8009515C, func_800951FC,
   func_80095274. The first split had it in psyq_spu_g.c. The tree compiles
   this run of the member's functions with other flags than the runs beside
   it (config/cc_flags.txt), so the member is in several files.
   The build must stay byte-matching. */

#include "common.h"
#include "psyq/libsnd.h"
#include "psyq/libspu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/kernel.h"
/* libetc INTR.OBJ's state intrEnv (0x1068 bytes) and its callback table, as
   eternal-silver-sssc-decomp names the fields. */
typedef void (*IntrHandler)(void);
typedef struct {
    u16 interruptsInitialized;   /* +0x00 */
    u16 inInterrupt;             /* +0x02 */
    IntrHandler handlers[11];    /* +0x04 one per I_STAT bit */
    u16 enabledInterruptsMask;   /* +0x30 */
    u16 savedMask;               /* +0x32 I_MASK saved by stopIntr */
    s32 savedPcr;                /* +0x34 DPCR saved by stopIntr */
    s32 buf[12];                 /* +0x38 the jmp_buf given to HookEntryInt */
    s32 stack[0x400];            /* +0x68 the interrupt stack */
} IntrEnv;
typedef struct {
    char *rcsid;
    void *(*DMACallback)();
    void *(*InterruptCallback)();
    void *(*ResetCallback)();
    void *(*StopCallback)();
    void *(*VSyncCallbacks)();
    void *(*RestartCallback)();
    IntrEnv *intrEnv;
} IntrCallbacks;

/* libetc INTR.OBJ (intr.c 1.75 of 1997/02/07; 0x80094BD8 to 0x80095297, 432 of 432 words
   equal the SDK 4.4 / 4.6 object modulo relocations): the object's state. The API half
   (ResetCallback ... GetIntrMask) is in libetc_intr_1.c; SetIntrMask and the statics
   startIntr, trapIntr, setIntr, stopIntr, restartIntr, memclr are here. */

/* intrEnv (0x1068 bytes at D_800970B4) */

/* the callback table (0x20 bytes at 0x800B7910) that D_800B7930 (pCallbacks) points to;
   the API functions call through it */

extern IntrEnv D_800970B4;          /* intrEnv */
extern volatile u16 *D_800B7934;    /* i_stat: 0x1F801070, I_STAT */
extern volatile u16 *D_80098144;    /* i_mask: 0x1F801074, I_MASK */
extern volatile s32 *D_800B793C;    
/* libetc SetIntrMask (INTR.OBJ): store a new I_MASK and return the old one. Built by
   the object's own recipe since i_mask is declared as the SDK has it (volatile); the
   text of the function is unchanged. */
/* func_80094D54 */
u16 SetIntrMask(u16 arg0);
extern IntrCallbacks *D_800B7930;           /* pCallbacks -> the callback table at 0x800B7910 */
extern int setjmp(int *);                   /* libc2 setjmp (SETJMP.OBJ); setjmp.h: `extern int setjmp(jmp_buf);`, jmp_buf = int[12] */
void func_80095274(void *ptr, u32 size);  /* TU defines this below */
void func_80094E44(void);  /* TU defines this below */
s32 HookEntryInt();
void *startIntrVSync();
long long startIntrDMA();            /* startIntrDMA: a MARKED SPELLING, see func_80094D6C's comment */

/* libetc startIntr (INTR.OBJ, static; ResetCallback's worker, entry +0xC of the callback
   table): hook the interrupt entry. Returns 0 when it is already hooked. Otherwise it
   zeroes I_MASK and I_STAT, sets DPCR to 0x33333333 (every DMA channel priority 3, off),
   clears intrEnv (0x41A words), and saves the context with setjmp: every later
   interrupt returns from that setjmp with a non-zero value and runs trapIntr. The saved
   $sp is replaced by &intrEnv.stack[1004], the buffer is given to HookEntryInt,
   interruptsInitialized is set, startIntrVSync's and startIntrDMA's results go into the
   table's VSyncCallbacks and DMACallback entries, then _96_remove,
   ExitCriticalSection, and &intrEnv is returned.
   MARKED SPELLING: startIntrDMA is declared `long long` in this TU
   although it returns a pointer: libetc_intr_dma_1.c defines it `void *` and INTR_DMA.OBJ's code
   sets only $v0, so this declaration contradicts the definition. Both sotn-decomp
   (libapi_internal.h) and psyz (libetc/intr.c, "TODO fix return type") write it so.
   Without it the function is 51/54: the pointer for the DMACallback store goes to $v1
   where the target has $a0, and a two-register return value is what keeps $v1 busy
   across that store. No true declaration of the function or of the member does it
   (pointer, function pointer, int, long, a prototype, a local temporary: all 51/54).
   Sony's intr.c is not available, so the construct that kept $v1 busy there is not
   known. The cast at the store only makes the integer-to-pointer conversion explicit
   (same bytes without it). */
IntrEnv *func_80094D6C(void);

/* lib_g0: func_80094E44 */
extern s32 D_800B7940;                      /* trapIntr's own count (INTR.OBJ .data+0x10F0): passes in a row that ended with an interrupt still pending */
extern char D_800137D8[];                   /* "unexpected interrupt(%04x)\n" */
extern char D_800137F4[];                   /* "intr timeout(%04x:%04x)\n" */
s32 printf();

/* libetc trapIntr (INTR.OBJ, static): the interrupt dispatcher. startIntr's setjmp
   returns non-zero into it on every interrupt (HookEntryInt(intrEnv.buf)), on the
   stack in intrEnv. With the entry not hooked it prints "unexpected interrupt" and
   returns from the exception. Otherwise it sets inInterrupt and, while
   enabledInterruptsMask & I_STAT & I_MASK is non-zero, walks the bits from 0 up (at most
   11): acknowledges each set bit (I_STAT = ~(1 << i)) and calls its handler if there is
   one. Then, if I_STAT & I_MASK still has a bit (an interrupt nobody handles), it counts
   the pass in D_800B7940; past 0x800 such passes in a row it prints "intr timeout",
   clears the count and zeroes I_STAT. A clean pass clears the count. Last it clears
   inInterrupt and returns from the exception.
   It carries lib_g0 because the object was built -G0: `D_800B7940++` keeps the scalar's
   address in a register (lui / addiu, lw 0(r), sw 0(r)) beside direct stores of the same
   symbol, which cse does only when the symbol is not small data (at -G8 109/116 with
   the sized scalar and 104 with an unsized array; 116/116 at -G0). */
void func_80094E44(void);

s32 ChangeClearRCnt(s32, s32);

/* libetc setIntr (INTR.OBJ, static; InterruptCallback's worker, entry +8 of the callback
   table): install (handler != 0) or remove the handler of IRQ bit irq and return the
   previous one. Nothing is done when the handler is unchanged or the interrupt entry is
   not hooked. I_MASK is zeroed while the table and enabledInterruptsMask change, then
   written back with the bit set or cleared. IRQ 0 (VBlank) also calls
   ChangeClearPAD(handler == 0) and ChangeClearRCnt(3, handler == 0): with a handler
   installed the kernel's pad driver passes the VBlank interrupt on without completing
   it. IRQ 4 / 5 / 6 do the same for root counters 0 / 1 / 2. */
IntrHandler func_80095014(s32 irq, IntrHandler handler)
{
    IntrHandler prev;
    s32 mask;

    prev = D_800970B4.handlers[irq];
    if (handler != prev && D_800970B4.interruptsInitialized) {
        mask = *D_80098144;
        *D_80098144 = 0;
        if (handler != 0) {
            D_800970B4.handlers[irq] = handler;
            mask |= 1 << irq;
            D_800970B4.enabledInterruptsMask |= 1 << irq;
        } else {
            D_800970B4.handlers[irq] = 0;
            mask &= ~(1 << irq);
            D_800970B4.enabledInterruptsMask &= ~(1 << irq);
        }
        if (irq == 0) {
            ChangeClearPAD(handler == 0);
            ChangeClearRCnt(3, handler == 0);
        }
        if (irq == 4) {
            ChangeClearRCnt(0, handler == 0);
        }
        if (irq == 5) {
            ChangeClearRCnt(1, handler == 0);
        }
        if (irq == 6) {
            ChangeClearRCnt(2, handler == 0);
        }
        *D_80098144 = mask;
    }
    return prev;
}
s32 EnterCriticalSection();
s32 ResetEntryInt();

/* libetc stopIntr (INTR.OBJ, static; StopCallback's worker, entry +0x10 of the callback
   table): unhook the interrupt entry. Returns 0 when it is not hooked; otherwise enters
   a critical section, saves I_MASK and DPCR in intrEnv, zeroes I_MASK and I_STAT, clears
   the DMA channels' enable bits (DPCR &= 0x77777777), restores the kernel's own entry
   (ResetEntryInt), clears interruptsInitialized and returns &intrEnv. The critical
   section is left open: restartIntr closes it. */
IntrEnv *func_8009515C(void);
s32 HookEntryInt();

/* libetc restartIntr (INTR.OBJ, static; RestartCallback's worker, entry +0x18 of the
   callback table): hook the interrupt entry again after stopIntr. Returns 0 when it is
   already hooked; otherwise HookEntryInt(intrEnv.buf), sets interruptsInitialized, writes
   the saved I_MASK and DPCR back, leaves the critical section and returns &intrEnv. */
IntrEnv *func_800951FC(void);

/* libetc memclr(ptr, size) (INTR.OBJ, static): zero `size` words at ptr; startIntr clears
   intrEnv with it. Nothing is done for a size of 0; otherwise a counter starts at size - 1 and one word is cleared per
   step until the counter reaches -1. The counter matches signed or unsigned. */
void func_80095274(void *ptr, u32 size);
