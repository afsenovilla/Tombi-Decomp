// FUNC 80124b78 1156 X003
// MATCHING 80124b78 1156
#include "TOBJ.H"
typedef void (*Fn)(TObj *);
#define WD2(o) (*(short *)((char *)(o) + 0xd2))
typedef struct { unsigned char b0, b1; } X;
extern int D_1F8002D0;
extern unsigned char D_8009C942;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_800A4582;
extern Fn D_80135C64[];
extern int FUN_800203dc(TObj *);
extern int FUN_800202b4(TObj *);
extern int FUN_8001f9e0(void);
extern void FUN_80018790(TObj *);
extern void func_801226A0(TObj *);
extern void func_80121FD0(TObj *);
extern void func_80121A88(TObj *);
extern void func_80121398(TObj *);
extern void func_801210A8(TObj *);
extern void func_801209F8(TObj *);
extern void func_8012338C(TObj *);
extern void func_80122DB4(TObj *);
extern void func_80122C44(TObj *);

void func_80124B78(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->box0 = 10;
            o->box1 = 0x14;
            o->box2 = 0x10;
            o->box3 = 0x20;
            o->animFrame &= 1;
            if (o->animFrame)
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
            x->b1 = 0;
            o->anim = 0;
            o->d8c = 0;
            o->w98 = 3;
            o->step++;
            break;
        case 1:
            if (FUN_800203dc(o)) {
                o->step = 0;
                o->b04++;
            }
            break;
        }
        break;
    case 1:
        if (D_8009C942 != 0 && o->subtype != 4) {
            FUN_800202b4(o);
            break;
        }
        if (o->b9e == 0) {
            switch (o->step) {
            case 0:
                if (FUN_800202b4(o) == 0)
                    break;
                D_80135C64[o->subtype](o);
                goto chk;
            case 1:
                if (FUN_800202b4(o) == 0)
                    break;
                switch (o->state) {
                case 0:
                    func_801226A0(o);
                    break;
                case 1:
                case 2:
                    func_80121FD0(o);
                    break;
                case 3:
                    func_80121A88(o);
                    break;
                case 4:
                    func_80121398(o);
                    break;
                case 5:
                case 6:
                    func_801210A8(o);
                    break;
                case 7:
                    func_801209F8(o);
                    break;
                case 8:
                case 9:
                    break;
                }
            chk:
                if (o->w98 == 0) {
                    o->active = 2;
                    o->b04 = 2;
                    o->step = 2;
                    o->state = 0;
                }
                break;
            }
            if (((D_1F8001F8 + D_1F800198) & 0xf) == 0 && D_800A4582 + 0xa0 < o->y.p.whole)
                o->b04 = 3;
        } else {
            FUN_800202b4(o);
            if (o->b9f == 0) {
                WD2(o) = o->h->p.whole;
                o->b9f = 0xf;
            } else {
                o->h->p.whole = (short)(WD2(o) - 2) + (FUN_8001f9e0() & 3);
                if (--o->b9f == 0) {
                    o->b9e = 0;
                    o->h->p.whole = WD2(o);
                }
            }
        }
        o->b9d = 0;
        break;
    case 2:
        switch (o->step) {
        case 0:
            if (D_8009C942 != 0) {
                FUN_800202b4(o);
                break;
            }
            func_8012338C(o);
            FUN_800202b4(o);
            if (o->w98 == 0) {
                o->active = 2;
                o->b04 = 2;
                o->step = 2;
                o->state = 0;
            }
            if (((D_1F8001F8 + D_1F800198) & 0xf) == 0 && D_800A4582 + 0xa0 < o->y.p.whole)
                o->b04 = 3;
            break;
        case 1:
            func_80122DB4(o);
            FUN_800202b4(o);
            break;
        case 2:
            func_80122C44(o);
            if (FUN_800202b4(o) == 0)
                o->b04 = 3;
            break;
        }
        break;
    case 3:
        if (o->subtype >= 2) {
            (*(short *)o->d90)--;
            FUN_80018790(o);
        } else {
            FUN_80018790(o);
        }
        break;
    }
}
