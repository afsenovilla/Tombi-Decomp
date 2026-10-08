// FUNC 8012dedc 476 X000
typedef struct {
    char p0[1]; unsigned char vis; char p1[2]; unsigned char st, step; char p2[4]; unsigned char b0a; char p3[2]; unsigned char b0d; char p4; unsigned char b0f;
    char p5[0x1e - 0x10]; unsigned short w1e; char p6[2]; int anim; char p7[0x3c - 0x28]; int d3c; char p8[0x68 - 0x40]; unsigned char b68, b69, p9, b6b;
    unsigned short w6c, w6e, w70, w72;
} O;
extern int G2d4, T4d94;
extern void FUN_8001fe6c(O *), FUN_800202b4(O *), FUN_8012dcfc(O *), FUN_8012d9fc(O *), FUN_8001fec0(O *), FUN_80020aec(int), FUN_80018790(O *);
extern int FUN_800203dc(O *);
extern void FUN_80026c50(int, int, int);

void FUN_8012dedc(O *o)
{
    unsigned char c = o->st;
    switch (c) {
    case 0:
        o->w72 = o->w6e = 0xc;
        o->w70 = o->w6c = 6;
        o->w1e = 0xb;
        o->b0a = 0;
        o->b0d = 0;
        o->b69 = 0;
        o->b68 = 0;
        *(signed char *)&o->b0f = -9;
        o->d3c = G2d4;
        o->anim = T4d94;
        FUN_8001fe6c(o);
        if (FUN_800203dc(o))
            o->st = o->st + 1;
        break;
    case 1:
        FUN_800202b4(o);
        if (o->vis != 0) {
            switch (o->step) {
            case 0:
                FUN_8012dcfc(o);
                break;
            case 1:
                FUN_8012d9fc(o);
                break;
            }
        }
        break;
    case 2:
        FUN_800202b4(o);
        if (o->vis != 0) {
            switch (o->step) {
            case 0:
                o->step = 1;
                break;
            case 1:
                FUN_8001fec0(o);
                break;
            case 2:
                FUN_80026c50(0, 1, 1);
                FUN_80020aec(o->b6b);
                o->st = 3;
                break;
            }
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
