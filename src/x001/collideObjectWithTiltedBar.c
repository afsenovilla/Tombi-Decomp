// FUNC 801253ac 1212 X001
// MATCHING 801253ac 1212
#include "spad.h"
/* Same family as x010 func_80121534 (template): box sums go through ac/as (ac = box0 + dx), the angle base reuses ac
   with a (short)(ang - ac) cast, case sums written `ac = X + 6; ac = box0 + ac;` (case 1 cross-jumps into case 3);
   ang priority bump at the top gives the s3/s5/s4 order; d0 pinned v1 with v0-pinned temps keeps d (a0) a copy. */
#include "TOBJ.H"

extern unsigned char D_8013C7B0[];
static __inline__ short sabs(short x)
{
    short r = x;
    if (x < 0) r = -x;
    return r;
}
extern int rcos(int);
extern int rsin(int);
extern int resolveHitResultB(TObj *, TObj *);
extern void playSFX(int);
extern void spawnImpactEffect(int, int, int, int);

int collideObjectWithTiltedBar(TObj *a, TObj *b, unsigned char k, int ang)
{
    short dx, dy, c, s, ac, as;
    int q, r;
    short kind;
    int d; register int d0 asm("$3");
    asm("" : "=r"(ang) : "0"(ang));

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = a->h->p.whole - b->h->p.whole;
    ac = b->box0 + dx;
    if (b->box1 < (unsigned short)ac) return 0;
    dy = a->y.p.whole - b->y.p.whole;
    as = b->box0 + dy;
    if (b->box1 < (unsigned short)as) return 0;
    switch (k) {
    case 1:
    case 2:
        q = dy < 0 ? 0xc00 : 0;
        ac = q;
        if (dx > 0) {
            if (q == 0) ac = 0x400;
            else ac = q - 0x400;
        }
        if ((unsigned)(((short)(ang - ac) & 0xfff) - 0x600) > 0x700) return 0;
    case 0:
        c = ((unsigned)(rcos(ang) * b->box0)) >> 12;
        s = ((unsigned)(rsin(ang) * b->box0)) >> 12;
        break;
    }
    ac = c;
    as = s;
    if (c < 0) ac = -c;
    if (s < 0) as = -s;
    kind = ac < as;
    if (ac == 0) kind = 3;
    else if (as == 0) kind = 2;
    switch (kind) {
    case 0:
        if (ac < sabs(dx)) return 0;
        as = (a->y.p.whole - (b->y.p.whole - dx * s / c)) + 6; as = a->box2 + as;
        if ((unsigned short)as > a->box3 + 0xc) return 0;
        break;
    case 1:
        if (as < sabs(dy)) return 0;
        ac = (a->h->p.whole - (b->h->p.whole - dy * c / s)) + 6; ac = a->box0 + ac;
        if ((unsigned short)ac > a->box1 + 0xc) return 0;
        break;
    case 2:
        as = (a->y.p.whole - b->y.p.whole) + 6; as = a->box2 + as;
        if ((unsigned short)as > a->box3 + 0xc) return 0;
        break;
    case 3:
        ac = (a->h->p.whole - b->h->p.whole) + 6; ac = a->box0 + ac;
        if ((unsigned short)ac > a->box1 + 0xc) return 0;
        break;
    }
    { register int t asm("$2") = ang >> 4; d0 = t & 0xff; }
    d = d0;
    if (k == 0) {
        if (kind & 1) {
            if (s > 0) {
                if (dy > 0) { register int t asm("$2") = d0 + 0x80; d = t & 0xff; }
            } else {
                if (dy < 0) { register int t asm("$2") = d0 + 0x80; d = t & 0xff; }
            }
        } else {
            if (c > 0) {
                if (dx < 0) { register int t asm("$2") = d0 + 0x80; d = t & 0xff; }
            } else {
                if (dx > 0) { register int t asm("$2") = d0 + 0x80; d = t & 0xff; }
            }
        }
    }
    if ((unsigned char)(D_8013C7B0[a->animFrame & 7] - d) < 0x80) b->velH = 0x600;
    else b->velH = -0x600;
    r = resolveHitResultB(a, b);
    if (r) {
        if (r >= 4) {
            if (b->velH > 0) b->velH += 0x400;
            else b->velH -= 0x400;
        } else if (r >= 2) {
            if (b->velH > 0) b->velH += 0x200;
            else b->velH -= 0x200;
        }
        playSFX(5);
        spawnImpactEffect(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
    }
    SPAD.s19e = 0;
    return 1;
}
