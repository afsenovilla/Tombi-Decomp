// FUNC 8012bd6c 896 X001
// MATCHING 8012bd6c 896
#include "TOBJ.H"

typedef struct { short w0; unsigned short w2; short w4; unsigned short w6; } AH;

extern AH *D_8013DAE8[];
extern unsigned char D_8013C800[];
extern char D_80077D0C[];
extern short D_1F80027E, D_1F800284;
extern int Rand(void);
extern void ObjSetFacingToPlayer(TObj *o);
extern void FUN_8001fab4(TObj *o);
extern void func_8012BB18(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);
extern void FUN_800eae0c(short, int, int, int);

static __inline__ int land2(TObj *o)
{
    short t;
    short w;

    if (o->b69 == 1) {
        o->d8c = 0;
        o->wb2 = 0;
        o->b69 = 0;
        o->wae = -1;
        o->b9c = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        t = D_1F80027E;
        if (t < 0) t = -t;
        if (t >= 9) t = 8;
        if (D_1F80027E < 0) t = -t;
        o->d8c = -t & 0xff;
        o->wb2 = t;
        w = D_1F800284;
        o->wb6 = (-t << 2) & 0xff;
        o->b9c = 0;
        o->wae = w;
        return 1;
    }
    return 0;
}

void func_8012BD6C(TObj *o)
{
    unsigned char *p1, *p2;
    int r;

    switch (o->substep) {
    case 0:
        o->b9d = 0;
        r = Rand() & 1;
        if (o->b0c == 0) r = 0;
        if (r == 0) {
            o->substep = 2;
            ObjSetFacingToPlayer(o);
            break;
        }
        o->timer = 0x3c;
        o->wac = 1;
        o->substep++;
        o->anim = D_8013DAE8[1];
        p1 = &D_8013C800[((AH *)o->anim)->w2 * 4];
        o->box0 = *p1++;
        o->box1 = *p1++;
        o->box2 = *p1++;
        o->box3 = *p1;
        o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
    case 1:
        func_8012BB18(o);
        if (--o->timer != -1) break;
        o->substep++;
        ObjSetFacingToPlayer(o);
    case 2:
        o->movetab = D_80077D0C;
        o->wac = 0;
        o->substep++;
        o->anim = D_8013DAE8[0];
        p2 = &D_8013C800[((AH *)o->anim)->w2 * 4];
        o->box0 = *p2++;
        o->box1 = *p2++;
        o->box2 = *p2++;
        o->box3 = *p2;
        o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
    case 3:
        FUN_8001fab4(o);
        func_8012BB18(o);
        land2(o);
        if (o->h->p.whole >= 0x943) {
            o->active = 2;
            o->b04 = 3;
            if (o->visible) {
                FUN_800eae0c(o->h->p.whole + 10, 0x2e, o->d->p.whole, 1);
                FUN_800eae0c(o->h->p.whole - 10, 0x2e, o->d->p.whole, 1);
            }
        }
        if (o->animFrame) {
            if (o->wb2 > 0) break;
        } else {
            if (o->wb2 < 0) break;
        }
        o->state = 8;
        o->substep = 0;
        break;
    }
}
