// FUNC 80123344 772 X000
// MATCHING 80123344 772
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"
typedef struct { short t; unsigned short x, y, z; } E;
extern int A00_ITEM_OBJ_ANIM_SUBTYPE0;
extern int A00_ITEM_OBJ_ANIM_SUBTYPE1;
extern unsigned short A00_ITEM_OBJ_PATH_START[];
extern E A00_ITEM_OBJ_PATH_NODES[];
extern void AnimLoadDuration(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void playSFX(int n);
extern void freeObjectLayer5(TObj *o);

void updateA00ItemObj(TObj *o)
{
    TObj *p;
    E *e;
    switch (o->b04) {
    case 0:
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 8;
        o->box3 = 0x10;
        o->b0a = 0;
        switch (o->subtype) {
        case 0:
            o->w1e = 0xb;
            o->b0d = 1;
            o->w08 = 0x79cf;
            *(int *)&o->anim = A00_ITEM_OBJ_ANIM_SUBTYPE0;
            goto common;
        case 1:
            o->b0d = 0;
            o->w1e = 8;
            *(int *)&o->anim = A00_ITEM_OBJ_ANIM_SUBTYPE1;
        common:
            o->d3c = SPAD.i2d4;
            break;
        }
        AnimLoadDuration(o);
        goto next;
    case 1:
        if (ObjCullRegister(o) == 0) break;
        p = (TObj *)o->d94;
        switch (o->subtype) {
        case 0:
            o->h->p.whole = p->h->p.whole + 12;
            o->y.p.whole = p->y.p.whole;
            o->d->p.whole = p->d->p.whole;
            if (((unsigned char*)&GCD.bd0aa)[0] == 0) break;
            goto next;
        case 1:
            switch (o->step) {
            case 0: {
                unsigned short v = A00_ITEM_OBJ_PATH_START[(unsigned short)p->wb8];
                o->wb8 = v;
                o->w22 = A00_ITEM_OBJ_PATH_NODES[v].t;
                o->step++;
            }
            case 1:
                e = &A00_ITEM_OBJ_PATH_NODES[(unsigned short)o->wb8];
                if (e->t == -2) {
                    o->wb8--;
                    e = &A00_ITEM_OBJ_PATH_NODES[(unsigned short)o->wb8];
                }
                if (e->t == -1) {
                    unsigned short v = A00_ITEM_OBJ_PATH_START[(unsigned short)p->wb8];
                    e = &A00_ITEM_OBJ_PATH_NODES[v];
                    o->wb8 = v;
                }
                if (--o->w22 == 0) {
                    o->wb8++;
                    o->w22 = e->t;
                }
                o->h->p.whole = p->h->p.whole + e->x;
                o->y.p.whole = p->y.p.whole + e->y;
                o->d->p.whole = p->d->p.whole;
                break;
            }
            if (((unsigned char*)&GCD.bd0c8)[0] == 0) break;
            goto next;
        }
        break;
    case 2:
        playSFX(9);
    next:
        o->b04++;
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
