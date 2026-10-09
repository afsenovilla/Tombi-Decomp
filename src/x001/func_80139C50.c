// FUNC 80139c50 1148 X001
// MATCHING 80139c50 1148
#include "TOBJ.H"
extern TObj D_800A6038;
#define P D_800A6038
extern unsigned short D_800A6066;
extern TObj *D_800A60C8;
extern TObj *D_8009C330;
extern unsigned char D_8009C93F, D_8009C93E, D_8009C942, D_8009CEBC, D_800A60F8, D_8009CDB2;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern int D_8009C984;

/* halfword view of o->anim (+0x24): an in-struct store, so it does not depend on the raw D_8009C330 store */
typedef struct { char pad[0x24]; unsigned short a; } AnimH;
extern char D_80010748[];
extern void AnimLoadDuration(TObj *);
extern TObj *FUN_8002dc50(int, int, int, int);
extern void FUN_8001fce4(TObj *);
extern void FUN_8004d620(int, int);

void func_80139C50(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;
    unsigned char *f;
    int n;

    switch (o->step) {
    case 0:
        D_8009C93F = 1;
        D_8009C93E = 1;
        D_8009C942 = 1;
        P.anim = D_80010748;
        AnimLoadDuration(&P);
        e->step = 1;
        e->state = 0;
        switch (D_8009CEBC) {
        case 0:
            D_800A60C8 = FUN_8002dc50(2, 10, 0x80, 0x6c);
            o->step++;
            break;
        case 1:
            D_800A60C8 = FUN_8002dc50(2, 11, 0x80, 0x6c);
            o->step++;
            break;
        }
        break;
    case 1:
        p = D_800A60C8;
        if (p->b04 != 2) break;
        f = &D_8009CEBC;
        switch (*f) {
        case 0:
            p->b04 = 3;
            *f = 1;
            o->step = 0;
            o->state = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            e->step = 0;
            e->state = 1;
            e->animFrame = e->b6b;
            ((AnimH *)o)->a = 0;
            *(unsigned short *)((char *)D_8009C330 + 0x2e) = 0xffff;
            D_8009C93E = 0;
            D_8009C942 = 0;
            D_800A60F8 = 0;
            D_8009C93F = 0;
            break;
        case 1:
            p->b04 = 3;
            D_800A6066 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            o->step = 2;
            o->state = 0;
            D_8009C330->animFrame = 0xffff;
            *f = 2;
            *(unsigned short *)&o->anim = 1;
            D_800A60F8 = 0;
            break;
        }
        break;
    case 2:
        switch (o->state) {
        case 0:
            o->w08 = 0x20;
            e->d8c = 0;
            e->b69 = 0;
            e->velY = -0x300;
            e->velV = 0;
            e->velH = (P.h->p.whole - e->h->p.whole) * 8;
            e->velV = (P.y.p.whole - e->y.p.whole) * 8;
            o->state = 1;
            break;
        case 1:
            FUN_8001fce4(e);
            e->y.raw += e->velY << 8;
            e->velY += 0x30;
            if (e->velY > 0) o->state = 2;
            if (--o->w08 == 0) {
                D_800A603C = 1;
                D_8009C330->animFrame = 0xffff;
                D_800A603D = 0;
                D_800A603E = 0;
                e->b69 = 0;
                e->velH = 0;
                e->velV = 0;
                e->velX = 0;
                e->velY = 0;
                e->b04 = 2;
                e->step = 3;
                e->state = 0;
                o->b04 = 3;
                o->step = 0;
                o->state = 0;
                D_8009CDB2 = 2;
                { int *q = &D_8009C984; *q |= 0x100; }
                FUN_8004d620(0x13, 2);
                D_8009C93E = 0;
                D_8009C942 = 0;
                D_800A60F8 = 0;
                D_8009C93F = 0;
            }
            break;
        case 2:
            FUN_8001fce4(e);
            e->y.raw += e->velY << 8;
            e->velY += 0x30;
            if (--o->w08 == 0) {
                D_800A603C = 1;
                D_8009C330->animFrame = 0xffff;
                D_800A603D = 0;
                D_800A603E = 0;
                e->b69 = 0;
                e->velH = 0;
                e->velV = 0;
                e->velX = 0;
                e->velY = 0;
                e->b04 = 2;
                e->step = 3;
                e->state = 0;
                o->b04 = 3;
                o->step = 0;
                o->state = 0;
                D_8009CDB2 = 2;
                D_8009C942 = 0;
                D_800A60F8 = 0;
                D_8009C93F = 0;
                D_8009C93E = 0;
                { int *q = &D_8009C984; *q |= 0x100; }
            }
            break;
        }
        break;
    }
}
