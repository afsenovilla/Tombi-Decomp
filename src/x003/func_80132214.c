// FUNC 80132214 1868 X003
// MATCHING 80132214 1868
#include "TOBJ.H"
typedef struct { char c[12]; } B12;

extern TObj *D_8009F2D8, *D_8009F2DC;
extern Fix16 *D_800A6078;
extern unsigned short D_800A6066;
extern short D_800A60EA;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern TObj *FUN_8002dcc8(int, int, void *);
extern void func_801289C8(TObj *, int, int);
extern void FUN_8005a8a8(int, int, int);
extern int Rand(void);

#define SPAWN(x, k) { v = *(B12 *)&(x)->a; (x)->d90 = (int)FUN_8002dcc8(2, k, &v); }
#define DONE(x) (((TObj *)(x)->d90)->b04 == 2)
#define KILL(x) ((TObj *)(x)->d90)->b04 = 3

void func_80132214(TObj *o)
{
    TObj *a = D_8009F2D8;
    TObj *b = D_8009F2DC;
    B12 v;

    switch (o->state) {
    case 0:
        SPAWN(b, 4);
        func_801289C8(a, 0, 0);
        o->state++;
        break;
    case 1:
        if (DONE(b)) {
            KILL(b);
            o->w08 = 4;
            o->state++;
        }
        break;
    case 2:
        if (--o->w08 <= 0) {
            SPAWN(a, 5);
            func_801289C8(a, 3, 0);
            func_801289C8(b, 0, 0);
            o->state++;
        }
        break;
    case 3:
        if (DONE(a)) {
            KILL(a);
            SPAWN(b, 6);
            func_801289C8(a, 0, 0);
            func_801289C8(b, 3, 0);
            o->state++;
        }
        break;
    case 4:
        if (DONE(b)) {
            KILL(b);
            SPAWN(a, 7);
            func_801289C8(a, 3, 0);
            func_801289C8(b, 0, 0);
            o->state++;
        }
        break;
    case 5:
        if (DONE(a)) {
            KILL(a);
            SPAWN(b, 8);
            func_801289C8(a, 0, 0);
            func_801289C8(b, 3, 0);
            o->state++;
        }
        break;
    case 6:
        if (DONE(b)) {
            KILL(b);
            SPAWN(a, 9);
            func_801289C8(a, 3, 0);
            func_801289C8(b, 0, 0);
            o->state++;
        }
        break;
    case 7:
        if (DONE(a)) {
            KILL(a);
            o->w08 = 200;
            FUN_8005a8a8(0x23, 0, 0);
            o->state = 0x10;
        }
        break;
    case 8:
        a->h->p.whole = a->d30 + (o->w08 & 1);
        a->y.p.whole = a->d34 + (Rand() & 1);
        if (--o->w08 <= 0) {
            SPAWN(a, 0xa);
            func_801289C8(a, 3, 0);
            func_801289C8(b, 0, 0);
            o->state++;
        }
        break;
    case 9:
        if (DONE(a)) {
            KILL(a);
            a->animFrame = 1;
            a->step = 2;
            a->state = 0;
            o->state++;
        }
        break;
    case 10:
        if (!a->visible) {
            SPAWN(b, 0xb);
            func_801289C8(b, 3, 0);
            a->b04 = 3;
            o->state++;
        }
        break;
    case 11:
        if (DONE(b)) {
            KILL(b);
            b->step = 2;
            b->state = 0;
            o->state++;
        }
        break;
    case 12:
        if (b->h->p.whole < 0x1d0) {
            b->h->p.whole = 0x1d0;
            b->step = 1;
            b->state = 0;
            o->w08 = 0x3c;
            o->state++;
        }
        break;
    case 13:
        if (--o->w08 <= 0) {
            SPAWN(b, 0xc);
            func_801289C8(b, 3, 0);
            o->state++;
        }
        break;
    case 14:
        if (DONE(b)) {
            KILL(b);
            b->step = 2;
            b->state = 0;
            o->state++;
        }
        break;
    case 15:
        D_800A6066 = b->h->p.whole < D_800A6078->p.whole;
        if (!b->visible) {
            D_800A603C = 1;
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_8009C93E = 0;
            D_800A60EA = 0;
            D_800A603D = 0;
            D_800A603E = 0;
            b->b04 = 3;
            o->b04 = 3;
        }
        break;
    case 16:
        if (--o->w08 <= 0) {
            o->w08 = 0x3c;
            func_801289C8(a, 0, 0);
            a->d30 = a->h->p.whole;
            a->d34 = a->y.p.whole;
            o->state = 8;
        }
        break;
    }
}
