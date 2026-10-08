// FUNC 80028754 368 MAIN0
/* w1: score 97 (was 114) with the body as an inline taking (o, short d), which creates the 0x18 frame.
   left: subu sp lands in the bne delay slot (game: sp first, move t0,a0 in the slot) and o is not copied to t0.
   old diff: game has a 0x18 frame with no saves (sp first), o moved to t0, d copied a1; scratchpad accesses via symbols */
extern unsigned char D_800A60D6;
extern int D_800A606C;
extern short D_1F8000E6;
extern short D_1F8000F2;
extern short D_1F80016E;
#define F2 D_1F8000F2
#define E6 D_1F8000E6
static __inline__ void inl(unsigned char *o, short d)
{
    short e;
    short s;
    short *p;
    p = &D_1F8000E6;
    e = *p;
    s = d + e;
    if (s != 0x3a) {
        if (s < 0x3a) {
            if (s < -6) {
                F2 += 2;
                if (*(short *)(o + 0x32) < F2) F2 = *(short *)(o + 0x32);
                else d += 2;
            } else {
                *p = e + 2;
            }
            p = &D_1F8000E6;
            if (d + *p > 0x3a) *p = 0x3a - d;
        } else {
            e -= 2;
            *p = e;
            if (d + e < 0x3a) *p = 0x3a - d;
        }
    }
    if (*(short *)(o + 0x32) < F2 + E6) E6 = *(short *)(o + 0x32) - F2;
}
void func_80028754(unsigned char *o)
{
    int t;
    if (D_800A60D6 == 3) { t = (unsigned short)F2 - 0x14; t = t - D_800A606C; }
    else t = (unsigned short)F2 - (unsigned short)D_1F80016E;
    inl(o, t);
}
