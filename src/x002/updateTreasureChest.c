// FUNC 80113484 720 X002
// MATCHING 80113484 720
// Debt left: register asm index (tried ~200 temp/type/order forms: best 7, loads or v0/v1 swapped); one second extern name.
#include "TOBJ.H"
typedef void (*ObjFn)(TObj *);
typedef union { int w; struct { unsigned short a, b; } h; } Stage;
extern Stage DAT_8009c960;
extern Stage DAT_8009c960c; /* debt: second name; with one name CSE (follow-jumps) shares one la between .w and the else .h.a in the anim block, the game reloads; tried casts, switch, goto, ptr temp, inverted if */
extern unsigned char DAT_8009cf9d;
extern int DAT_1f8002e0[];
extern int DAT_1f8002d4[];
extern unsigned char DAT_80115a54[];
extern unsigned char DAT_80115a18[];
extern void **DAT_80115a08[];
extern void **DAT_80115948[];
extern void l2Type0EInitSubtype0();
extern void l2Type0EInitSubtype1();
extern void l2Type0EInitSubtype2();
static ObjFn DAT_80115d90[7] = {
    (ObjFn)l2Type0EInitSubtype0, (ObjFn)l2Type0EInitSubtype1, (ObjFn)l2Type0EInitSubtype2, (ObjFn)l2Type0EInitSubtype0,
    (ObjFn)l2Type0EInitSubtype0, (ObjFn)l2Type0EInitSubtype0, (ObjFn)l2Type0EInitSubtype0,
};
extern void l2Type0EActiveIdle();
extern void l2Type0EActive1();
extern void l2Type0EActive2();
extern void l2Type0EActive3();
extern void l2Type0EActive4();
static ObjFn DAT_80115dac[7] = {
    (ObjFn)l2Type0EActiveIdle, (ObjFn)l2Type0EActive1, (ObjFn)l2Type0EActive2, (ObjFn)l2Type0EActive3,
    (ObjFn)l2Type0EActive4, (ObjFn)l2Type0EActiveIdle, (ObjFn)l2Type0EActiveIdle,
};
extern void l2Type0EHit0();
extern void l2Type0EHit1();
extern void l2Type0EHit2();
extern void l2Type0EHit3();
extern void l2Type0EHit4();
static ObjFn DAT_80115dc8[7] = {
    (ObjFn)l2Type0EHit0, (ObjFn)l2Type0EHit1, (ObjFn)l2Type0EHit2, (ObjFn)l2Type0EHit3,
    (ObjFn)l2Type0EHit4, (ObjFn)l2Type0EHit2, (ObjFn)l2Type0EHit0,
};
extern void FUN_8001fe6c(TObj *);
extern void freeObjectLayer2(TObj *);

void updateTreasureChest(TObj *o)
{
    int ok;
    switch (o->b04) {
    case 0:
        o->w9a = 0;
        if (o->animFrame) {
            o->w9a = o->animFrame;
            o->animFrame = 0;
            ok = o->w9a == 1 && DAT_8009cf9d == 2;
            if (!ok) {
                o->b04 = 3;
                break;
            }
        }
        if (DAT_8009c960.w == 0x30009) {
            o->w1e = 10;
            o->d3c = DAT_1f8002e0[0];
        } else {
            if (DAT_8009c960.h.a == 1 && DAT_8009c960.h.b < 2)
                o->w1e = DAT_80115a54[o->b0c & 0x7f];
            else
                {
                /* debt: register asm; local-alloc otherwise gives the earlier-born index $2 */
                register int i asm("$3");
                i = o->b0c & 0x7f;
                o->w1e = DAT_80115a18[DAT_8009c960.h.a * 4 + i];
            }
            o->d3c = DAT_1f8002d4[0];
        }
        o->b04++;
        o->b0d = 0;
        o->b6a = o->b0f;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->box0 = 0x10;
        o->box1 = 0x20;
        if (o->b0c == 1) {
            o->box2 = 0x11;
            o->box3 = 0x21;
        } else {
            o->box2 = 0xd;
            o->box3 = 0x1d;
        }
        *(signed char *)&o->b0f = -3;
        o->wac = 0;
        if (DAT_8009c960.w == 0x30009)
            o->anim = *DAT_80115a08[o->b0c & 0x7f];
        else
            o->anim = *DAT_80115948[DAT_8009c960c.h.a * 4 + (o->b0c & 0x7f)];
        FUN_8001fe6c(o);
        DAT_80115d90[o->subtype](o);
        break;
    case 1:
        DAT_80115dac[o->subtype](o);
        break;
    case 2:
        DAT_80115dc8[o->subtype](o);
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
