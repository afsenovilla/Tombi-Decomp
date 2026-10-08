// FUNC 8003fd78 464 MAIN0
/* score 10 (b23, was 33): box2 = p[0]; v = p[1]; with o->b69 = 0 between them. Left: the game loads b9c (lbu v1)
   before the zero stores and leaves sh box3 in the beqz delay slot; ours reads b9c into v0 after sh box3 (sched1 keeps
   the load next to the branch, priority 1). Tried: b9c in a local (all types, 3 positions: always folded back),
   in-struct byte store for da0, hill-climb of the store order. Older notes: -fno-schedule-insns, box3 in both branches. */
#include "TOBJ.H"
extern short DAT_80115320[];
extern short TileCollideAt(TObj *o, short x, short y);
extern void FUN_8003fb90(TObj *o);

int ObjTileCollide(TObj *o, short dy, int noMove)
{
    short *p;
    short r;
    short v;
    int a;
    int b;

    p = &DAT_80115320[((short *)o->anim)[1] * 4];
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = p[0];
    o->b69 = 0;
    v = p[1];
    o->wb0 = 0;
    *(unsigned char *)&o->da0 = 0;
    o->bbe = 0;
    o->box3 = v;
    if (o->b9c) {
        if (o->velX >= 0) {
            a = 8;
            b = -8;
        } else {
            a = -8;
            b = 8;
        }
    } else {
        if (o->velH >= 0) {
            a = 8;
            b = -8;
        } else {
            a = -8;
            b = 8;
        }
    }
    r = TileCollideAt(o, o->h->p.whole, dy + (o->y.p.whole + o->box2));
    if (r == 0) {
        r = TileCollideAt(o, o->h->p.whole + a, dy + (o->y.p.whole + o->box2));
        if (r == 0) {
            r = TileCollideAt(o, o->h->p.whole + b, dy + (o->y.p.whole + o->box2));
            if (r == 0)
                return 0;
        }
    }
    if (noMove == 0)
        o->y.p.whole += dy;
    FUN_8003fb90(o);
    return r;
}
