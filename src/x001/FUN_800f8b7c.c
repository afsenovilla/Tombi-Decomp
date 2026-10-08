// FUNC 800f8b7c 432 X001
// MATCHING 800f8b7c 432
#include "TOBJ.H"
extern TObj *DAT_8009f0ec;
extern unsigned char *DAT_8009c330;
extern int FUN_8001fe3c(unsigned char a, unsigned char r);
extern int FUN_8001fe0c(unsigned char a, int r);

void FUN_800f8b7c(TObj *o, int unused, int p3)
{
    TObj *pl = DAT_8009f0ec;
    int ang;
    int s, t;
    unsigned char *c;
    o->d30 = pl->h->p.whole + o->wb8;
    o->d34 = pl->y.p.whole + o->wba;
    if (o->animFrame & 1) {
        ang = (0x1bf - p3) & 0xff;
        o->h->p.whole = FUN_8001fe3c(ang, DAT_8009c330[1]) + o->d30;
        s = FUN_8001fe0c(ang, DAT_8009c330[1]);
        t = FUN_8001fe0c((DAT_8009f0ec->d8c >> 4) + 0x70, DAT_8009f0ec->box0);
        o->d84 = 0;
        o->d8c = 0x100 - (unsigned char)p3;
        s += o->d34;
        o->y.p.whole = t + s;
    } else {
        ang = (p3 + 0xc0) & 0xff;
        o->h->p.whole = FUN_8001fe3c(ang, DAT_8009c330[1]) + o->d30;
        s = FUN_8001fe0c(ang, DAT_8009c330[1]);
        t = FUN_8001fe0c((DAT_8009f0ec->d8c >> 4) + 0x10, DAT_8009f0ec->box0);
        o->d84 = 0;
        o->d8c = (unsigned char)p3;
        s += o->d34;
        o->y.p.whole = t + s;
    }
    c = DAT_8009c330;
    *(short *)(c + 0xe) += c[8] ? -*(short *)(c + 2) : *(short *)(c + 2);
}
