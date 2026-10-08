// FUNC 801377b8 1936 X000
// MATCHING 801377b8 1936
#include "TOBJ.H"
typedef struct { TObj o; unsigned char bc0; } BigObj;
extern BigObj DAT_800a6038;
#define X DAT_800a6038.o
extern unsigned char DAT_8009c93e, DAT_8009c93f, DAT_8009c940, DAT_8009c941, DAT_8009c942;
extern unsigned char DAT_8009cebd, DAT_8009d0a7, DAT_8009d2b0, DAT_8009ce4a, DAT_8009d00d, DAT_8009f3dc;
extern int DAT_8009c984;
extern unsigned char DAT_8009d00dA[];
extern unsigned char *DAT_8009c330;
extern void *PTR_8013ac14, *PTR_8013ac18, *PTR_8013ac20, *PTR_8013ac24, *PTR_8013ac2c;
extern char DAT_801396ca[];
extern char D_80010748[];
extern int FUN_8002dc50(int, int, int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_8003c7c4(void *, void *, int, int, int);
extern void FUN_80026e0c(int, int);
extern void FUN_8004d620(int, int);
extern void FUN_8001e4f0(int);

#define CEBD (*(volatile unsigned char *)&DAT_8009cebd)

static __inline__ short chk(void)
{
    short r = 0;
    if (DAT_8009d0a7 != 0 && DAT_8009c940 != 0)
        r = DAT_8009c941 == 3;
    return r;
}
static __inline__ int chk2(void)
{
    int r = 0;
    if (DAT_8009d0a7 != 0) {
        r = 0;
        if (DAT_8009c940 != 0)
            r = DAT_8009c941 == 3;
    }
    return r;
}

void FUN_801377b8(TObj *o)
{
    TObj *p = *(TObj **)&o->category;
    short buf[6];
    int f;

    switch (o->step) {
    case 0:
        DAT_8009c93f = 1;
        if (*(unsigned short *)&o->anim != 0) {
            DAT_800a6038.bc0 = 1;
            DAT_8009cebd = 1;
            o->step = 3;
            break;
        }
        if (chk()) {
            X.animFrame = 1;
            DAT_8009cebd = 2;
            DAT_8009c93e = 1;
            X.b04 = 1;
            X.step = 0;
            X.state = 0;
            DAT_8009d2b0 = 0;
            X.d90 = FUN_8002dc50(4, 2, 0x80, 0x6c);
            p->anim = PTR_8013ac20;
            AnimLoadDuration(p);
            p->b04 = 2;
            p->step = 4;
            p->state = 0;
        } else {
            DAT_8009cebd = 0;
            X.d90 = FUN_8002dc50(4, 0, 0x80, 0x6c);
        }
        o->step++;
        break;
    case 1:
        AnimAdvance(p);
        if (((TObj *)X.d90)->b04 != 2)
            break;
        switch (DAT_8009cebd) {
        case 0:
            ((TObj *)X.d90)->b04 = 3;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            X.animFrame = 1;
            X.b04 = 6;
            X.step = 5;
            X.state = 0;
            DAT_8009c330[9] = 10;
            X.wb2 = 0;
            X.velH = 0;
            X.velV = 0;
            X.velX = 0xc0;
            X.b69 = 0;
            DAT_8009cebd = 1;
            o->step = 3;
            break;
        case 1:
            ((TObj *)X.d90)->b04 = 3;
            p->anim = PTR_8013ac18;
            AnimLoadDuration(p);
            p->active = 1;
            p->b04 = 1;
            p->step = 0;
            p->state = 0;
            if (DAT_8009ce4a == 0) {
                FUN_8005a8a8(0xa6, 0, 0);
                o->w08 = 300;
                o->step = 6;
                o->state = 0;
                o->substep = 0;
                break;
            }
            DAT_8009cebd = 0;
            X.animFrame = 1;
            X.b04 = 1;
            X.step = 0;
            X.state = 0;
            X.velX = 0;
            X.velY = 0;
            X.b69 = 1;
            DAT_8009c330[9] = 0;
            o->step = 0;
            o->state = 0;
            goto clear;
        case 2:
            ((TObj *)X.d90)->b04 = 3;
            buf[1] = p->h->p.whole + 0x10;
            buf[3] = p->y.p.whole;
            buf[5] = p->d->p.whole;
            FUN_8003c7c4(DAT_801396ca, buf, 0, 0, 1);
            X.b04 = 5;
            X.step = 0;
            X.state = 0;
            p->active = 1;
            DAT_8009cebd = 3;
            FUN_80026e0c(3, 1);
            o->w08 = 0x50;
            o->step = 4;
            if (DAT_8009d00dA[0] != 0)
                o->step = 7;
            break;
        case 3:
            ((TObj *)X.d90)->b04 = 3;
            CEBD = 4;
            CEBD = DAT_8009f3dc + 4;
            p->anim = PTR_8013ac2c;
            AnimLoadDuration(p);
            o->step = 4;
            break;
        case 4: case 5: case 6: case 7: case 8: case 9:
            ((TObj *)X.d90)->b04 = 3;
            p->anim = PTR_8013ac14;
            AnimLoadDuration(p);
            CEBD = 10;
            CEBD = DAT_8009f3dc + 10;
            o->step = 4;
            break;
        case 10: case 11: case 12: case 13: case 14: case 15:
            ((TObj *)X.d90)->b04 = 3;
            FUN_8005a9a4(0xa6, 0);
            o->step = 5;
            if (DAT_8009d00d == 0)
                FUN_8004d620(0xc, 3);
            p->b04 = 2;
            p->step = 3;
            p->state = 0;
            o->w08 = 0x50;
            break;
        }
        break;
    case 3:
        AnimAdvance(p);
        if (X.b69 != 0) {
            p->active = 1;
            p->anim = PTR_8013ac24;
            AnimLoadDuration(p);
            p->b04 = 2;
            p->step = 4;
            p->state = 0;
            X.animFrame = 1;
            X.b04 = 5;
            X.step = 0;
            X.state = 0;
            X.b69 = 4;
            X.anim = D_80010748;
            AnimLoadDuration(&X);
            DAT_8009cebd = 1;
            X.d90 = FUN_8002dc50(4, 1, 0x80, 0x6c);
            o->step = 1;
        }
        break;
    case 4:
        if (o->w08 == 10)
            FUN_8001e4f0(9);
        if (--o->w08 > 0)
            break;
        X.d90 = FUN_8002dc50(4, DAT_8009cebd, 0x80, 0x6c);
        o->step = 1;
        break;
    case 5:
        if (--o->w08 > 0)
            break;
        DAT_8009d00d = 0;
        X.b04 = 1;
        X.step = 0;
        X.state = 0;
        DAT_8009c942 = 0;
        DAT_800a6038.bc0 = 0;
        DAT_8009c93e = 0;
        DAT_8009c93f = 0;
        { unsigned char *c = &DAT_8009cebd; *c = *c + 1; }
        o->b04 = 2;
        DAT_8009c940 = 0;
        DAT_8009c984 |= 0x40;
        break;
    case 6:
        if (--o->w08 > 0)
            break;
        X.animFrame = 1;
        X.b04 = 1;
        X.step = 0;
        X.state = 0;
        X.velX = 0;
        X.velY = 0;
        X.b69 = 1;
        o->step = 0;
        o->state = 0;
        DAT_8009cebd = 0;
    clear:
        *(short *)&o->anim = 0;
        DAT_800a6038.bc0 = 0;
        DAT_8009c93f = 0;
        DAT_8009c942 = 0;
        DAT_8009c93e = 0;
        break;
    case 7:
        if (o->w08 == 10)
            FUN_8001e4f0(9);
        if (--o->w08 > 0)
            break;
        X.d90 = FUN_8002dc50(4, 3, 0x80, 0x6c);
        o->step = 1;
        break;
    }
}
