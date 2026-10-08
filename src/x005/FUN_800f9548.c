// FUNC 800f9548 316 X005
// MATCHING 800f9548 316
#include "TOBJ.H"
typedef struct G { char p0[2]; short w2; char p1[4]; unsigned char b8; char p2[5]; short we; } G;
extern unsigned char *DAT_8009c330;
extern G *DAT_8009c330b[];
extern int FUN_8001fe3c(int, int);
extern int FUN_8001fe0c(int, int);

void FUN_800f9548(TObj *o, int b, int p)
{
    int s;
    int ang;
    short r;
    G *g;
    int d;
    if (o->animFrame & 1) {
        ang = 0x1bf; ang -= p;
        s = FUN_8001fe3c(ang & 0xff, DAT_8009c330[1]);
        o->h->p.whole = s + o->d30;
        s = FUN_8001fe0c(ang & 0xff, DAT_8009c330[1]);
        o->d84 = 0;
        o->d8c = 0x100 - (unsigned char)p;
        r = s + o->d34;
    } else {
        ang = p + 0xc0;
        s = FUN_8001fe3c(ang & 0xff, DAT_8009c330[1]);
        o->h->p.whole = s + o->d30;
        s = FUN_8001fe0c(ang & 0xff, DAT_8009c330[1]);
        o->d84 = 0;
        o->d8c = (unsigned char)p;
        r = s + o->d34;
    }
    o->y.p.whole = r;
    g = DAT_8009c330b[0];
    d = g->we;
    if (g->b8) {
        s = g->w2;
        g->we = d - s;
    } else {
        s = g->w2;
        g->we = d + s;
    }
}
