// FUNC 80103290 920 X005
// MATCHING 80103290 920
#include "TOBJ.H"
extern TObj *D_8009D2E8;
extern unsigned short D_8009D670;
extern unsigned short D_1F8001FC, D_1F8003C6;
extern unsigned char D_8009D2B0[];
void ObjMotionStep(TObj *o);
void PlayerSetAnimIfChanged(TObj *o, int n);
void FUN_8001f96c(int, int, int, int);
short ObjTileCollide(TObj *, int, int);
int func_8013675C(short, short, short, short, short, short);
#define BLOCK()                                                                     \
    {                                                                               \
        o->timer = 0;                                                               \
        PlayerSetAnimIfChanged(o, 0xd);                                             \
        FUN_8001f96c(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);                  \
        *(unsigned char *)&o->wac = 3;                                              \
        o->b9c = 0;                                                                 \
        o->velX = 0;                                                                \
        o->velY = 0;                                                                \
        o->h->p.whole = D_8009D2E8->h->p.whole;                                     \
        o->y.p.whole = D_8009D2E8->y.p.whole - (D_8009D2E8->box3 - D_8009D2E8->box2); \
        o->state = 2;                                                               \
    }
void func_80103290(TObj *o)
{
    short f;
    TObj *e;
    volatile unsigned short *k;
    switch (o->state) {
    case 1:
        o->timer++;
        ObjMotionStep(o);
        f = 0;
        o->y.p.whole += 8;
        if (o->y.p.whole + D_8009D2E8->box2 >= D_8009D2E8->y.p.whole) {
            o->y.p.whole = D_8009D2E8->y.p.whole - D_8009D2E8->box2;
            f = 1;
        }
        if (o->velX < 0) {
            o->h->p.whole -= 8;
            if (o->h->p.whole <= D_8009D2E8->h->p.whole) {
                o->h->p.whole = D_8009D2E8->h->p.whole;
                if (f) BLOCK();
            }
        } else {
            o->h->p.whole += 8;
            if (o->h->p.whole >= D_8009D2E8->h->p.whole) {
                o->h->p.whole = D_8009D2E8->h->p.whole;
                if (f) BLOCK();
            }
        }
        if ((o->b69 | ObjTileCollide(o, 0, 1)) || o->timer >= 11) BLOCK();
        break;
    case 2:
        k = &D_8009D670;
        if (*k & 0x80) o->animFrame = 1;
        if (*k & 0x20) o->animFrame = 0;
        o->h->p.whole = D_8009D2E8->h->p.whole;
        if (D_8009D2E8->type == 0xb) {
            if (D_8009D2E8->subtype == 0) o->y.p.whole = D_8009D2E8->y.p.whole - (D_8009D2E8->box3 - D_8009D2E8->box2) + 8;
            else o->y.p.whole = D_8009D2E8->y.p.whole - (D_8009D2E8->box3 - D_8009D2E8->box2);
        }
        if (D_1F8001FC & D_1F8003C6) {
            D_8009D2B0[0] = 0;
            o->b9c = 1;
            o->b69 = 0;
            o->wb2 = 0;
            e = D_8009D2E8;
            if (e->type == 0xb) {
                if (e->b0c)
                    *(int *)((char *)o + 0x90) = func_8013675C(6, 0, e->b0c, e->a.p.whole, e->y.p.whole, e->b.p.whole);
                D_8009D2E8->state = 1;
            }
            o->step = 0xf;
            o->state = 0;
        }
        break;
    }
}
