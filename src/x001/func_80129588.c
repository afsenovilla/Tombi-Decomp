// FUNC 80129588 1444 X001
// MATCHING 80129588 1444
#include "TOBJ.H"
typedef struct { short w0, w2, w4, w6, w8; } S;
extern unsigned char D_8013C7C8[];
extern char D_80077CF4[];
extern void *D_8013FC84;
extern void *D_8013FC7C;
extern void *D_8013FCA0;
extern void *D_8013FC74;
extern short D_1F80027E;
extern short D_1F80016A;
extern int Rand(void);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short FUN_8004065c(TObj *, short, short, int);
extern short FUN_800408d8(TObj *, short, short);

#define SET_ANIM(o, a) do { (o)->anim = (a); AnimLoadDuration(o); } while (0)

static __inline__ short chk(TObj *o)
{
    S *q = (S *)&o->wb4;
    int f;
    short d;

    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) {
        o->wba = 1;
        return 1;
    }
    f = o->animFrame & 1;
    if ((unsigned short)f) d = -0x10;
    else d = 0x10;
    if (FUN_8004065c(o, o->h->p.whole + d, o->y.p.whole, f)) {
        q->w6 = 0;
        return 1;
    }
    return 0;
}

static __inline__ unsigned char below(TObj *o)
{
    return o->y.p.whole <= o->wb6;
}

void func_80129588(TObj *o)
{
    S *p = (S *)&o->wb4;
    int t;

    switch (o->state) {
    case 0:
        o->b69 = 0;
        o->b9c = 0;
        o->wac = 5;
        o->state++;
        SET_ANIM(o, D_8013FC84);
        break;
    case 1:
        if (AnimAdvance(o))
            o->state++;
        break;
    case 2:
        o->b9c = 1;
        o->wac = 3;
        SET_ANIM(o, D_8013FC7C);
        t = o->d8c - 0x21;
        o->movetab = D_80077CF4;
        if ((unsigned int)t < 0xc0) {
            o->velH = 0x300;
            o->state = 7;
            if (o->d8c & 0x80)
                o->animFrame = 0;
            else
                o->animFrame = 1;
        } else {
            o->state++;
        }
        break;
    case 3:
        o->b9c = 1;
        o->b69 = 0;
        o->d8c = 0;
        o->velV = -0x400;
        o->state++;
    case 4:
        AnimAdvance(o);
        o->velV += 0x20;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 1;
            o->state++;
        } else if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
            o->velV = 0;
        }
        break;
    case 5:
        o->d8c = 0;
        if (o->w22)
            o->velV = -0x500;
        else if (D_8013C7C8[Rand() & 0xf])
            o->velV = -0x500;
        else
            o->velV = -0x400;
        o->b9c = 1;
        o->b69 = 0;
        o->wac = 0xc;
        o->state++;
        SET_ANIM(o, D_8013FCA0);
        break;
    case 6:
        AnimAdvance(o);
        if (o->w22 && !below(o)) {
            if (o->velV > -0x100)
                o->velV = -0x100;
        } else {
            o->velV += 0x40;
        }
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->step = 5;
            o->state = 0;
            break;
        }
        if (o->b69 != 4 && !FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10))
            break;
        o->velV = 0;
        if (o->b69 == 4) {
            p->w6 = 1;
            o->wb2 = 0x80;
        } else {
            o->wb2 = ((-D_1F80027E << 2) + 0x80) & 0xff;
            p->w6 = 0;
        }
        {
            int d = o->d8c;
            if ((unsigned)((d - (unsigned short)o->wb2) & 0xff) > 0x80)
                o->state = 8;
            else
                o->state = 9;
        }
        break;
    case 7:
        o->velH -= 0x10;
        if (o->velH < 0) {
            o->velH = 0;
            o->state = 3;
        }
        if (o->animFrame & 1) {
            if (o->d8c) o->d8c--;
            o->h->raw -= o->velH << 8;
        } else {
            if (o->d8c) o->d8c++;
            o->h->raw += o->velH << 8;
        }
        o->d8c = *(unsigned char *)&o->d8c;
        o->y.raw += -0x8000;
        AnimAdvance(o);
        FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10);
        chk(o);
        break;
    case 8:
        o->d8c += 2;
        if ((unsigned)((o->d8c - (unsigned short)o->wb2) & 0xff) < 0x80) {
            if (o->w22)
                o->animFrame = o->h->p.whole > o->wb4;
            else
                o->animFrame = D_1F80016A < o->h->p.whole;
            o->step = 8;
            o->state = 0;
            o->wac = 1;
            o->d8c = o->wb2;
            SET_ANIM(o, D_8013FC74);
        }
        break;
    case 9:
        o->d8c -= 2;
        if ((unsigned)((o->d8c - (unsigned short)o->wb2) & 0xff) > 0x80) {
            if (o->w22)
                o->animFrame = o->h->p.whole > o->wb4;
            else
                o->animFrame = D_1F80016A < o->h->p.whole;
            o->step = 8;
            o->state = 0;
            o->wac = 1;
            o->d8c = o->wb2;
            SET_ANIM(o, D_8013FC74);
        }
        break;
    }
}
