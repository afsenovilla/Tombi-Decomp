// FUNC 8012eae8 1244 X001
// MATCHING 8012eae8 1244
#include "TOBJ.H"
#include "raw7.h"

typedef struct { short w0; unsigned short w2; short w4; unsigned short w6; } AH;

extern AH *D_8013DAE8[];
extern unsigned char D_8013C800[];
extern char D_80077D30[];
extern short D_1F80027E, D_1F800284;
extern unsigned short D_1F800176, D_1F80017A[];
extern int func_800203DC(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void FUN_8001fb20(TObj *o);
extern void FUN_8001fab4(TObj *o);
extern void func_8012BB18(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);

static __inline__ int land(TObj *o, short *q)
{
    short t;

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
        o->wae = D_1F800284;
        q[1] = (-t << 2) & 0xff;
        o->b9c = 0;
        return 1;
    }
    return 0;
}

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

void func_8012EAE8(TObj *o)
{
    short *q = &o->wb4;
    unsigned char *p;
    unsigned short d;

    switch (o->state) {
    case 0:
        o->b9d = 0;
        o->b69 = 0;
        o->movetab = D_80077D30;
        S16(o, 0xc4) = o->animFrame;
        if (o->b0c == 0) {
            o->state = 2;
            o->wac = 0;
            o->y.p.whole = D_1F80017A[0] - 0xf0;
        } else {
            if (o->b0c == 2) {
                o->wac = 0xe;
                o->state++;
            } else {
                o->wac = 0;
                o->state++;
            }
        }
        o->anim = D_8013DAE8[o->wac];
        p = &D_8013C800[((AH *)o->anim)->w2 * 4];
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p++;
        o->box3 = *p;
        o->animTimer = ((AH *)o->anim)->w6 & 0x3fff;
        break;
    case 1:
        if (func_800203DC(o)) o->state++;
        break;
    case 2:
        if (o->b0c == 0) {
            if (o->animFrame == 0) {
                o->h->p.whole = D_1F800176 - 0x10;
                if (o->h->p.whole < 0x3ca) {
                    o->b04 = 3;
                    break;
                }
            } else {
                o->h->p.whole = D_1F800176 + 0x150;
            }
        }
        FUN_8001fb20(o);
        if (land2(o)) {
            d = q[1];
            o->active = 1;
            o->timer = 0;
            o->d8c = d;
            o->state++;
        }
        break;
    case 3:
        switch (o->b0c) {
        case 0:
            ObjCullRegister(o);
            FUN_8001fab4(o);
            func_8012BB18(o);
            land(o, q);
            if ((unsigned short)(o->h->p.whole - 0x30 - D_1F800176) < 0xe0) {
                o->state = 8;
                o->substep = 0;
                o->step++;
                if (o->animFrame) {
                    if (o->wb2 > 0) {
                        o->state = 9;
                        o->substep = 2;
                    }
                } else {
                    if (o->wb2 < 0) {
                        o->state = 9;
                        o->substep = 2;
                    }
                }
            }
            break;
        case 1:
            if (ObjCullRegister(o)) {
                o->state = 9;
                o->substep = 0;
                o->step++;
            }
            break;
        case 2:
            if (ObjCullRegister(o)) func_8012BB18(o);
            break;
        }
        break;
    }
}
