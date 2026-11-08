// VERSION SLPS_01144 info2 800F9450 1720
// JP version of PAL i0nfo2 800f80d8 (npcSet07SceneStep2.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char c[12]; } B12;
typedef struct { unsigned char v; } U8S;
extern U8S D_800A7B7C;
extern U8S D_800A7B7D;
extern U8S D_800A7B7E;
extern U8S D_800A7C24;
extern TObj *D_800A0E70;
extern U8S D_8009E4E6;
extern U8S D_8009E4E7;
extern U8S D_8009E4EA;
extern unsigned char D_8009E97B;
extern U8S D_8009EA93;
extern unsigned char D_8009EC83;
extern unsigned char D_8009EE6B;
extern void topdownActorSetAnim(TObj *o, int a, int b);
extern TObj *func_8002E42C(int, int, B12 *);
extern void func_8001C78C(int, int, int);
extern void func_8001C988(int, int);
extern void func_80027A5C(int, int, int);
extern void func_80027C18(int, int);
extern void func_80020E38(TObj *o);

#define VX(v) (*(short *)&(v).c[2])
#define VY(v) (*(short *)&(v).c[6])

void func_800F9450(TObj *o)
{
    B12 v;
    TObj *t = D_800A0E70;
    unsigned char k;

    switch (o->state) {
    case 0:
        t->step = 2;
        t->state = 0;
        D_800A7B7D.v = 2;
        D_800A7B7E.v = 0;
        D_8009E4E7.v = 1;
        D_8009E4EA.v = 1;
        o->w08 = 0x3C;
        o->state++;
        if (D_8009E97B == 0xFF) {
            v = *(B12 *)&t->a;
            VX(v) += 0x10;
            t->animFrame = t->b69 - 1;
            t->d90 = (int)func_8002E42C(2, 0x30, &v);
            topdownActorSetAnim(t, 0xB, 0);
            o->state = 0xF;
        } else if (D_8009E97B == 1) {
            t->animFrame = t->b69 - 1;
            o->state = 5;
        } else if (D_8009EE6B & 0x10) {
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            S16(t, 0x2E) = 3;
            t->d90 = (int)func_8002E42C(2, 0x2C, &v);
            topdownActorSetAnim(t, 0xB, 0);
            o->state = 4;
        } else {
            topdownActorSetAnim(t, 0x10, 0);
        }
        break;
    case 1:
        func_80020E38(o);
        if (--o->w08 <= 0) {
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            t->d90 = (int)func_8002E42C(2, 0x2A, &v);
            topdownActorSetAnim(t, 0xB, 0);
            o->state++;
        }
        break;
    case 2:
        func_80020E38(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            S16(t, 0x2E) = 3;
            t->d90 = (int)func_8002E42C(2, 0x2B, &v);
            topdownActorSetAnim(t, 0xB, 0);
            o->state++;
        }
        break;
    case 3:
        func_80020E38(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            t->d90 = (int)func_8002E42C(2, 0x2C, &v);
            S16(t, 0x2E) = 3;
            topdownActorSetAnim(t, 0xB, 0);
            o->state++;
        }
        break;
    case 4:
        func_80020E38(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            func_8001C78C(0x2F, 0, 0);
            D_8009E97B = 1;
            o->state = 9;
        }
        break;
    case 5:
        v = *(B12 *)&t->a;
        VX(v) -= 0x50;
        VY(v) += 0xA0;
        t->d90 = (int)func_8002E42C(2, 0x2D, &v);
        D_8009E4E7.v = 1;
        D_8009E4EA.v = 1;
        k = t->b69;
        t->animFrame = k - 1;
        topdownActorSetAnim(t, k + 7, 0);
        o->state++;
        break;
    case 6:
        func_80020E38(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            v = *(B12 *)&t->a;
            VX(v) -= 0x50;
            VY(v) += 0xA0;
            if (D_8009EC83 >= 10) {
                if (D_8009EC83 >= 15) {
                    t->d90 = (int)func_8002E42C(2, 0x2F, &v);
                    o->state = 8;
                } else {
                    t->d90 = (int)func_8002E42C(2, 0x2E, &v);
                    o->state = 7;
                }
            } else {
                o->state = 9;
            }
        }
        break;
    case 7:
        func_80020E38(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            func_80027A5C(0x1F, 1, 1);
            func_80027A5C(0x1F, 1, 1);
            func_80027C18(0x37, D_8009EC83);
            func_8001C988(0x2F, 0);
            o->state = 9;
        }
        break;
    case 8:
        func_80020E38(o);
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            func_80027A5C(0x5E, 1, 1);
            func_80027C18(0x37, D_8009EC83);
            func_8001C988(0x2F, 0);
            o->state = 9;
        }
        break;
    case 9:
        t->b68 = 0;
        topdownActorSetAnim(t, 3, 0);
        t->step = 0;
        t->state = 0;
        D_8009E4E7.v = 0;
        D_8009E4E6.v = 0;
        D_8009E4EA.v = 0;
        D_8009EA93.v = 0;
        D_800A7C24.v = 0;
        D_800A7B7C.v = 1;
        D_800A7B7D.v = 0;
        D_800A7B7E.v = 0;
        o->step = 0;
        o->state = 0;
        break;
    case 15:
        if (((TObj *)t->d90)->b04 == 2) {
            ((TObj *)t->d90)->b04 = 3;
            o->state = 9;
        }
        break;
    }
}
#endif
