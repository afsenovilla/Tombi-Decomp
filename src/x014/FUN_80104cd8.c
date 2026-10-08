// FUNC 80104cd8 240 X014
// MATCHING 80104cd8 240
#include "TOBJ.H"
typedef struct { char p00[0x16]; short s16; char p18[0x2e - 0x18]; unsigned short s2e; char p30[0x40 - 0x30]; short *p40; char p44[0x70 - 0x44]; unsigned short s70; char p72[0x8c - 0x72]; unsigned int d8c; } PL;
extern PL *DAT_8009d2e8;
extern int FUN_8001fddc(int a, int b);
extern int FUN_8001fdac(int a, int b);

void FUN_80104cd8(TObj *o)
{
    int u = *(unsigned short *)((char *)o->anim + 4);
    int t;
    short r;
    PL *pl;
    o->d88 = u;
    if (o->animFrame & 1) {
        o->d88 = ((0x40 < u && 0xbf < u) ? 0x180 : 0x80) - u;
    }
    DAT_8009d2e8->s2e = o->animFrame & 1;
    r = FUN_8001fddc((unsigned char)o->d88, (short)(DAT_8009d2e8->s70 + 0x10));
    DAT_8009d2e8->p40[1] = o->h->p.whole + r;
    pl = DAT_8009d2e8;
    r = FUN_8001fdac((unsigned char)o->d88, (short)(pl->s70 + 0x10));
    pl = DAT_8009d2e8;
    pl->s16 = o->y.p.whole + r;
    pl->d8c = (o->d88 - 0xc0) & 0xff;
}
