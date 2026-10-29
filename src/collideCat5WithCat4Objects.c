// FUNC 80049524 824 MAIN0
// MATCHING 80049524 824
#if defined(VERSION_PCPX_96084) /* PrePre 8 scratchpad layout */
#define D_1F80019E D_1F8002A6
#endif
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"

extern short D_1F80019E;
void pushObjectOutOfObjectFacing(TObj *o, TObj *e);
void pushObjectOutOfCat4Type01(TObj *o, TObj *e);
void pushObjectOutOfCat4Type02(TObj *o, TObj *e);
void pushCat5ObjectOutOfCat4Type04(TObj *o, TObj *e);
void func_8012038C(TObj *o, TObj *e);
void pushObjectOutOfCat4Type06(TObj *o, TObj *e);
void func_801280DC(TObj *o, TObj *e);
void func_80127FDC(TObj *o, TObj *e);
void func_801216A0(TObj *o, TObj *e);
void func_8011FAE8(TObj *o, TObj *e);
void func_8011FA74(TObj *o, TObj *e);
void func_801203D0(TObj *o, TObj *e);
void func_8012788C(TObj *o, TObj *e);
void func_801210B4(TObj *o, TObj *e);
void func_80121A40(TObj *o, TObj *e);
void func_80121994(TObj *o, TObj *e);

static __inline__ short hit(TObj *o, TObj *p)
{
    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    if ((unsigned short)((o->h->p.whole - p->h->p.whole) + (p->box0 + (o->box1 - o->box0))) > p->box1 + o->box1)
        return 0;
    if ((unsigned short)((o->y.p.whole - p->y.p.whole) + (p->box2 + (o->box3 - o->box2))) > o->box3 + p->box3)
        return 0;
    return 1;
}

void collideCat5WithCat4Objects(void)
{
    short n;
    TObj **pp, **qq;
    TObj *o, *e;

    n = SPAD.s248;
    pp = (*(TObj ***)&SPAD.p228);
    if (SPAD.s24c == 0)
        return;
    while (n != 0) {
        o = *pp++;
        n--;
        if (!(o->active & 1))
            continue;
        qq = (*(TObj ***)&SPAD.p224);
        for (D_1F80019E = SPAD.s24c; D_1F80019E != 0; ) {
            e = *qq++;
            D_1F80019E--;
            if (!(e->active & 1))
                continue;
            switch (e->type) {
            case 0: pushObjectOutOfObjectFacing(o, e); break;
            case 1: pushObjectOutOfCat4Type01(o, e); break;
            case 2: pushObjectOutOfCat4Type02(o, e); break;
            case 4:
                switch (GAME.selectedArea) {
                case 0: pushCat5ObjectOutOfCat4Type04(o, e); break;
                case 3: func_8012038C(o, e); break;
                }
                break;
            case 6: pushObjectOutOfCat4Type06(o, e); break;
            case 0xf: func_801280DC(o, e); break;
            case 0x14: func_80127FDC(o, e); break;
            case 0x1c: func_801216A0(o, e); break;
            case 0x23: func_8012788C(o, e); break;
            case 0x28: func_801210B4(o, e); break;
            case 0x1f: func_8011FA74(o, e); break;
            case 0x22: func_801203D0(o, e); break;
            case 0x1b:
                if (hit(o, e))
                    e->b68 = 1;
                break;
            case 0x34: func_80121994(o, e); break;
            case 0x33: func_80121A40(o, e); break;
            case 0x1e: func_8011FAE8(o, e); break;
            }
        }
    }
}
