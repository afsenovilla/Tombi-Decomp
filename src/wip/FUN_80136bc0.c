// FUNC 80136bc0 288 X000
typedef struct Q { char p0[0x16]; short y; } Q;
typedef struct R { char p0[2]; unsigned short w2; } R;
typedef struct O { char p0[0x1c]; unsigned short w1c; char p1[2]; Q *q; } O;
extern unsigned char DAT_8009603c;
extern short DAT_8009604e;
extern R *DAT_80096078;
extern signed char DAT_8009d2b0;
extern unsigned short DAT_1f8001fc, DAT_1f8003c4;
extern char DAT_800960f8;

int FUN_80136bc0(O *o)
{
    int r = 0;
    if (o->q->y < -500) {
        if (DAT_8009604e == -0x2e3 && (short)DAT_80096078->w2 >= 0xbf9) {
            if (o->w1c) r = 1;
            else goto tail;
        }
    } else {
        if (DAT_8009603c != 5 && DAT_8009604e >= -0x12b && (unsigned short)(DAT_80096078->w2 - 0xa1a) < 0x28) {
            if (o->w1c) r = 1;
            else {
tail:
                if (DAT_8009d2b0 == 1 && (DAT_1f8003c4 & DAT_1f8001fc)) {
                    r = 1;
                    DAT_800960f8 = 0;
                    o->w1c = 1;
                }
            }
        }
    }
    return r;
}
