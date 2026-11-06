// FUNC 80132964 216 X001
// MATCHING 80132964 216
// VERSION SLPS_01144 x01 80131240 216
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

extern short probeWallAt(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

short func_80131240(TObj *o)
{
    int d = o->d88;
    unsigned char c = (unsigned int)(d - 0x40) < 0x80;

    if (o->b69 != 0) {
        if (c) {
            o->d88 = d - 0x10;
        } else {
            o->d88 = d + 0x10;
        }
        return 1;
    }
    if (d < 0x81) {
        if (!probeWallAt(o, o->h->p.whole, o->y.p.whole, c)) goto zero;
        return 2;
    }
    if (probeWallAt(o, o->h->p.whole, o->y.p.whole, c)) {
        return 2;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole)) return 1;
zero:
    return 0;
}
#else
#include "TOBJ.H"

extern short probeWallAt(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

short segmentedCrawlerCheckWall(TObj *o)
{
    int d = o->d88;
    unsigned char c = (unsigned int)(d - 0x40) < 0x80;

    if (o->b69 != 0) {
        if (c) {
            o->d88 = d - 0x10;
        } else {
            o->d88 = d + 0x10;
        }
        o->d88 = *(unsigned char *)&o->d88;
        return 1;
    }
    if (d < 0x81) {
        if (!probeWallAt(o, o->h->p.whole, o->y.p.whole, c)) goto zero;
        return 2;
    }
    if (probeWallAt(o, o->h->p.whole, o->y.p.whole, c)) {
        return 2;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole)) goto one;
zero:
    return 0;
one:
    return 1;
}
#endif
