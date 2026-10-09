// FUNC 8012a794 976 X001
/* score 6: only the movement angle: game keeps d8c in a2 and the angle in a1 (move a1,a2 / addiu a1,a2,0x80);
   here both share one register. Tried if/else, ternary, inline helpers taking d or the angle, int/short types. */
#include "TOBJ.H"
typedef struct { short w0, w2, w4, w6, w8; } X;

extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern short D_1F80027E;
extern short D_8007A5F0[], D_8007A1F0[];
extern unsigned char D_8013C7C8[], D_8013C7D8[];
extern char D_80077CE8[], D_80077CF4[];
extern void *D_8013FC74[], *D_8013FCA0[];
extern void playSFX(int);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int Rand(void);
extern void FUN_8001faf4(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short FUN_800408d8(TObj *, short, short);
extern void func_80128110(TObj *);

static __inline__ short wall(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    int f;
    short d;

    if ((o->b9d & 2) && o->animFrame == (o->b9d & 1)) {
        o->wba = 1;
        return 1;
    }
    f = o->animFrame & 1;
    if ((unsigned short)f) d = -0x10;
    else d = 0x10;
    if (func_8004065C(o, o->h->p.whole + d, o->y.p.whole, f)) {
        x->w6 = 0;
        return 1;
    }
    return 0;
}

static __inline__ void mv(TObj *o, int a, short sp)
{
    a &= 0xff;
    o->h->raw += (D_8007A5F0[a] * sp) >> 4;
    o->y.raw += (D_8007A1F0[a] * sp) >> 4;
}

void func_8012A794(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);

    switch (o->state) {
    case 0:
        o->b9c = 0;
        o->wbc = 1;
        o->wac = 1;
        o->state++;
        o->anim = D_8013FC74[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (o->visible && !((D_1F8001F8 + D_1F800198) & 0xf)) playSFX(0x5e);
        AnimAdvance(o);
        if (x->w6) {
            {
            int d = o->d8c;
            short sp = *(short *)o->movetab;
            mv(o, o->animFrame ? d : d + 0x80, sp);
            }
            o->y.p.whole -= 2;
        } else {
            FUN_8001faf4(o);
            o->y.p.whole -= 3;
        }
        if (wall(o)) {
            x->w8 = 1;
            o->step = 2;
            o->state = 0;
            o->wb0 = 1;
            break;
        }
        if (o->b69 == 4) {
            x->w6 = 1;
            o->b69 = 0;
        } else if (!FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
            x->w8 = 1;
            o->step = 2;
            o->state = 0;
            o->wb0 = 0;
            break;
        } else {
            o->b69 = 0;
            o->d8c = (-D_1F80027E * 4 + 0x80) & 0xff;
            x->w6 = 0;
        }
        if (--o->timer == -1) {
            o->timer = 0x80;
            o->state++;
        }
        break;
    case 2:
        func_80128110(o);
        o->state = 0;
        if (o->step == 1) {
            if (D_8013C7C8[Rand() & 0xf]) o->movetab = D_80077CE8;
            else o->movetab = D_80077CF4;
            if (D_8013C7D8[Rand() & 7]) {
                o->timer = 300;
                o->ba7 = 0;
            } else {
                o->timer = 200;
                o->ba7 = 1;
            }
            o->step = 8;
            o->state = 1;
        } else if (o->step == 3) {
            o->step = 4;
            o->velV = 0;
            o->state = 0;
            o->wac = 0xc;
            o->anim = D_8013FCA0[0];
            AnimLoadDuration(o);
        }
        break;
    }
}
