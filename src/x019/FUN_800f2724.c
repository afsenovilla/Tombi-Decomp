// FUNC 800f2724 380 X019
// MATCHING 800f2724 380
#include "TOBJ.H"
typedef struct { short x, y; } XY;
extern TObj *DAT_8009c330;
extern short DAT_1f800238;
extern unsigned short DAT_1f8001f8;
extern XY DAT_801151e0[];
extern void FUN_800efc04(TObj *, TObj *);
extern void FUN_8001fe94(TObj *, int);
extern short FUN_800eef0c(void);
extern TObj *FUN_80018448(void);
extern void FUN_800eec40(TObj *);
extern void FUN_8010eaf8(TObj *);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);

void FUN_800f2724(TObj *o)
{
    TObj *p = DAT_8009c330;
    TObj *q;
    short x, y, z;
    XY *t;
    p->animTimer = 4;
    if (p->animFrame != 4) {
        p->animTimer = 4;
        FUN_800efc04(o, p);
        FUN_8001fe94(o, 0);
        DAT_8009c330->animFrame = DAT_8009c330->animTimer;
    }
    if (o->ba7 != 0) {
        x = o->a.p.whole;
        y = o->y.p.whole;
        z = o->b.p.whole;
        if (FUN_800eef0c() == 0 && DAT_1f800238 >= 6 && (q = FUN_80018448()) != 0) {
            q->active = 1;
            q->type = 0x31;
            q->subtype = 1;
            t = &DAT_801151e0[DAT_1f8001f8 & 7];
            q->a.p.whole = x;
            q->y.p.whole = y;
            q->b.p.whole = z;
            q->h->p.whole += t->x;
            q->y.p.whole += t->y;
        }
    }
    FUN_800eec40(o);
    FUN_8010eaf8(o);
    o->h->raw += o->velX << 8;
    FUN_8010f0f4(o);
    FUN_8001fd94(o);
}
