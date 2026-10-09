// FUNC 80122fa0 1784 X014
// MATCHING 80122fa0 1784
/* covers csv entries 80122fa0 (596 B) and 801231f4 (1188 B): one function cut at a jump into the middle */
#include "TOBJ.H"

extern TObj D_800A6038;
extern int D_1F8002DC[];
extern void *D_80129F9C[];

extern unsigned short D_8009C962[];
extern unsigned char D_8009C942;
extern unsigned char D_8009C958;
extern short D_800A457C;
extern short D_800A457E;
extern short D_8007A1F0[], D_8007A5F0[];
void AnimLoadDuration(TObj *o);
void AnimAdvance(TObj *o);
void ObjCullRegister(TObj *o);
void playSFX(int a);
void func_8002052C(TObj *o, int a);
void FUN_80018790(TObj *o);

#define SFX(o) \
    if ((o->w22 & 0x7f) == 0) { \
        playSFX(D_8009C962[0] == 7 ? 0xef : 0xca); \
    } \
    o->w22++;

static __inline__ short clamp(TObj *o)
{
    if (o->subtype & 1) {
        if (o->h->p.whole < D_800A457C - 0x90) {
            o->h->p.whole = D_800A457C - 0x90;
            return 1;
        }
    } else {
        if (D_800A457E + 0x90 < o->h->p.whole) {
            o->h->p.whole = D_800A457E + 0x90;
            return 1;
        }
    }
    return 0;
}

void func_80122FA0(TObj *o)
{
    TObj *p = &D_800A6038;
    short dx, dy;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 0x18;
        o->box1 = 0x30;
        o->box2 = 0x40;
        o->box3 = 0x70;
        *(signed char *)&o->b0f = -0x10;
        o->velX = 0x80;
        o->b0a = 10;
        o->w1e = 1;
        o->b0d = 0x80;
        o->b6a = 0;
        o->ba5 = 0;
        o->ba6 = 0;
        o->ba7 = 0;
        o->animFrame = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d3c = D_1F8002DC[0];
        o->anim = D_80129F9C[o->subtype];
        AnimLoadDuration(o);
        o->timer = 0x14;
        o->velH = 0x180;
        o->w22 = 0;
        playSFX(D_8009C962[0] == 7 ? 0xef : 0xca);
        o->w22++;
        break;
    case 1:
        if (D_8009C942 == 1) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        AnimAdvance(o);
        switch (o->step) {
        case 0:
            SFX(o);
            if (o->b6a) {
                o->timer = 300;
                o->step++;
                break;
            }
            if (o->timer == 0) {
                o->ba5 += 2;
                if (o->ba5 >= 0xc0) {
                    *(signed char *)&o->ba5 = -0x40;
                }
                o->ba5 = o->ba5;
                o->ba6 = o->ba5;
            } else {
                o->timer--;
            }
            {
                int v = o->velH;
                int i = ((unsigned short)o->wb4 << 4) & 0xf0;
                short a = (v * D_8007A5F0[i]) >> 12;
                short b = (v * D_8007A1F0[i]) >> 12;
                o->h->raw += a << 8;
                o->y.raw += b << 8;
            }
            if (clamp(o)) {
                o->active = 2;
                o->b04++;
                break;
            }
            func_8002052C(o, 0x50);
            break;
        case 1:
            SFX(o);
            o->ba5++;
            if (o->ba5 >= 0xf0) {
                *(signed char *)&o->ba5 = -0x10;
            }
            o->ba5 = o->ba5;
            o->ba6 = o->ba5;
            if (o->h->p.whole != p->h->p.whole) {
                if (p->h->p.whole >= o->h->p.whole) {
                    dx = -1;
                } else {
                    dx = 1;
                }
            } else {
                dx = 0;
            }
            p->h->p.whole += dx;
            if ((short)(o->y.p.whole - 0x20) != p->y.p.whole) {
                if (p->y.p.whole >= (short)(o->y.p.whole - 0x20)) {
                    dy = -1;
                } else {
                    dy = 1;
                }
            } else {
                dy = 0;
            }
            p->y.p.whole += dy;
            if (o->subtype) {
                p->d8c += 0x10;
            } else {
                p->d8c -= 0x10;
            }
            p->active = 6;
            p->d8c = (unsigned char)p->d8c;
            if (dx != 0 || dy != 0) {
                if (--o->timer != -1) break;
            }
            o->velV = 0;
            o->timer = 0x1e;
            o->step++;
            break;
        case 2:
            SFX(o);
            p->active = 6;
            if (o->subtype) {
                p->d8c += 0x14;
            } else {
                p->d8c -= 0x14;
            }
            p->d8c = (unsigned char)p->d8c;
            o->velV += 0x40;
            if (o->velV > 0x300) {
                o->velV = 0x300;
            }
            p->y.raw -= o->velV << 8;
            if (--o->timer == 0) {
                o->step++;
            }
            o->velX -= 2;
            if (o->velX < 0) {
                o->velX = 0;
            }
            break;
        case 3:
            o->velX -= 2;
            if (o->velX < 0) {
                o->velX = 0;
            }
            p->active = 1;
            D_800A6038.step = 0x47;
            D_800A6038.animFrame = 2;
            D_800A6038.d8c = 0;
            D_800A6038.b04 = 1;
            D_800A6038.state = 1;
            D_800A6038.velY = 0x200;
            o->active = 2;
            o->b04++;
            break;
        }
        break;
    case 2:
        if (D_8009C942 == 1) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        AnimAdvance(o);
        o->velX -= 4;
        if (o->velX < 0) {
            o->velX = 0;
            o->b04++;
            D_8009C958 = 1;
            break;
        }
        func_8002052C(o, 0x50);
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
