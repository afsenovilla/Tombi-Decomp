// FUNC 80028754 368 MAIN0
typedef struct O { char p[0x32]; short y32; } O;
extern unsigned char DAT_800a60d6;
extern int DAT_800a606c;
extern short DAT_1f8000f2;
extern short DAT_1f80016e;
extern short DAT_1f8000e6[];

static __inline__ void body(O *o)
{
    short d;
    short t;
    if (DAT_800a60d6 == 3)
        d = DAT_1f8000f2 - 0x14 - DAT_800a606c;
    else
        d = DAT_1f8000f2 - DAT_1f80016e;
    t = d + DAT_1f8000e6[0];
    if (t != 0x3a) {
        if (t < 0x3a) {
            if (t < -6) {
                DAT_1f8000f2 += 2;
                if (o->y32 < DAT_1f8000f2) DAT_1f8000f2 = o->y32;
                else d += 2;
            } else {
                DAT_1f8000e6[0] += 2;
            }
            if (d + DAT_1f8000e6[0] > 0x3a) DAT_1f8000e6[0] = 0x3a - d;
        } else {
            DAT_1f8000e6[0] -= 2;
            if (d + DAT_1f8000e6[0] < 0x3a) DAT_1f8000e6[0] = 0x3a - d;
        }
    }
    if (o->y32 < DAT_1f8000f2 + DAT_1f8000e6[0]) DAT_1f8000e6[0] = o->y32 - DAT_1f8000f2;
}

void FUN_80028754(O *o)
{
    body(o);
}
