// FUNC 8011708c 1236 X011
// MATCHING 8011708c 1236
#include "TOBJ.H"
typedef struct { unsigned char c[12]; } B12;
typedef struct { void **p; int x, y; } AT;
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern TObj *FUN_80018568(void);
extern void FUN_8005a8a8(int, int, int);
extern signed char D_8009D2B0;
extern unsigned short D_800A604A;
extern Fix16 *D_800A607C;
extern unsigned short D_1F8001FC, D_1F8003C4;
extern unsigned char D_8009C942[];
extern unsigned char D_800A603C[0], D_800A603D[0], D_800A603E[0];
extern unsigned char D_8009D082;
extern short D_800A60EA[];
extern short D_1F8001C6;
extern AT D_80119C48[];

void func_8011708C(TObj *o)
{
    B12 v;
    signed char *p;
    TObj *q;

    switch (o->state) {
    case 0:
        p = &D_8009D2B0;
        if (*p == 3) break;
        if ((unsigned short)(D_800A604A - 0x40) >= 0x20) break;
        if (D_800A607C->p.whole != 0) break;
        if (!(D_1F8001FC & D_1F8003C4)) break;
        D_8009C942[0] = 1;
        *p = 0;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(B12 *)&o->a;
        o->anim = D_80119C48[o->subtype].p[1];
        FUN_8001fe6c(o);
        o->d90 = FUN_8002dcc8(2, 6, &v);
        o->state++;
        break;
    case 1:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        D_1F8001C6 = 0;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        o->animFrame = 1;
        q = FUN_80018568();
        if (q != 0) {
            q->active = 2;
            q->type = 0xf;
            q->subtype = 0x33;
            q->b0c = 0x80;
            q->b0f = 0;
            q->animFrame = 0;
        }
        o->anim = D_80119C48[o->subtype].p[0];
        FUN_8001fe6c(o);
        o->state = 2;
        break;
    case 2:
        if (D_8009D082 != 1) break;
        v = *(B12 *)&o->a;
        o->anim = D_80119C48[o->subtype].p[2];
        FUN_8001fe6c(o);
        o->d90 = FUN_8002dcc8(2, 7, &v);
        o->state++;
        break;
    case 3:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        D_8009D082 = 2;
        o->state++;
        break;
    case 4:
        if (D_8009D082 != 3) break;
        v = *(B12 *)&o->a;
        o->d90 = FUN_8002dcc8(2, 8, &v);
        o->state++;
        break;
    case 5:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        FUN_8005a8a8(0x68, 0, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 6:
        if (--o->timer != 0) break;
        o->animFrame = 0;
        v = *(B12 *)&o->a;
        o->d90 = FUN_8002dcc8(2, 9, &v);
        o->state++;
        break;
    case 7:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        v = *(B12 *)&o->a;
        o->d90 = FUN_8002dcc8(2, 0xa, &v);
        o->state++;
        break;
    case 8:
        if (((unsigned char *)o->d90)[4] != 2) break;
        ((unsigned char *)o->d90)[4] = 3;
        o->anim = D_80119C48[o->subtype].p[0];
        FUN_8001fe6c(o);
        o->state++;
        break;
    case 9:
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
