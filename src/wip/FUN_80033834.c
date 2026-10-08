// FUNC 80033834 296 MAIN0
typedef struct { char p0[6]; unsigned char step; char p1[0x20 - 7]; short timer; char p2[0x2e - 0x22]; unsigned short af; char p3[0x7a - 0x30]; short w7a; char p4[0xaa - 0x7c]; short waa; short wac; } O;
extern short DAT_8007a074[];
extern char DAT_800a6038[];
extern int DAT_800a60c4;
extern void FUN_800312e8(void);
extern void AnimJump(char *, int);
extern short FUN_800331c4(O *);

void FUN_80033834(O *o)
{
    int f;
    int r;
    short s;
    short t;
    f = o->af;
    f = f * 3;
    FUN_800312e8();
    AnimJump(DAT_800a6038, 0);
    o->wac = DAT_8007a074[(short)f];
    switch (o->af & 7) {
    case 0:
    case 2:
        o->waa = 0;
        break;
    case 1:
    case 3:
        o->waa = 0x7f;
        break;
    case 4:
        o->waa = 0x20;
        break;
    case 5:
        o->waa = 0x60;
        break;
    case 6:
    case 7:
        o->waa = 0x40;
        break;
    }
    r = 0;
    o->w7a = 2;
    DAT_800a60c4 = 0;
    s = FUN_800331c4(o);
    if (s >= 0) {
        if (s < 2) {
            o->timer = 5;
        } else if (s == 2) {
            o->timer = 5;
            r = 1;
        } else {
            r = 0;
        }
    }
    o->step = r ? 3 : 2;
}
