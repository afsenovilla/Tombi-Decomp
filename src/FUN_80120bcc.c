// FUNC 80120bcc 312 X000
// MATCHING 80120bcc 312
#include "TOBJ.H"
extern void FUN_80040278(TObj *, int, int);
extern short DAT_1f80027e;
extern Fix16 *DAT_800a6078;
extern Fix16 *DAT_800a607c;
typedef struct G { char p[0x16]; short y; } G;
extern G DAT_800a6038;
extern int DAT_800a60c4;

void FUN_80120bcc(TObj *o)
{
    int s, t;
    short y;
    s = o->state;
    t = s & 0xff;
    if (t != 1) {
        if (t < 2 && t == 0) {
        o->state = s + 1;
        o->wb6 = 0x180;
        o->wb4 = 0;
        o->velH = 0;
        }
    } else {
        o->velX = (short)*(unsigned short *)&o->wb6 >> 1;
        o->h->raw = o->h->raw + o->wb6 * 0x100;
        y = o->y.p.whole;
        o->y.p.whole = y + 2;
        FUN_80040278(o, o->h->p.whole, (short)(y + 0x12));
        o->d84 = (-(int)DAT_1f80027e << 6) & 0xfff;
        DAT_800a6078->p.whole = o->h->p.whole;
        DAT_800a6038.y = o->y.p.whole - 8;
        DAT_800a607c->p.whole = o->d->p.whole;
        DAT_800a60c4 = o->d84 >> 4;
    
    }
    o->velH = o->velH - o->velX;
}
