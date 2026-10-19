// FUNC 800e8644 604 I0NFO
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE6C(TObj *o);
extern int GetGraphType(void);
extern void func_800EDFB8(TObj *o, int a);
extern void func_800EE07C(TObj *o);
extern void *D_800106B4;
extern char D_80011790[];
typedef struct { unsigned short a, b; } Pair7A;
extern Pair7A D_8007A024[];
extern TObj *D_8009C330;
extern unsigned char D_8009C970;
extern unsigned short D_8009C982[];
extern unsigned char D_8009C990[];
extern unsigned char D_8009CEAB;
extern unsigned char D_8009CF06;
extern unsigned char D_8009D006;
extern signed char D_8009D2B0;
extern unsigned char D_8009D2B3[];
extern unsigned short D_800A6040[];
extern unsigned char D_800A60A3;
extern unsigned char D_800A60FA[];
extern unsigned short D_800A60FC[];

void func_800E8644(TObj *o)
{
    TObj *p;
    int pad[2];
    D_8009C330 = (TObj *)((char *)o + 0xF8);
    if (o->step == 0) {
        o->anim = D_80011790;
        func_8001FE6C(o);
        p = D_8009C330;
        if (GetGraphType() != 1) {
            GetGraphType();
        }
        S16(p, 0x14) = 0;
        o->w1e = 0;
        D_800A60FA[0] = D_8009D2B3[0];
        D_800A6040[0] = D_8007A024[D_8009C990[0]].a;
        D_800A60FC[0] = D_8007A024[D_8009D2B3[0]].b;
        U16(D_8009C330, 0x28) = 0xFFFF;
        U16(D_8009C330, 0x2A) = 0xFFFF;
        D_8009C330->animFrame = 0xFFFF;
        D_8009C330->active = 0;
        o->visible = 1;
        o->b0a = 1;
        o->ba5 = 0;
        o->wb0 = 0;
        o->velH = 0;
        o->velV = 0;
        o->wb2 = 0;
        o->b6b = 1;
        o->b9d = 0;
        if (D_8009C982[0] != 0 && D_8009C982[0] != 2) {
            o->animFrame = 4;
        }
        if (D_8009CF06 != 0) {
            o->subtype = 1;
        }
        if (D_8009D006 != 0) {
            o->subtype = 2;
        }
        S16(o, 0x9A) = D_8009C970;
        o->anim = D_800106B4;
        *(signed char *)&o->b0f = -8;
        o->w98 = D_8009C970;
        o->d3c = *(int *)0x1F8002F0;
        func_8001FE6C(o);
        func_800EDFB8(o, 1);
        if (D_8009CEAB == 3) {
            func_800EE07C(o);
        }
        D_800A60A3 = 0;
        o->b04 = 1;
        o->step = 0x63;
        o->state = 0;
        switch (D_8009D2B0) {
        case 4:
            o->h->p.whole += 8;
            o->y.p.whole += 8;
            break;
        case 3:
            o->h->p.whole = -0x49;
            o->animFrame = 6;
            o->b04 = 4;
            o->step = 5;
            o->state = 0;
            o->substep = 0;
            break;
        }
    }
}
