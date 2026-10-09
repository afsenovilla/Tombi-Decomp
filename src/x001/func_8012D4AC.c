// FUNC 8012d4ac 2796 X001
// MATCHING 8012d4ac 2796
#include "TOBJ.H"
typedef struct A { short w0; unsigned short w2; unsigned short w4; unsigned short w6; } A;
typedef struct B { unsigned char c[4]; } B;
extern char D_80077CF4[];
extern B D_8013C800[];
extern A *D_8013DAE8[];
extern unsigned char D_800A603E;
extern unsigned char D_800A60E4[];
extern short D_1F80027E;
extern short D_1F800284;
extern void FUN_80026bfc(int, int);
extern void FUN_8001fb20(TObj *);
extern void FUN_8001faf4(TObj *);
extern TObj *FUN_80018448(void);
extern short func_8004065C(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

static __inline__ int land(TObj *o)
{
    short t;
    short w;

    if (o->b69 == 1) {
        o->d8c = 0;
        o->wb2 = 0;
        o->b69 = 0;
        o->wae = -1;
        o->b9c = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        t = D_1F80027E;
        if (t < 0) t = -t;
        if (t >= 9) t = 8;
        if (D_1F80027E < 0) t = -t;
        o->d8c = -t & 0xff;
        o->wb2 = t;
        w = D_1F800284;
        o->wb6 = (-t << 2) & 0xff;
        o->b9c = 0;
        o->wae = w;
        return 1;
    }
    return 0;
}

#define SETANIM(k) \
    o->anim = D_8013DAE8[k]; \
    { unsigned char *p; { B *t = D_8013C800; p = t[((A *)o->anim)->w2].c; } \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; } \
    o->animTimer = ((A *)o->anim)->w6 & 0x3fff;

#define COMMON() \
    FUN_8001fb20(o); \
    func_8004065C(o, o->h->p.whole + 0x10, o->y.p.whole + 8, 0); \
    func_8004065C(o, o->h->p.whole - 0x10, o->y.p.whole + 8, 1); \
    land(o);

void func_8012D4AC(TObj *o)
{
    short *e = &o->wb4;
    TObj *n;
    short v;
    int d;

    switch (o->state) {
    case 0:
        o->b9c = 0;
        FUN_80026bfc(0, 6);
        o->movetab = D_80077CF4;
        *(signed char *)&o->b0f = -7;
        o->b68 = 0;
        o->d8c = 0;
        o->velV = 0;
        o->state++;
    case 1:
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        func_8004065C(o, o->h->p.whole + 0x10, o->y.p.whole + 8, 0);
        func_8004065C(o, o->h->p.whole - 0x10, o->y.p.whole + 8, 1);
        if (land(o)) o->velV = 0;
        break;
    case 2:
        D_800A603E = 2;
        o->timer = 4;
        o->wac = 6;
        o->state++;
        SETANIM(6);
        COMMON();
        break;
    case 3:
        if (--o->timer == -1) {
            o->timer = 8;
            o->wac = 7;
            o->state++;
            SETANIM(7);
            n = FUN_80018448();
            if (n) {
                n->active = 1;
                n->type = 0x10;
                n->subtype = 0;
                d = -0x10;
                if (o->animFrame & 1) d = 0x10;
                n->a.p.whole = o->a.p.whole + d;
                n->y.p.whole = o->y.p.whole;
                n->b.p.whole = o->b.p.whole;
                n->animFrame = o->animFrame & 1;
            }
        }
        COMMON();
        break;
    case 4:
        if (--o->timer == -1) {
            o->wac = 8;
            SETANIM(8);
            o->state++;
            o->timer = 0xe;
        }
        COMMON();
        break;
    case 5:
        FUN_8001faf4(o);
        o->y.p.whole += 3;
        func_8004065C(o, o->h->p.whole + 0x10, o->y.p.whole + 8, 0);
        func_8004065C(o, o->h->p.whole - 0x10, o->y.p.whole + 8, 1);
        if (land(o)) {
            v = D_1F80027E;
            if (v < 0) v = -v;
            if (v > 8) v = 8;
            if (D_1F80027E < 0) v = -v;
            o->wb2 = v;
            o->d8c = -v & 0xff;
            o->wb6 = (-v << 2) & 0xff;
            o->wae = D_1F800284;
            o->b9c = 0;
            if (o->animFrame) {
                if (o->wb2 < 0) {
                    o->velH = 0x200;
                    o->step = 3;
                    o->state = 1;
                    e[8] = 1;
                    break;
                }
            } else if (o->wb2 >= 2) {
                o->velH = 0x200;
                o->step = 3;
                o->state = 1;
                e[8] = 0;
                break;
            }
        }
        if (--o->timer == -1) {
            D_800A60E4[0] = 3;
            o->wac = 6;
            SETANIM(6);
            o->state++;
        }
        break;
    case 6:
        o->wac = 6;
        SETANIM(6);
        COMMON();
        break;
    case 7:
        o->wac = 9;
        SETANIM(9);
        o->state++;
        break;
    case 8:
        break;
    }
}
