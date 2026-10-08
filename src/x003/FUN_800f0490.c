// FUNC 800f0490 1740 X003
// MATCHING 800f0490 1740
#include "TOBJ.H"
typedef struct {
    unsigned char b0;
    unsigned char _p1[3];
    unsigned char b4;
    unsigned char _p5[0xf];
    short w14;
    unsigned char _p16[0x12];
    unsigned short w28;
    unsigned short w2a;
    unsigned short w2c;
    unsigned short w2e;
} PL;
typedef struct {
    TObj t;
    unsigned char _pc0[9];
    unsigned char bc9;
    unsigned char _pca[0x24];
    unsigned short wee;
    unsigned short wf0;
    unsigned short wf2;
    unsigned short wf4;
    short wf6;
    PL pl;
} TObjX;
#define X(o) ((TObjX *)(o))
typedef struct { unsigned char c, f, a, b; } E;
typedef struct { unsigned short a, b; } T2;

extern int FUN_8005efe4(void);
extern void FUN_80037e18(int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8002c654(void);
extern void FUN_8002cd20(TObj *, int, int);
extern void FUN_80118910(void);
extern void FUN_80122990(TObj *);
extern void FUN_801228bc(TObj *);
extern void FUN_801212a0(TObj *);
extern void FUN_8011dc4c(TObj *);
extern void FUN_8011d8c4(TObj *);
extern void FUN_8011d334(TObj *);
extern PL *DAT_8009c330;
extern unsigned char DAT_8009d2b2[];
extern unsigned char DAT_8009cff8;
extern unsigned char DAT_8009d006[];
extern unsigned char DAT_8009cf06;
extern unsigned char DAT_8009d2b3;
extern unsigned char DAT_8009c990;
extern unsigned char DAT_800a60fa[];
extern T2 DAT_8007a024[];
extern unsigned short DAT_800a6040[];
extern unsigned short DAT_800a60fc[];
extern unsigned char DAT_8009c970, DAT_8009c970b[];
extern int DAT_1f8002c8[];
extern char DAT_80010748[];
extern unsigned char DAT_8009cef8;
extern unsigned short DAT_8009c960;
extern unsigned int DAT_8009c960i;
extern unsigned short DAT_8009c962;
extern unsigned short DAT_8009c982;
extern signed char DAT_8009d2b0;
extern E **DAT_80115190[];
extern unsigned char DAT_8009c93a, DAT_8009c93aA[];
extern unsigned char DAT_8009ce41;

#define LOADP() p = (unsigned char *)DAT_80115190[DAT_8009c960]; p = ((unsigned char **)p)[DAT_8009c962]; p += DAT_8009c982 * 4

void FUN_800f0490(TObj *o)
{
    PL *pl;
    unsigned char *p;
    short c;
    unsigned short t;
    Fix16 *d;
    int a;
    char pad[0xe0];

    DAT_8009c330 = &X(o)->pl;
    switch (o->step) {
    case 0:
        pl = DAT_8009c330;
        if (FUN_8005efe4() != 1) FUN_8005efe4();
        pl->w14 = 0;
        o->w1e = 0;
        switch (DAT_8009d2b2[0]) {
        case 8: a = 1; break;
        case 9: a = 2; break;
        case 7: a = 0; break;
        default: a = 0; break;
        }
        FUN_80037e18(a);
        {
            PL *q = DAT_8009c330;
            *(unsigned char *)q = 0;
            q->w28 = 0xffff;
            q->w2a = 0xffff;
            q->w2e = 0xffff;
        }
        DAT_8009c330->b4 = 0;
        o->visible = 1;
        *(signed char *)&o->b0f = -8;
        o->b0a = 0;
        o->ba5 = 0;
        o->wb0 = 0;
        o->velH = 0;
        o->velV = 0;
        o->wb2 = 0;
        o->b9d = 0;
        o->subtype = 0;
        DAT_8009cff8 = 0;
        if (DAT_8009d006[0]) o->subtype = 2;
        if (DAT_8009cf06) o->subtype = 1;
        else DAT_8009d2b3 &= 3;
        DAT_800a60fa[0] = DAT_8009d2b3;
        DAT_800a6040[0] = DAT_8007a024[DAT_8009c990].a;
        DAT_800a60fc[0] = DAT_8007a024[DAT_8009d2b3].b;
        o->w9a = DAT_8009c970;
        o->w98 = DAT_8009c970b[0];
        o->d3c = DAT_1f8002c8[0];
        o->anim = DAT_80010748;
        FUN_8001fe6c(o);
        o->step++;
        if (DAT_8009cef8) {
            X(o)->bc9 = 1;
            FUN_8002c654();
        }
        switch (DAT_8009c960) {
        case 0: FUN_801228bc(o); break;
        case 1: case 7: FUN_801212a0(o); break;
        case 3: FUN_8011dc4c(o); break;
        case 4: case 12: FUN_8011d8c4(o); break;
        case 9: FUN_8011d334(o); break;
        }
        switch (DAT_8009d2b0) {
        case 3:
            LOADP();
            { unsigned char t3 = p[2];
            o->b04 = 5;
            o->step = 4;
            o->state = 0;
            o->substep = 0;
            o->animFrame = t3; }
            break;
        case 4:
            LOADP();
            o->active = 1;
            { unsigned char t4 = p[2];
            o->b04 = 1;
            o->step = 99;
            o->state = 0;
            o->animFrame = t4; }
            break;
        }
        if (DAT_8009c960i == 0x2000e) {
            o->b04 = 1;
            o->state = 0;
            o->step = 0x3e;
            o->active = 1;
            o->animFrame = 0;
            o->w22 = 0;
            o->w56 = -2000;
            DAT_8009c93a = 1;
        }
        break;
    case 1:
        LOADP();
        c = *p; p += 2;
        o->animFrame = p[0];
        o->timer = p[1];
        if (c == 99) {
            o->b04 = 5;
            o->step = 12;
            o->state = 0;
            break;
        }
        if (c == 98) {
            if (DAT_8009ce41) break;
            o->b04 = 5;
            o->step = 4;
            o->state = 0;
            break;
        }
        if (o->animFrame & 0x80) FUN_8002cd20(o, 0, 0);
        if (o->animFrame & 0x40) {
            o->w22 = 0x3c;
            if ((c & 0xff) == 7 || (c & 0xff) == 9) o->w22 = 1;
        }
        o->b04 = 5;
        o->animFrame &= 0xf;
        if (c == 2) {
            DAT_8009c93a = 1;
            o->active = 1;
        }
        o->step = c;
        o->state = 0;
        o->substep = 0;
        if (DAT_8009c960 == 0) FUN_80122990(o);
        switch (o->step) {
        case 2:
            { unsigned short t2 = o->h->p.whole;
            X(o)->wf2 = o->y.p.whole;
            X(o)->wf6 = 0;
            o->active = 3;
            o->b04 = 0;
            o->w22 = 0x3c;
            o->step = 3;
            o->state = 0;
            o->substep = 0;
            X(o)->wee = t2; }
            DAT_8009c93aA[0] = 1;
            break;
        case 3:
            FUN_80118910();
            t = o->h->p.whole;
            X(o)->wf2 = o->y.p.whole;
            d = o->d;
            X(o)->wee = t;
            { int v = d->p.whole - 0x5a;
            o->active = 3;
            o->timer = 0x3c;
            o->w22 = 0x3c;
            o->b04 = 0;
            o->step = 4;
            o->state = 0;
            o->substep = 0;
            X(o)->wf6 = (v / 0x5a) * 0x5a; }
            break;
        }
        break;
    case 2:
        o->active = 3;
        LOADP();
        c = p[0];
        o->w22 = 0x3c;
        { unsigned char t5 = p[2];
        o->b04 = 1;
        o->state = 0;
        o->substep = 0;
        o->animFrame = t5 & 0xf; }
        o->step = c;
        DAT_8009c93a = 1;
        break;
    case 3:
        break;
    case 4:
        if (--o->timer <= 0) {
            o->b04 = 5;
            o->step = 2;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
