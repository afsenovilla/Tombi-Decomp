// FUNC 80125818 3148 X004
/* score 222 (draft, 3136 of 3148 B): substep AI with 11 cases; logic complete incl. the near() player-range inline
   (used twice) and the per-case wbc tick. o35: case 4's k test is `switch (k) { case 0: ... case 1: ... }` (439->222).
   Remaining: near() keeps its result in a0 and copies s to v1 (tried int/short/uchar s, dy types, result var,
   assigning the call result: no gain); lbu substep scheduled after the li in several setanim+substep++ tails
   (statement swaps break cross-jumping; plain vs do-while SETANIM per site: no gain); case 5/8/9 operand regs. */
#include "TOBJ.H"
typedef struct { short x, y; } P2;
typedef struct {
    TObj o;
    unsigned short wc0, wc2, wc4, wc6, wc8, wca, wcc;
} TX;
#define X(o) ((TX *)(o))
#define U16(f) (*(unsigned short *)&(f))
extern void *D_801345B8, *D_8013460C, *D_80134610, *D_801345CC, *D_801345D0;
extern char D_80077CDC[], D_80077D0C[], D_80077D84[];
extern Fix16 *D_800A6078, *D_800A607C;
extern unsigned short D_800A604E;
extern short D_1F8001C8;
extern short D_80131148[];
extern unsigned short D_80131128[];
extern int Rand(void);
extern void SfxPlay(int);
extern void AnimLoadDuration(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001fa60(TObj *, int);
extern int FUN_8001fdac(int, int);
extern int FUN_8002078c(P2, P2);
extern short FUN_800411cc(TObj *, short, short);
extern short FUN_80041ebc(TObj *, short, short);

#define TICK if (--U16(o->wbc) == 0) { o->wbc = 0x28; if (X(o)->wca) SfxPlay(0x12); }
#define SETANIM(a) do { o->anim = (a); AnimLoadDuration(o); } while (0);

static __inline__ int near(TObj *o)
{
    int s;
    unsigned short dy;

    if (o->d->p.whole != D_800A607C->p.whole) return 0;
    s = 0;
    if (((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x40) < 0x80 && (Rand() & 0xf) < 0xc) ||
        ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x80) < 0x100 && (Rand() & 0xf) < 6))
        s = 1;
    dy = D_800A604E - o->y.p.whole + 0x30;
    if (s != 1) return 0;
    return dy < 0xf8;
}

