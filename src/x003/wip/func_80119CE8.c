// FUNC 80119ce8 288 X003
/* score 71: loop hoisting now matches (dd0 store written in both branches: D_801358AC life 2 gets hoisted,
   D_8013592C not; game regs t3/t4). Left: game loads x,y,z (t0-t2) at the loop top and p->d94 (a0) right after the
   first lbu subtype, and keeps o in a2 (move a2,a0); ours schedules those loads late and leaves o in a0.
   Tried: inline wrapper, pad sizes (72 fixes the frame), early n = p->d94 temp, hill-climb of the loop body. */
#include "TOBJ.H"
typedef struct {
    TObj o;
    void *dc0, *dc4, *dc8, *dcc, *dd0;
} TObjX;
#define X(p) ((TObjX *)(p))
extern char D_80135974[], D_801358AC[];
extern char D_8013592C[], D_80135934[], D_8013593C[];

void func_80119CE8(TObj *o)
{
    TObj *p, *q;
    int x, y, z;
    char pad[72];

    o->b6b = 0;
    if (o->d90 != 0) return;
    p = o;
    q = p;
    while (1) {
        X(p)->dc4 = D_8013592C + o->subtype * 8;
        x = p->a.raw;
        y = p->y.raw;
        z = p->b.raw;
        p->wb4 = 0;
        p->wb6 = 0;
        *(void **)&p->wb8 = D_80135974;
        *(void **)&p->wbc = D_80135974;
        X(p)->dc0 = D_80135974;
        X(p)->dc8 = D_80135934 + o->subtype * 8;
        X(p)->dcc = D_8013593C + o->subtype * 8;
        if (p->d94 == 0) {
            p->box0 = 0x14;
            p->box1 = 0x28;
            p->box2 = 0xf;
            p->box3 = 0x1e;
            X(p)->dd0 = D_801358AC;
        } else {
            p->active = 2;
            X(p)->dd0 = D_801358AC;
        }
        p = (TObj *)p->d94;
        if (p == 0) break;
        q->d30 = p->a.raw - x;
        q->d34 = p->y.raw - y;
        q->d38 = p->b.raw - z;
        q = p;
    }
}
