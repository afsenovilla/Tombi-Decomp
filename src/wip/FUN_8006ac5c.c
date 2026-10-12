// FUNC 8006ac5c 152 MAIN0
// CC gcc-2.8.1
/* score 14 (gcc-2.8.1, was 32 with 2.7.2): regs, prologue and epilogue match. Left: constant 1 is CSEd (li v0,1 reused for sb b46 via move and as sltu operand) where the game has li v0,1 / li v1,1 / sltiu ..,1, and lbu be4 is scheduled first in the game. Tried: all 720 orders of the 6 stores x t type (char/uchar/int) x compare operand order (14 best), return via r/early return/one var, return types, ! / ?: forms of the compare, raw char* offsets (24), flags -G8/-O1/-O3/-fno-expensive-optimizations/-fno-cse-follow-jumps/-fno-schedule-insns.
   n66: the `move v1,v0` / `sltu ..,v0` come from gcc 2.8's post-reload reload_cse_regs (it replaces constants with a
   hard reg already holding them): the game's `li v0,1` must sit AFTER the sb/sltiu in the pre-sched2 order (sched1
   hoists ours to the block top). Ghidra order (be4 load first, b46 first) + -fno-schedule-insns keeps the constants
   apart but swaps s2/s3 and loses the early lbu (26-30). */
typedef struct { char p0[0x14]; int w14, w18; char p1[0x46 - 0x1c]; char b46; char p2[0x51 - 0x47]; char b51, b52, b53; char p3[0xe4 - 0x54]; unsigned char be4; } TO;
extern int (*FP)();
extern char A14[], A18[];
int FUN_8006ac5c(TO *o, int a, int b)
{
    char t = a;
    if (FP(o, a, b) == 0) {
        o->w14 = (int)A14;
        o->b51 = a;
        o->b52 = b;
        o->w18 = (int)A18;
        o->b53 = (o->be4 == t);
        o->b46 = 1;
        return 1;
    }
    return 0;
}
