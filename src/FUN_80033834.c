// FUNC 80033834 296 MAIN0
// MATCHING 80033834 296
typedef struct { char p0[6]; unsigned char step; char p1[0x20 - 7]; short timer; char p2[0x2e - 0x22]; unsigned short af; char p3[0x7a - 0x30]; short w7a; char p4[0xaa - 0x7c]; short waa; short wac; } O;
extern short DAT_8007a074[];
extern char PLAYER[];
extern int DAT_800a60c4;
extern void weaponSetPlayerAnimB(void);
extern void AnimJump(char *, int);
extern short weaponFindNearestTarget(O *);

void FUN_80033834(O *o)
{
    int f;
    short r;
    short s;
    short t;
    f = o->af;
    f = f * 3;
    weaponSetPlayerAnimB();
    AnimJump(PLAYER, 0);
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
    s = weaponFindNearestTarget(o);
    switch (s) {
    case 0:
    case 1:
        o->timer = 5;
        break;
    case 2:
        o->timer = 5;
        r = 1;
        break;
    }
    o->step = r ? 3 : 2;
}
