// FUNC 80119ce8 288 X003
// MATCHING 80119ce8 288
/* debt: empty-loop barrier after the y load keeps the x/y/z loads at the loop top. */
#include "TOBJ.H"
typedef struct {
    char pad[0xb8];
    void *db8, *dbc, *dc0, *dc4, *dc8, *dcc, *dd0;
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
        x = p->a.raw;
        y = p->y.raw;
        do {} while (0);
        z = p->b.raw;
        p->wb4 = 0;
        p->wb6 = 0;
        X(p)->db8 = D_80135974;
        X(p)->dbc = D_80135974;
        X(p)->dc0 = D_80135974;
        X(p)->dc4 = D_8013592C + o->subtype * 8;
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
