// FUNC 8006a59c 212 MAIN0
// CC gcc-2.8.1
// FLAGS -O2 -G8 -fno-expensive-optimizations
/* score 25 (ncheck, old-gcc 2.8.1 = CC1PSX 4.4 here): library code from the newer compiler (jr ra; addiu sp epilogue
   with s-regs) -> `// CC gcc-2.8.1`. Control flow matches with the early-return form. Left: (1) the game keeps
   `li v1,1; sb` and `addiu v1,v1,1; sra v1,v1,1` while ours gets `move v1,v0` / `addu v1,v1,v0; srav` because
   sched1 hoists the return value `li v0,1` to the top of the body and 2.8's reload_cse_regs then replaces every
   constant 1 by v0 (dumps: .greg still has the constants, .sched2 has the copies; CC1PSX 4.4 does the same).
   The game must have had `v0 = 1` after the body at reload time (it is stolen into the beqz delay slot by reorg).
   Tried: nested ifs, if/else returns, r variable, goto ok, do{}while(0) and asm barrier before return 1, inline
   body, `return n != 0`, gcc-2.8.0, -O1/-O3/-fno-schedule-insns(2): all worse. (2) `lbu v1,0xe3` hoisted above
   `sw a0,0x18` and (n+3)>>2 computed early into a1 in the game; v/v2 temp splits do not change it.
   -fno-expensive-optimizations (25 vs 42) only moves things around and is probably not the real flag. */
typedef struct S {
    int a0;
    int a4;
    int a8;
    char pad[0x14 - 0xc];
    int a14;
    int a18;
    char pad1[0x46 - 0x1c];
    char b46, b47;
    char pad2;
    char b49;
    char pad3[0xe3 - 0x4a];
    unsigned char be3;
    char pad4[5];
    unsigned char be9;
} S;
extern int (*DAT_800981c8)(void);
extern void LAB_8006a670(void);
extern void LAB_8006a718(void);
int FUN_8006a59c(S *s, int n)
{
    int v;
    if (n == 0 || s->a4 != 0 || DAT_800981c8() != 0) return 0;
    s->b49 = 4;
    s->b46 = 1;
    s->a14 = (int)LAB_8006a670;
    s->a18 = (int)LAB_8006a718;
    v = (n + 3 >> 2) * 4;
    s->a0 = v;
    s->b47 = 0;
    v = v + ((s->be3 + 1) >> 1) * 4;
    s->a4 = v;
    s->a8 = v + (s->be9 * 5 + 3 & 0xffc);
    return 1;
}