void func_80125818(TObj *o)
{
    P2 a, b;
    short d;
    int t;
    short k;

    switch (o->substep) {
    case 0:
        if (U16(o->wb4)) {
            o->w22 = 0xf0;
            o->wbc = 1;
            o->wac = 1;
            SETANIM(D_801345B8)
            o->substep = 4;
        } else {
            o->w22 = 0x30;
            o->wb4 = 1;
            o->wbc = 1;
            o->wac = 0x16;
            SETANIM(D_8013460C)
            o->movetab = D_80077CDC;
            o->substep++;
        }
        break;
    case 1:
        if (--o->w22 == 0) {
            o->wbc = 1;
            o->velH = 0;
            o->velV = 0;
            o->w22 = 0;
            o->wac = 0x17;
            SETANIM(D_80134610)
            o->substep++;
        }
        TICK
        break;
    case 2:
        {
            int v = o->velV;
            o->y.raw += v << 8;
            o->velV -= 0x10;
            o->w22++;
        }
        if (o->velV < -0x1ff || FUN_800411cc(o, o->h->p.whole, o->y.p.whole - 0x18)) {
            o->wbc = 1;
            o->substep++;
        }
        TICK
        break;
    case 3:
        {
            int v = o->velV;
            o->y.raw += v << 8;
            o->velV += 0x10;
            o->w22++;
        }
        if (o->velV >= 0 || FUN_800411cc(o, o->h->p.whole, o->y.p.whole - 0x18)) {
            o->wac = 1;
            SETANIM(D_801345B8)
            o->wbc = 1;
            o->substep++;
            o->w22 = 0xf0 - o->w22 % 40;
        }
        TICK
        break;
    case 4:
        if (--U16(o->wbc) == 0) {
            o->wbc = 0x38;
            if (X(o)->wca) SfxPlay(0x12);
        }
        if (--o->w22 == 0) {
            o->timer = 0xf0;
            o->wac = 1;
            SETANIM(D_801345B8)
            o->substep = 9;
        }
        if (o->w22 != 0xb4 && o->w22 != 0x3c) break;
        if (near(o)) {
            o->step = 1;
            o->state = 0;
            o->substep = 0;
            break;
        }
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->d38 = o->b.p.whole;
        k = D_80131148[Rand() & 0xf];
        switch (k) {
        case 0:
            o->animFrame = 1;
            o->wba = ((Rand() & 1) << 5) + 0x20;
            o->wbc = 0x38;
            o->movetab = D_80077D0C;
            o->wac = 6;
            SETANIM(D_801345CC)
            o->w22 = 0xb4;
            o->substep++;
            break;
        case 1:
            o->animFrame = 0;
            o->wba = ((Rand() & 1) << 5) + 0x20;
            o->wbc = 0x38;
            o->movetab = D_80077D0C;
            o->wac = 6;
            SETANIM(D_801345CC)
            o->w22 = 0xb4;
            o->substep++;
            break;
        }
        break;
    case 5:
        if (o->animFrame) d = -0x20;
        else d = 0x20;
        if (D_1F8001C8)
            t = o->b.p.whole - o->d38;
        else
            t = o->a.p.whole - o->d30;
        t += U16(o->wba);
        if (--o->w22 == 0 || (unsigned short)t < U16(o->wba) * 2 || X(o)->wc0 != D_1F8001C8 ||
            FUN_800411cc(o, o->h->p.whole + d, o->y.p.whole + 0x18)) {
            o->w22 = 0x28;
            o->wac = 1;
            SETANIM(D_801345B8)
            o->substep++;
        }
        FUN_8001fa88(o, o->animFrame);
        TICK
        break;
    case 6:
        if (--o->w22) break;
        o->w22 = 0x78;
        o->wbc = 1;
        o->movetab = D_80077D84;
        o->wac = 1;
        SETANIM(D_801345B8)
        o->substep++;
        break;
    case 7:
        if (--o->w22 == 0) {
            if (near(o)) {
                o->step = 1;
                o->state = 0;
                o->substep = 0;
            } else {
                o->movetab = D_80077D0C;
                o->wac = 6;
                o->animFrame ^= 1;
                SETANIM(D_801345CC)
                o->substep++;
            }
        }
        if (--U16(o->wbc) == 0) {
            o->wbc = 0x28;
            o->wac = 7;
            X(o)->wcc ^= 1;
            SETANIM(D_801345D0)
            if (X(o)->wca) SfxPlay(0x12);
        }
        FUN_8001fa60(o, X(o)->wcc);
        break;
    case 8:
        if (o->animFrame) d = -0x20;
        else d = 0x20;
        if (D_1F8001C8)
            t = o->b.p.whole - o->d38;
        else
            t = o->a.p.whole - o->d30;
        if ((unsigned short)(t + 4) < 8 || FUN_800411cc(o, o->h->p.whole + d, o->y.p.whole + 0x18)) {
            o->w22 = 0x3c;
            o->wac = 1;
            SETANIM(D_801345B8)
            o->substep++;
        }
        FUN_8001fa88(o, o->animFrame);
        break;
    case 9:
        if (D_1F8001C8 == 0)
            t = X(o)->wc4 - o->h->p.whole;
        else
            t = X(o)->wc8 - o->h->p.whole;
        if ((unsigned short)(t + 8) < 0x10) {
            o->timer = 300;
            o->substep++;
            break;
        }
        a.x = o->h->p.whole;
        if (D_1F8001C8 == 0)
            b.x = X(o)->wc4;
        else
            b.x = X(o)->wc8;
        a.y = o->y.p.whole;
        b.y = X(o)->wc6 - 0x38;
        if (b.x < a.x) {
            o->animFrame = 1;
            o->h->p.whole -= 2;
        } else {
            o->animFrame = 0;
            o->h->p.whole += 2;
        }
        o->y.raw += (short)FUN_8001fdac((FUN_8002078c(a, b) + 0x100) & 0xf8, 0x200) << 8;
        TICK
        break;
    case 10:
        o->y.raw += ((X(o)->wc6 << 16) - o->y.raw) >> 6;
        if (--U16(o->wbc) == 0) {
            o->wbc = 0x28;
            if (X(o)->wca) SfxPlay(0x12);
        }
        if ((unsigned short)(X(o)->wc6 - o->y.p.whole + 2) < 4) {
            if (FUN_80041ebc(o, o->h->p.whole, o->y.p.whole + 0x18)) {
                o->wb4 = 0;
                o->state = D_80131128[Rand() & 0xf];
                o->substep = 0;
                if (o->d->p.whole == D_800A607C->p.whole &&
                    (unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x80) < 0x100 &&
                    (unsigned short)(D_800A604E - o->y.p.whole + 0x30) < 0xf8) {
                    o->state = 4;
                    o->substep = 0;
                }
                break;
            }
        } else if (--o->timer) {
            break;
        }
        o->state = 3;
        o->substep = 0;
        break;
    }
}
