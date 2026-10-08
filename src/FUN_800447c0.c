// FUNC 800447c0 480 MAIN0
// MATCHING 800447c0 480
#include "TOBJ.H"
typedef struct { unsigned short x, y; } P2;
extern P2 DAT_8007b610[];
extern short DAT_1f80019e;
extern int FUN_800425c4(TObj *, TObj *, int);
extern void FUN_8001f96c(int, int, int, int);
extern void SfxPlay(int);

void FUN_800447c0(TObj *o, TObj *q)
{
    P2 *t;
    int tx;
    unsigned short ty;
    int n;
    int s;
    unsigned short b0;

    if ((unsigned short)(o->d->p.whole - q->d->p.whole + 0x2d) > 0x5a) return;
    { unsigned short *u = &DAT_8007b610[q->b0c].x;
    b0 = o->box0;
    tx = *u++;
    ty = *u; }
    if ((unsigned short)(o->h->p.whole - (q->h->p.whole + tx) + (q->box0 + b0)) > q->box1 + o->box1) return;
    n = 1;
    if ((unsigned short)(o->y.p.whole - (q->y.p.whole + ty) - 4 + (q->box2 + o->box2)) > o->box3 + q->box3 - 4) return;
    q->b68 = 1;
    q->b9e = 0;
    switch (FUN_800425c4(o, q, b0)) {
    case 1: case 7:
        n = 1;
        break;
    case 4: case 5: case 10:
        n = 1;
        o->b6a = 1;
        o->active = 2;
        o->wa8 = 0x4ff;
        break;
    case 0: case 3: case 9:
        n = 2;
        break;
    case 13:
        q->b68 = 0;
        n = -1;
        break;
    case 6: case 11: case 12:
        n = 2;
        o->b6a = 1;
        o->active = 2;
        o->wa8 = 0x4ff;
        break;
    case 2: case 8:
        n = 0;
        q->b9e = 1;
        q->b9f = 0;
        break;
    }
    if (n >= 0) {
        FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        s = 7;
        if ((q->category & 0x7f) == 4) s = 6;
        SfxPlay(s);
    }
    DAT_1f80019e = 0;
}
