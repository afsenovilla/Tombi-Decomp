// FUNC 801162b4 988 X005
// MATCHING 801162b4 988
#include "TOBJ.H"
typedef struct { unsigned char c[12]; } B12;
typedef struct { void **p; int x, y; } AT;
extern int FUN_8002dcc8(int, int, void *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8005a9a4(int, int);
extern unsigned char D_8009CDD5;
extern unsigned char D_8009CDD2;
extern unsigned char D_8009CF00;
extern unsigned char D_8009CF04;
extern unsigned char D_8009C942[];
extern unsigned char D_8009D2B0;
extern unsigned char D_8009C93F[];
extern unsigned char D_8009CEED;
extern unsigned char D_800A603C[];
extern unsigned char D_800A603D[];
extern unsigned char D_800A603E[];
extern short D_800A60EA[];
extern short D_1F8001C6;
extern AT D_801176CC[];

void func_801162B4(TObj *o)
{
    B12 v;
    int t;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(B12 *)&o->a;
        if (D_8009CDD5 == 2) {
            if (D_8009CF00 != 0 && D_8009CF04 != 0) {
                o->d90 = FUN_8002dcc8(2, 0x32, &v);
                o->state = 5;
            }
        } else if (D_8009CDD5 == 1) {
            o->d90 = FUN_8002dcc8(2, 0x29, &v);
            o->state++;
        } else {
            switch (D_8009CDD2) {
            case 0:
            case 1:
                o->d90 = FUN_8002dcc8(2, 0x27, &v);
                o->state++;
                break;
            case 2:
                o->d90 = FUN_8002dcc8(2, 0x28, &v);
                o->state = 2;
                break;
            default:
                o->d90 = FUN_8002dcc8(2, 0x29, &v);
                o->state++;
                break;
            }
        }
        D_8009C942[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        break;
    case 1:
        if (((unsigned char *)o->d90)[4] == 2) {
            ((unsigned char *)o->d90)[4] = 3;
            D_1F8001C6 = 0;
            D_8009C942[0] = 0;
            D_800A60EA[0] = 0;
            if (D_8009C93F[0] == 0) {
                D_800A603C[0] = 1;
                D_800A603D[0] = 0;
                D_800A603E[0] = 0;
            }
            t = (unsigned short)o->wbc;
            o->b68 = 0;
            goto reset;
        }
        break;
    case 2:
        if (((unsigned char *)o->d90)[4] == 2) {
            ((unsigned char *)o->d90)[4] = 3;
            o->animFrame = 1;
            o->active = 4;
            o->anim = D_801176CC[o->subtype].p[1];
            AnimLoadDuration(o);
            o->state++;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += -0x20000;
        if (o->visible == 0) o->state++;
        break;
    case 4:
        FUN_8005a9a4(0x2e, 0);
        D_1F8001C6 = 0;
        D_8009CEED = 2;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        if (D_8009C93F[0] == 0) {
            D_800A603C[0] = 1;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
        }
        t = (unsigned short)o->wbc;
        o->b68 = 0;
        goto reset;
    case 5:
        if (((unsigned char *)o->d90)[4] == 2) {
            ((unsigned char *)o->d90)[4] = 3;
            o->anim = D_801176CC[o->subtype].p[0];
            AnimLoadDuration(o);
            FUN_8005a9a4(0x31, 0);
            o->state++;
        }
        break;
    case 6:
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_8009C93F[0] = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        t = (unsigned short)o->wbc;
        D_1F8001C6 = 0;
        o->b68 = 0;
    reset:
        o->step = 0;
        o->state = 0;
        o->animFrame = t;
        break;
    }
}
