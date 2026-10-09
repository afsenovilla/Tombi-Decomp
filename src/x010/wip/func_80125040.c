/* score 15 (was 27): case 0 with box stores as inline setbox(o, 8, 0x10, 8, 0x10) (8 loads early into v1, t into a0 as in
   the game). Left: sb b0f/sb b0d/li 5 scheduled after the TBL/D_1F8002DC loads (game: b0f, w1e, b0d before sw a8; only
   sh wac after the loads). Raw (non-struct) b0d/w1e stores with d3c last pin them before the load but break the
   cross-jumped anim tail with case 4 (reg of the TBL reload / d3c value, 102+). Tried setbox body orders/param types,
   TB-struct TBL, `TBL(o)[o->wac]`, do/while anim, hill-climb of the 11 statements. */
// FUNC 80125040 876 X010
#include "TOBJ.H"
#include "raw7.h"

extern unsigned char D_8009D2C3, D_8009C942;
extern void **D_8012F3A8[];
extern int D_1F8002DC;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern void FUN_80018790(TObj *);
extern int ObjCullRegister(TObj *);
extern int func_800203DC(TObj *);
extern void AnimLoadDuration(TObj *);
extern void func_80124370(TObj *);
extern void func_80124A00(TObj *);
extern void func_80124F0C(TObj *);

#define TBL(o) (*(void ***)((char *)(o) + 0xa8))

static __inline__ void setbox(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

void func_80125040(TObj *o)
{
    short *sv = &o->wb4;
    void **t;

    switch (o->b04) {
    case 0:
        if (D_8009D2C3 & 0x40) {
            FUN_80018790(o);
            break;
        }
        o->b0c = o->subtype;
        sv[0] = o->h->p.whole;
        sv[1] = o->y.p.whole;
        sv[2] = o->d->p.whole;
        o->d38 = 0;
        o->d8c = 0;
        t = D_8012F3A8[o->subtype];
        o->b04++;
        TBL(o) = t;
        o->d3c = D_1F8002DC;
        o->b0d = 0;
        setbox(o, 8, 0x10, 8, 0x10);
        *(signed char *)&o->b0f = -9;
        o->w1e = 5;
        o->wac = 0;
        o->anim = TBL(o)[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (D_8009C942) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_80124370(o);
            break;
        case 1:
            func_80124A00(o);
            break;
        }
        if ((unsigned)((o->d38 - 0x40) & 0xff) < 0x80) {
            o->animFrame = 1;
            o->d8c = (o->d38 + 0x80) & 0xff;
        } else {
            o->animFrame = 0;
            o->d8c = o->d38;
        }
        o->b9d = 0;
        o->b69 = 0;
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 1:
            switch (o->state) {
            case 0:
                *(signed char *)&o->b0f = -7;
                o->wac = 3;
                o->b9c = 0;
                o->b0c = o->subtype;
                o->state++;
                o->anim = TBL(o)[3];
                AnimLoadDuration(o);
                break;
            case 1:
                break;
            case 2:
                o->state++;
                break;
            }
            break;
        case 0:
        case 2:
            func_80124F0C(o);
            if (!o->visible) o->b04 = 3;
            break;
        }
        break;
    case 3:
        *(signed char *)&o->b0f = -9;
        o->b0b = 0;
        o->h->p.whole = sv[0];
        o->y.p.whole = sv[1];
        o->d->p.whole = sv[2];
        o->b04++;
        break;
    case 4:
        if (((D_1F8001F8 + D_1F800198) & 0xf) == 0 && !func_800203DC(o)) {
            o->active = 1;
            o->b9d = 0;
            o->b69 = 0;
            o->d38 = 0;
            o->d30 = 0;
            o->velX = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            o->wac = 0;
            o->anim = TBL(o)[0];
            AnimLoadDuration(o);
        }
        break;
    }
}
