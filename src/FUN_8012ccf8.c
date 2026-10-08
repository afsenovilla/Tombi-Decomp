// FUNC 8012ccf8 1436 X000
// MATCHING 8012ccf8 1436
#include "TOBJ.H"
#include "raw7.h"

typedef struct { unsigned char b0, b1; } B8012CCF8;

extern int D_1F8002D0;
extern int D_1F800190;
extern int D_1F80018C;
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942[];
extern unsigned char D_800A4553[];
extern int D_800A4568[];
extern void (*D_801390F8[])(TObj *);
extern int func_800203DC(TObj *);
extern int ObjCullRegister(TObj *);
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern void FUN_80129fb4(TObj *);
extern void func_80129BB0(TObj *);
extern void FUN_801294fc(TObj *);
extern void FUN_80128fac(TObj *);
extern void FUN_801288b8(TObj *);
extern void FUN_80128190(TObj *);
extern void FUN_80127eb0(TObj *);
extern void FUN_80127820(TObj *);
extern void FUN_8012ae1c(TObj *);
extern void func_8012A820(TObj *);
extern void FUN_8012a6a4(TObj *);
extern void FUN_8002ee50(char, int, int, int);
extern TObj *FUN_800183b8(void);
extern void freeObjectLayer2(TObj *);

void FUN_8012ccf8(TObj *o)
{
    B8012CCF8 *p = (B8012CCF8 *)&o->wb4;
    TObj *n;
    int t;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->box0 = 10;
            o->box1 = 0x14;
            t = o->animFrame;
            o->box2 = 0x10;
            o->box3 = 0x20;
            o->animFrame = t & 1;
            if (t & 1)
                o->wb8 = -0x10;
            else
                o->wb8 = 0x10;
            o->d3c = D_1F8002D0;
            o->w1e = 1;
            o->b0a = 2;
            o->b0d = 0;
            o->b69 = 0;
            o->b68 = 0;
            *(signed char *)&o->b0f = -9;
            p->b1 = 0;
            o->d8c = 0;
            o->w98 = 3;
            o->anim = 0;
            o->step++;
            break;
        case 1:
            if (func_800203DC(o)) {
                o->step = 0;
                o->b04++;
            }
            break;
        }
        break;
    case 1:
        if (o->step == 1 && D_8009C942[0]) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        if (o->b9e == 0) {
            switch (o->step) {
            case 0:
                if (o->state != 0 && D_8009C942[0]) {
                    ObjCullRegister(o);
                    break;
                }
                if (o->w98 == 0) {
                    o->active = 2;
                    o->b04 = 2;
                    o->step = 2;
                    o->state = 0;
                    break;
                }
                if (o->visible == 0)
                    break;
                D_801390F8[o->subtype](o);
                break;
            case 1:
                if (o->visible == 0)
                    break;
                switch (o->state) {
                case 0:
                    FUN_80129fb4(o);
                    break;
                case 1:
                    func_80129BB0(o);
                    break;
                case 2:
                    FUN_801294fc(o);
                    break;
                case 3:
                    FUN_80128fac(o);
                    break;
                case 4:
                    FUN_801288b8(o);
                    break;
                case 5:
                    FUN_80128190(o);
                    break;
                case 6:
                    FUN_80127eb0(o);
                    break;
                case 7:
                    FUN_80127820(o);
                    break;
                case 8:
                case 9:
                    break;
                }
                if (o->w98 == 0) {
                    o->active = 2;
                    o->b04 = 2;
                    o->step = 2;
                    o->state = 0;
                }
                break;
            }
        } else if (o->b9f == 0) {
            U16(o, 0xd2) = o->h->p.whole;
            o->b9f = 0xf;
        } else {
            o->h->p.whole = (short)(S16(o, 0xd2) - 2) + (Rand() & 3);
            if (--o->b9f == 0) {
                o->b9e = 0;
                o->h->p.whole = U16(o, 0xd2);
            }
        }
        o->b9d = 0;
        break;
    case 2:
        switch (o->step) {
        case 0:
            if (D_8009C942[0]) {
                ObjCullRegister(o);
                break;
            }
            FUN_8012ae1c(o);
            ObjCullRegister(o);
            if (o->w98 == 0) {
                o->active = 2;
                o->b04 = 2;
                o->step = 2;
                o->state = 0;
            }
            break;
        case 1:
            func_8012A820(o);
            ObjCullRegister(o);
            break;
        case 2:
            FUN_8012a6a4(o);
            if (ObjCullRegister(o) == 0)
                o->b04 = 3;
            break;
        case 3:
            ObjCullRegister(o);
            switch (o->state) {
            case 0:
                D_8009C93F[0] = 1;
                D_8009C942[0] = 1;
                D_800A4553[0] = 3;
                D_800A4568[0] = 0;
                D_1F80018C = o->h->raw;
                D_1F800190 = o->y.raw;
                FUN_8002ee50(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                FUN_8002ee50(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                n = FUN_800183b8();
                if (n) {
                    n->active = 1;
                    n->type = 0x2b;
                    n->b6b = o->b6b;
                    n->b1d = o->b1d;
                    n->a.p.whole = o->a.p.whole;
                    n->y.p.whole = o->y.p.whole;
                    n->b.p.whole = o->b.p.whole;
                }
                o->timer = 10;
                o->state++;
            case 1:
                AnimAdvance(o);
                if (--o->timer == -1)
                    o->b04 = 3;
                break;
            }
            break;
        }
        break;
    case 3:
        if (o->subtype == 5)
            (*(short *)o->d94)--;
        freeObjectLayer2(o);
        break;
    }
}
