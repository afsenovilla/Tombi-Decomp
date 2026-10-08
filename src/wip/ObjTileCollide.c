// FUNC 8003fd78 464 MAIN0
/* score 4: only 'move s4,a1' / 'move s5,a2' swapped (game copies keep first, dy after the lbu b9c). With int dy the param moves sit before FUNCTION_BEG and sched2 orders them by luid (dy first). short dy (or K&R short) fixes the moves but then sched1 puts sh box3 before lbu b9c and both share v0 (33). Tried: perms of the zero stores/box3 store, volatile b9c read + volatile box3 store (4, move s4 lands in beqz slot), TObj *p copies, all int/short combos of params/locals, K&R decl. b41: cc1 -dS shows why: int params copies 4,6,8 sit at block head and sched1 skips them, so sched2 orders moves by luid (a1 first). short dy makes combine leave a deleted note between copies, so sched1 schedules them (boosted, near the beqz) and the lbu b9c lands between them after sh box3 -> v0 shared. box3 store written inside both if branches gives 15 (sb be takes the delay slot). Perms of stores+c=o->b9c temp with short dy: best 10. */
/* b52: sched2 dump: moves 6 (a1) and 8 (a2) both priority 1, tie broken by luid (higher luid picked first = placed later), so the keep copy needs a lower luid or dy's move a higher priority. A body copy `int dy = dy0;` puts the moves in the right order but sched1 then hoists sh box3 before lbu b9c (33; store perms 10). */
#include "TOBJ.H"

extern unsigned short D_80115320[];
extern short TileCollideAt(TObj *, short, short);
extern void FUN_8003fb90(TObj *);

short ObjTileCollide(TObj *o, int dy, int keep)
{
    unsigned short *t;
    int d1, d2;
    unsigned short v;
    short r;

    t = &D_80115320[((short *)o->anim)[1] * 4];
    o->box0 = *t++;
    o->box1 = *t++;
    o->box2 = *t++;
    v = *t;
    o->b69 = 0;
    o->wb0 = 0;
    *(unsigned char *)&o->da0 = 0;
    o->bbe = 0;
    o->box3 = v;
    if (o->b9c != 0) {
        if (o->velX >= 0) { d1 = 8; d2 = -8; }
        else { d1 = -8; d2 = 8; }
    } else {
        if (o->velH >= 0) { d1 = 8; d2 = -8; }
        else { d1 = -8; d2 = 8; }
    }
    r = TileCollideAt(o, o->h->p.whole, dy + (o->y.p.whole + o->box2));
    if (r == 0) {
        r = TileCollideAt(o, o->h->p.whole + d1, dy + (o->y.p.whole + o->box2));
        if (r == 0) {
            r = TileCollideAt(o, o->h->p.whole + d2, dy + (o->y.p.whole + o->box2));
            if (r == 0) return 0;
        }
    }
    if (keep == 0) o->y.p.whole += dy;
    FUN_8003fb90(o);
    return r;
}
