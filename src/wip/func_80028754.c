// FUNC 80028754 368 MAIN0
/* diff: game has a 0x18 frame with no saves (sp first), o moved to t0, d copied a1; scratchpad accesses via symbols */
extern unsigned char D_800A60D6;
extern int D_800A606C;
extern short D_1F8000E6;
extern short D_1F8000F2;
extern short D_1F80016E;
#define F2 D_1F8000F2
#define E6 D_1F8000E6
void func_80028754(unsigned char *o)
{
    short d;
    short e;
    short s;
    short *p;
    int t;
    if (D_800A60D6 == 3) { t = (unsigned short)F2 - 0x14; t = t - D_800A606C; }
    else t = F2 - D_1F80016E;
    d = t;
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
