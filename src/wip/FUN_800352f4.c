// FUNC 800352f4 308 MAIN0
// wip: falta orden de stores 6068/606c frente a lw 6078 y la posicion del primer mult (score 59)
#include "TOBJ.H"
extern char *DAT_8009c338;
extern char DAT_800a6100[];
extern int DAT_800a6068;
extern int DAT_800a606c;
extern int DAT_8009c934;
extern Fix16 * DAT_800a6078;
extern short DAT_800a604e;
extern short DAT_800a60ec, DAT_800a60b2;
extern unsigned char DAT_800a60d4;
extern void SfxPlay(int);
extern int csqrt(int);
extern short FUN_800335d4(TObj *);

void FUN_800352f4(TObj *o)
{
    int dx, dy;

    DAT_8009c338[0xd] = 0;
    SfxPlay(0x26);
    DAT_800a6100[0] = 1;
    DAT_800a6068 = o->h->p.whole;
    DAT_800a606c = o->y.p.whole + 4;
    dx = o->h->p.whole - DAT_800a6078->p.whole;
    if (dx < 0) dx = -dx;
    dy = o->y.p.whole - DAT_800a604e;
    if (dy < 0) dy = -dy;
    dx = csqrt((dx * dx + dy * dy) << 12);
    DAT_800a60ec = dx >> 12;
    if (DAT_800a60d4 == 0) DAT_800a60ec = DAT_800a60ec - 0x10;
    if (DAT_800a60ec < 0x28) DAT_800a60ec = 0x28;
    DAT_800a60b2 = FUN_800335d4(o);
    o->step = 3;
    o->state = 0;
    o->d90 = DAT_8009c934;
}
