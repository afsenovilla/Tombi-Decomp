// FUNC 80118ea0 2208 X017
// MATCHING 80118ea0 2208
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
extern TObj *D_8009F2D8;
extern TObj *D_8009F2DC;
extern TObj D_800A6038;
extern unsigned char D_8009CE3A, D_8009C93A, D_8009C93E, D_8009C93F, D_8009C942;
extern unsigned char D_8009C975, D_8009CDF1;
extern unsigned char *D_8009C330;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern short D_1F8001C6;
extern char D_80010814[];
extern void FUN_8001d6a4();
extern void FUN_8001fe6c(TObj *);
extern void FUN_800eea7c(TObj *, int, int);
extern void FUN_8001d63c(int);
extern void FUN_8001f110(int);
extern void FUN_800171b8(int, void *);
extern void FUN_8001eb64(void);
extern void FUN_8005a9a4(int, int);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_80026c50(int, int, int);
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void func_801180BC(TObj *, unsigned short, short);

void func_80118EA0(TObj *o)
{
    TObj *s1 = D_8009F2D8;
    TObj *s2 = D_8009F2DC;
    TObj *p;
    B12 v;

    switch (o->state) {
    case 0:
        if (D_8009CE3A != 0) break;
        if (D_8009C93A == 0) break;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_800A6038.b04 = 5;
        D_800A6038.step = 0x64;
        D_800A6038.state = 0;
        D_800A6038.anim = D_80010814;
        FUN_8001fe6c(&D_800A6038);
        o->state++;
        break;
    case 1:
        D_800A6038.h->raw -= 0x12800;
        if (D_800A6038.h->p.whole < 0x4a) {
            FUN_800eea7c(&D_800A6038, 0, 0);
            o->w08 = 0x14;
            o->state++;
        }
        break;
    case 2:
        if (--o->w08 > 0) break;
        D_800A6038.animFrame = 1;
        D_8009C330[9] = 5;
        D_800A6038.b9c = 1;
        D_800A6038.b04 = 6;
        D_800A6038.b69 = 0;
        D_800A6038.wb2 = 0;
        D_800A6038.velH = 0;
        D_800A6038.velV = 0;
        D_800A6038.velX = 0;
        D_800A6038.velY = 0;
        D_800A6038.step = 4;
        D_800A6038.state = 0;
        o->state++;
        break;
    case 3:
        if (D_800A6038.b69 == 0) break;
        D_800A6038.b04 = 5;
        D_800A6038.step = 0x64;
        D_800A6038.state = 0;
        FUN_800eea7c(&D_800A6038, 0, 0);
        o->w08 = 0x1e;
        o->state++;
        break;
    case 4:
        if (--o->w08 > 0) break;
        D_800A6038.animFrame = 0;
        D_800A6038.anim = D_80010814;
        FUN_8001fe6c(&D_800A6038);
        D_800A6038.b04 = 5;
        D_800A6038.step = 0x64;
        D_800A6038.state = 0;
        o->state++;
        break;
    case 5:
        D_800A6038.h->raw += 0x12800;
        if (D_800A6038.h->p.whole >= 0x79) {
            FUN_800eea7c(&D_800A6038, 0, 0);
            o->w08 = 0x1e;
            o->state++;
        }
        break;
    case 6:
        if (--o->w08 > 0) break;
        o->state = 10;
        break;
    case 10:
        FUN_8001d63c(0xb);
        D_800A6038.b04 = 5;
        D_800A6038.step = 0x62;
        D_8009C93F = 1;
        D_8009C975 = 3;
        o->state++;
        break;
    case 11:
        if (D_8009C975 != 1) break;
        D_1F8001CC = 1;
        D_1F8001CD = 0xc;
        D_1F8001C6 = 2;
        FUN_8001f110(0);
        FUN_800171b8(1, FUN_8001d6a4);
        o->state++;
        break;
    case 12:
        D_8009C975 = 4;
        FUN_8001eb64();
        D_8009CDF1 = 0xff;
        o->state++;
    case 13:
        if (D_8009C975 != 0) break;
        o->w08 = 0x12c;
        FUN_8005a9a4(0x49, 0);
        o->state++;
        break;
    case 14:
        if (--o->w08 > 0) break;
        o->w08 = 1;
        o->state = 0xf;
        break;
    case 15:
        if (--o->w08 > 0) break;
        func_801180BC(s1, 1, 0);
        func_801180BC(s2, 1, 0);
        s1->step = 0;
        s2->step = 0;
        {
            short t = s1->h->p.whole;
            o->state++;
            o->w08 = t - 0xc0;
        }
        break;
    case 16:
        s1->h->p.whole--;
        s2->h->p.whole--;
        if (--o->w08 > 0) break;
        func_801180BC(s1, 0, 0);
        func_801180BC(s2, 0, 0);
        o->w08 = 0x50;
        o->state++;
        break;
    case 17:
        if (--o->w08 > 0) break;
        func_801180BC(s1, 1, 0);
        o->w08 = 0x10;
        o->state++;
        break;
    case 18:
        s1->h->p.whole--;
        if (--o->w08 > 0) break;
        func_801180BC(s1, 0, 0);
        o->w08 = 0x1e;
        o->state++;
        break;
    case 19:
        if (--o->w08 > 0) break;
        o->state++;
        break;
    case 20:
        v = *(B12 *)&s1->a;
        s1->d90 = (int)FUN_8002dcc8(2, 0x20, &v);
        func_801180BC(s1, 2, 0);
        o->state++;
        break;
    case 21:
        {
            TObj *q = (TObj *)s1->d90;
            if (q->b04 != 2) break;
            q->b04 = 3;
        }
        v = *(B12 *)&s1->a;
        s1->d90 = (int)FUN_8002dcc8(2, 0x23, &v);
        o->state++;
        break;
    case 22:
        p = (TObj *)s1->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_8005a8a8(0x76, 0, 0);
        func_801180BC(s1, 0, 0);
        o->w08 = 0x12c;
        o->state++;
        break;
    case 23:
        if (--o->w08 > 0) break;
        v = *(B12 *)&s2->a;
        s2->d90 = (int)FUN_8002dcc8(2, 0x26, &v);
        func_801180BC(s2, 2, 0);
        o->state++;
        break;
    case 24:
        p = (TObj *)s2->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_80026c50(0x9b, 1, 1);
        func_801180BC(s2, 0, 0);
        o->w08 = 0x78;
        o->state++;
        break;
    case 25:
        if (--o->w08 > 0) break;
        v = *(B12 *)&s2->a;
        s2->d90 = (int)FUN_8002dcc8(2, 0x27, &v);
        func_801180BC(s2, 2, 0);
        o->state++;
        break;
    case 26:
        p = (TObj *)s2->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_8005a8a8(0x96, 0, 0);
        func_801180BC(s2, 0, 0);
        o->w08 = 0x12c;
        o->state = 0x63;
        break;
    case 99:
        if (--o->w08 > 0) break;
        D_1F8001C6 = 0;
        D_8009C93E = 0;
        D_8009C93F = 0;
        D_8009C942 = 0;
        s1->step = 1;
        s2->step = 1;
        D_800A6038.b04 = 1;
        D_800A6038.animFrame = 0;
        D_800A6038.wb2 = 0;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
        o->b04 = 3;
        break;
    }
}
