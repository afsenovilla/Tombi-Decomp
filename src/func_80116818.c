// FUNC 80116818 1276 X000
// MATCHING 80116818 1276
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char pad[0x12]; short w12; char p14[2]; short w16; char p18[0x2e - 0x18]; short w2e; } G6038;
extern G6038 D_800A6038;
extern unsigned short D_8009C962;
extern unsigned char D_8009C938, D_8009C966, D_8009D095, D_8009C933, D_8009C93F, D_8009C942;
extern unsigned char D_8009D2B0, D_8009C930, D_8009C93E, D_8009C975[], D_8009C93C[];
extern int D_8009C984;
extern short D_1F8001C6;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern void FUN_80029cb4(TObj *);
extern void FUN_80029078(TObj *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern unsigned char *FUN_800186f8(void);
extern void FUN_8001eacc(void);
extern void FUN_8001f110(int);
extern void FUN_8001d63c(int);
extern void FUN_8001f4bc(void);
extern void FUN_8001eb64(void);
extern void FUN_8001d6a4();
extern void FUN_800171b8(int, void *);

static __inline__ void slide(TObj *o)
{
    int v;
    int c;
    int *p;

    *(int *)o->d34 += o->y.raw;
    if (o->y.raw > 0) {
        p = (int *)o->d34;
        v = (short)o->animFrame;
        c = v < ((short *)p)[1];
    } else {
        p = (int *)o->d34;
        v = (short)o->animTimer;
        c = ((short *)p)[1] < v;
    }
    if (c) {
        *p = v << 16;
        o->y.raw = 0;
    }
}

void func_80116818(TObj *o)
{
    G6038 *g = &D_800A6038;
    unsigned char *p;

    switch (o->state) {
    case 0:
        FUN_80029cb4(o);
        if (D_8009C962 != 0) break;
        if ((short)o->animFrame <= g->w12) g->w12 = o->animFrame;
        if (D_8009C938 != 0) break;
        if (g->w12 < 0x49e) break;
        if (g->w16 >= -0xe7) break;
        D_8009C966 = 1;
        o->state++;
        FUN_8005a8a8(2, 0, 1);
        if (D_8009D095 != 0) break;
        p = FUN_800186f8();
        if (p == 0) break;
        p[0] = 1;
        p[2] = 0xd;
        p[3] = 1;
        if (D_8009C933 == 0) {
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_8009D2B0 = 0;
            p[1] = 0;
        } else {
            p[1] = 1;
        }
        D_8009D095 = 1;
        break;
    case 1:
        FUN_80029cb4(o);
        if ((short)o->animFrame <= g->w12) g->w12 = o->animFrame;
        if (D_8009C938 != 0) break;
        if ((D_8009C984 & 1) && D_8009C930 == 3) {
            g->w2e = 0;
            o->animFrame = 0x52e;
            o->y.raw = 0x10000;
            o->w48 = 0;
            D_8009C93F = 1;
            D_8009C942 = 1;
            FUN_8001eacc();
            FUN_8001f110(1);
            FUN_8001d63c(2);
            o->state++;
        } else if (g->w12 < 0x49e) {
            D_8009C966 = 2;
            o->state--;
        }
        break;
    case 2:
        if (o->w48 != 0) {
            o->w48--;
            break;
        }
        slide(o);
        FUN_80029078(o);
        if (o->y.raw != 0) break;
        if (o->visible == 0) {
            D_8009C930 = 4;
            o->state = 4;
            o->animFrame = 0x5ae;
            o->y.raw = 0x20000;
            o->w48 = 0x78;
        } else {
            D_8009C93E = 0;
            D_8009C93F = 0;
            D_8009C942 = 0;
            o->animTimer = 0xa0;
            o->animFrame = U16(o, 0x42);
            o->state++;
        }
        break;
    case 3:
        o->subtype = 0;
        o->state = 0;
        break;
    case 4:
        if (o->w48 != 0) {
            if (--o->w48 < 80) slide(o);
            if (o->w48 == 60) {
                D_8009C975[0] = 3;
                D_8009C93C[0] = 1;
            }
        } else {
            D_8009C966 = 3;
            D_8009C930 = 0;
            D_1F8001C6 = 2;
            FUN_8001f4bc();
            o->state++;
            D_1F8001CC = 1;
            D_1F8001CD = 2;
            FUN_800171b8(1, FUN_8001d6a4);
        }
        break;
    case 5:
        if (D_1F8001CC == 0) {
            o->animFrame = 0x52e;
            D_1F8001C6 = 0;
            *(int *)o->d34 = 0x52e0000;
            o->y.raw = 0;
            D_8009C975[0] = 4;
            D_8009C93C[0] = 0;
            o->state = 2;
            o->animTimer = 0x4c0;
            o->y.raw = -0x10000;
            o->visible = 1;
            FUN_8001eb64();
            FUN_8005a9a4(2, 0);
        }
        break;
    }
}
