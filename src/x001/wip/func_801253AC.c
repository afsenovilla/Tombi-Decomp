// FUNC 801253ac 1212 X001
/* score 308: first full draft from the asm (logic follows the game: box test, quadrant/angle check, rcos/rsin
   projections, abs compares, side selection, velH kick). Frame is 0x40 in the game vs 0x30 here (16 B of locals:
   probably inlined helpers) and the register allocation of the box pseudos and the |c|/|s| selector differ.
   o28: angle base is a copy (`base = q; if (dx > 0) { if (!q) base = 0x400; else base = q - 0x400; }` gives the
   game's move/bnez shape), ac/as both initialised before the sign tests. The quadrant selector in the game keeps
   r = ac < as (v1), q = r (a3) and switches on another copy (move v1,a3 at each join): an inline returning q gets
   the shape partly (score 311). */
#include "TOBJ.H"

extern unsigned char D_8013C7B0[];
extern short D_1F80019E;
extern int rcos(int);
extern int rsin(int);
extern int func_800429D0(TObj *, TObj *);
extern void playSFX(int);
extern void FUN_8001f96c(int, int, int, int);

int func_801253AC(TObj *a, TObj *b, unsigned char k, int ang)
{
    short dx, dy, c, s, ac, as;
    int base, q, r, v;
    unsigned char d;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = a->h->p.whole - b->h->p.whole;
    if ((unsigned short)(b->box0 + dx) > b->box1) return 0;
    dy = a->y.p.whole - b->y.p.whole;
    if ((unsigned short)(b->box0 + dy) > b->box1) return 0;
    switch (k) {
    case 1:
    case 2:
        q = dy < 0 ? 0xc00 : 0;
        base = q;
        if (dx > 0) {
            if (q) base = q - 0x400;
            else base = 0x400;
        }
        if ((unsigned)(((ang - base) & 0xfff) - 0x600) > 0x700) return 0;
    case 0:
        c = ((unsigned)(rcos(ang) * b->box0)) >> 12;
        s = ((unsigned)(rsin(ang) * b->box0)) >> 12;
        break;
    }
    ac = c;
    as = s;
    if (c < 0) ac = -c;
    if (s < 0) as = -s;
    q = ac < as;
    if (ac == 0) q = 3;
    else if (as == 0) q = 2;
    switch (q) {
    case 0:
        if (ac < (dx < 0 ? -dx : dx)) return 0;
        if ((unsigned short)(a->box2 + (a->y.p.whole - (b->y.p.whole - dx * s / c)) + 6) > a->box3 + 0xc) return 0;
        break;
    case 1:
        if (as < (dy < 0 ? -dy : dy)) return 0;
        if ((unsigned short)(a->box0 + (a->h->p.whole - (b->h->p.whole - dy * c / s)) + 6) > a->box1 + 0xc) return 0;
        break;
    case 2:
        if ((unsigned short)(a->box2 + (a->y.p.whole - b->y.p.whole + 6)) > a->box3 + 0xc) return 0;
        break;
    case 3:
        if ((unsigned short)(a->box0 + (a->h->p.whole - b->h->p.whole + 6)) > a->box1 + 0xc) return 0;
        break;
    }
    d = ang >> 4;
    if (k == 0) {
        if (q & 1) {
            if (s > 0) {
                if (dy > 0) d += 0x80;
            } else {
                if (dy < 0) d += 0x80;
            }
        } else {
            if (c > 0) {
                if (dx < 0) d += 0x80;
            } else {
                if (dx > 0) d += 0x80;
            }
        }
    }
    if ((unsigned char)(D_8013C7B0[a->animFrame & 7] - d) < 0x80) b->velH = 0x600;
    else b->velH = -0x600;
    r = func_800429D0(a, b);
    if (r) {
        if (r >= 4) {
            v = b->velH;
            if (v > 0) b->velH = v + 0x400;
            else b->velH = v - 0x400;
        } else if (r < 2) {
            v = b->velH;
            if (v > 0) b->velH = v + 0x200;
            else b->velH = v - 0x200;
        }
        playSFX(5);
        FUN_8001f96c(1, a->a.p.whole, a->y.p.whole, a->b.p.whole);
    }
    D_1F80019E = 0;
    return 1;
}
