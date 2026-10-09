// FUNC 80121e5c 624 X009
/* score 4: whole function incl. csv piece func_80122044 (tail). Only diff: game tests the in-range flag with 'beqz v0,L; move v0,zero' where L re-tests the flag (not jump-threaded); here the first test is threaded straight to the end. Tried: && value in int/short/u16/u8 locals and inlines, flag=0 first, if/else zero, ternary. */
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001fb20(TObj *);
extern int FUN_8001f9e0(void);
extern short TileCollideAt(TObj *, short, short);
extern char D_80077CF4[];
extern void *D_8012EA40[];
extern unsigned char D_8012B2CC[];
extern unsigned short D_1F80016A, D_1F80016E;

static __inline__ short ground(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 16)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

static __inline__ int near(TObj *o)
{
    return (unsigned short)(D_1F80016A - o->h->p.whole + 0x64) < 0xf1
        && (unsigned short)(D_1F80016E - o->y.p.whole + 0x50) < 0xc9;
}

void func_80121E5C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->movetab = D_80077CF4;
        o->b6a = 1;
        if (o->wb4 != 3) {
            o->wb4 = 3;
            o->wac = 0x12;
            o->state++;
            o->anim = D_8012EA40[0];
            AnimLoadDuration(o);
        } else {
            o->state = 2;
        }
        break;
    case 1:
        if (AnimAdvance(o)) o->state++;
        break;
    case 2:
        if (!o->visible) return;
        if (o->subtype & 0x80) {
            o->state++;
            if (!D_8012B2CC[FUN_8001f9e0() & 7]) o->timer = 0x12c;
            else o->timer = 0x1e0;
        } else {
            unsigned short r = D_1F80016A - o->h->p.whole + 0x64;
            r = r < 0xf1;
            if (r) r = (unsigned short)(D_1F80016E - o->y.p.whole + 0x50) < 0xc9;
            if (r) {
            if (D_8012B2CC[FUN_8001f9e0() & 7]) o->timer = 0x78;
            else o->timer = 0x3c;
            o->state++;
            }
        }
        break;
    case 3:
        if (--o->timer == 0) {
            o->state = 0;
            o->step++;
        }
        break;
    }
    if (o->visible && !ground(o)) {
        FUN_8001fb20(o);
    }
}
