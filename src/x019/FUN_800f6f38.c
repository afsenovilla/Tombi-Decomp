// FUNC 800f6f38 292 X019
// MATCHING 800f6f38 292
#include "TOBJ.H"
extern unsigned char *DAT_8009c330;
extern int FUN_8001fe3c(int, int);
extern int FUN_8001fe0c(int, int);

void FUN_800f6f38(TObj *o, int p)
{
    int s;
    int ang;
    short r;
    if (o->animFrame & 1) {
        ang = 0x1bf; ang -= p;
        s = FUN_8001fe3c(ang & 0xff, DAT_8009c330[1]);
        o->h->p.whole = s + (o->wb8 + o->d30);
        s = FUN_8001fe0c(ang & 0xff, DAT_8009c330[1]);
        o->d84 = 0;
        o->d8c = 0x100 - (unsigned char)p;
        r = s + (o->wba + o->d34);
    } else {
        ang = p + 0xc0;
        s = FUN_8001fe3c(ang & 0xff, DAT_8009c330[1]);
        o->h->p.whole = s + (o->wb8 + o->d30);
        s = FUN_8001fe0c(ang & 0xff, DAT_8009c330[1]);
        o->d84 = 0;
        o->d8c = (unsigned char)p;
        r = s + (o->wba + o->d34);
    }
    o->y.p.whole = r;
}
