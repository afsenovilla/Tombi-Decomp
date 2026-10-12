// FUNC 800697b4 204 MAIN0
// CC gcc-2.8.1
// FLAGS -O2 -G8
/* score 37 (ncheck = CC1PSX 4.4 here): newer-compiler library code -> `// CC gcc-2.8.1` + `-G8`; DAT_8009bf88/8c as one
   array (base in v0, stores at +4 then +0). Left: (1) the game has `lui at; jal EnterCriticalSection; sw zero,DAT(at)`:
   the `sw $0,sym` macro sits in the jal delay slot, which gcc's reorg refuses here (the call's (mem (symbol)) marks
   memory as needed; the store is not eligible from before or after the call). Tried: store after the call, volatile,
   array, char/short types, block temp, -fno-schedule-insns, -O1, -fno-expensive-optimizations; -fno-delayed-branch
   (asm-filled slots) is much worse under the real ASPSX. (2) the p[0]=-2 / p[1]|=1 block: game loads p into v1,
   stores -2 first, then lw 4(v1) / move a1,zero / ori / sw in the jal slot; ours loads p into a2 with lw 4 first.
   (3) final block: game `sw zero,4(v0); sw zero,(v0); li v0,1; sw` then the epilogue loads; ours interleaves. */
extern int DAT_800981e4;
extern int DAT_800981e0;
extern int DAT_8009bf78[];
extern int *DAT_8009820c;
extern void (*DAT_800981b0)(int);
extern int DAT_8009bf88[];
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void SysDeqIntRP(int, int *);
extern void SysEnqIntRP(int, int *);
extern void ChangeClearRCnt(int, int);

void FUN_800697b4(void)
{
    int *p;
    DAT_800981e4 = 0;
    EnterCriticalSection();
    SysDeqIntRP(2, DAT_8009bf78);
    SysEnqIntRP(2, DAT_8009bf78);
    p = DAT_8009820c;
    p[0] = -2;
    p[1] = p[1] | 1;
    ChangeClearRCnt(3, 0);
    ExitCriticalSection();
    DAT_800981b0(DAT_800981e0);
    DAT_800981b0(DAT_800981e0 + 0xf0);
    DAT_8009bf88[1] = 0;
    DAT_8009bf88[0] = 0;
    DAT_800981e4 = 1;
}
