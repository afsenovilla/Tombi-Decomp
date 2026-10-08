// FUNC 80028754 368 MAIN0
/* w5: score 126 (was 138). Left: game has move t0,a0 at entry (o copy), e6 address in a2/a0 regs, and the two
   `e6 = 0x3a - d` stores are not cross-jumped; ours is 16 B shorter. */
typedef struct O { char p[0x32]; short y32; } O;
extern unsigned char DAT_800a60d6;
extern int DAT_800a606c;
extern short DAT_1f8000f2;
extern short DAT_1f80016e;
extern short DAT_1f8000e6[];
extern short E6;

static __inline__ void body(O *o)
{
    short d;
    short t;
    short *e;
    if (DAT_800a60d6 == 3)
        { unsigned short b = DAT_1f8000f2 - 0x14; d = b - DAT_800a606c; }
    else
        d = DAT_1f8000f2 - DAT_1f80016e;
    t = d + *e;
    if (t != 0x3a) {
        if (t < 0x3a) {
            if (t < -6) {
                DAT_1f8000f2 += 2;
                if (o->y32 < DAT_1f8000f2) DAT_1f8000f2 = o->y32;
                else d += 2;
            } else {
                *e += 2;
            }
            if (d + *e > 0x3a) *e = 0x3a - d;
        } else {
            *e -= 2;
            if (d + *e < 0x3a) *e = 0x3a - d;
        }
    }
    if (o->y32 < DAT_1f8000f2 + E6) E6 = o->y32 - DAT_1f8000f2;
}

void FUN_80028754(O *o)
{
    body(o);
}
