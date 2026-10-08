// FUNC 801183f0 256 X006
/* score 29: whole function (covers csv piece 80118440). Only the loop head differs: the game computes the entry
   address tbl + i*8 twice (addu a0 / addu a1, both v1+v0) and reads the next entry as 8(a0); every C form tried
   either CSEs the two addresses into one register (p[1] vs p[0]: 37) or keeps a separate +8 addiu (this form).
   Tried: p/q copies, p++ (31), index temps of each type, inline/macro helpers, goto loop, -fno-* cse/loop flags. */
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } V8;
typedef struct { short w0, w2, w4, w6; int d8, dc; } E;
extern int SquareRoot0(int);
extern int ratan2(int, int);

void func_801183F0(TObj *o)
{
    E *e = (E *)&o->wb4;
    V8 *p, *q;
    int i;
    int dx, dy, dz, d;

    for (;;) {
        i = o->animTimer;
        p = (V8 *)*(int *)&o->wa8 + (i + 1);
        q = (V8 *)*(int *)&o->wa8 + i;
        dx = p->x - q->x;
        dz = p->z - q->z;
        dy = p->y - q->y;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d != 0) {
            e->dc = d << 8;
            if (e->d8 < d << 8) break;
            e->d8 -= d << 8;
        }
        o->animTimer++;
    }
    e->w6 = ratan2(dy, d);
    o->w76 = dy;
    e->w4 = ratan2(dz, dx);
}
