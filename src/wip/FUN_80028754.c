// FUNC 80028754 368 MAIN0
typedef struct O { char p[0x32]; short y32; } O;
extern unsigned char DAT_800a60d6;
extern int DAT_800a606c;
extern unsigned short DAT_1f8000f2;
extern unsigned short DAT_1f80016e;
extern unsigned short DAT_1f8000e6;

void FUN_80028754(O *o)
{
    short d;
    short t;
    unsigned short e;
    char pad[16];
    if (DAT_800a60d6 == 3)
        d = DAT_1f8000f2 - 0x14 - DAT_800a606c;
    else
        d = DAT_1f8000f2 - DAT_1f80016e;
    e = *(volatile unsigned short *)&DAT_1f8000e6;
    t = d + e;
    if (t != 0x3a) {
        if (t < 0x3a) {
            if (t < -6) {
                DAT_1f8000f2 += 2;
                if (o->y32 < (short)DAT_1f8000f2)
                    DAT_1f8000f2 = o->y32;
                else
                    d += 2;
            } else {
                *(volatile unsigned short *)&DAT_1f8000e6 = e + 2;
            }
            if (d + *(volatile short *)&DAT_1f8000e6 > 0x3a)
                *(volatile unsigned short *)&DAT_1f8000e6 = 0x3a - d;
        } else {
            e -= 2;
            *(volatile unsigned short *)&DAT_1f8000e6 = e;
            if (d + (short)e < 0x3a)
                *(volatile unsigned short *)&DAT_1f8000e6 = 0x3a - d;
        }
    }
    if (o->y32 < (short)DAT_1f8000f2 + (short)DAT_1f8000e6)
        DAT_1f8000e6 = o->y32 - DAT_1f8000f2;
}
