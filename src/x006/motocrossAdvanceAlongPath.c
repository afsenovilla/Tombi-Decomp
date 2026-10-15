// FUNC 801184f0 380 X006
// MATCHING 801184f0 380
/* Path-point pair in the backward loop: the game keeps o->pa8 in v1 and a/b in a0/a1, which needs
   b to be a global pseudo (multi-set) so local-alloc cannot tie it with the dying pa8 temp: b is
   reused for the dx argument of the last ratan2 (plain C, no debt). The xp copy before the loop
   gives the game's `addiu a0,s0,0xb4` + `move s2,a0`. */
typedef struct { short x, y, z, pad; } P8;
typedef struct { short b4, b6, b8, ba; int bc; unsigned int c0; int c4; } X;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x2c - 8];
    unsigned short w2c;
    char p2e[0xa8 - 0x2e];
    P8 *pa8;
    char pac[0xb4 - 0xac];
    X x;
} O;
extern int SquareRoot0(int);
extern short ratan2(int, int);
extern void motocrossFindPathSegment(O *);

void motocrossAdvanceAlongPath(O *o)
{
    X *xp;
    X *x = &o->x;
    int v;
    int dx, dz, dy, d;
    int a, b;
    int i;

    v = x->bc + x->b6;
    x->bc = v;
    if (x->c0 < (unsigned int)v) {
        if (v < 0) {
            xp = x;
            if (o->w2c == 0) {
                xp->bc = 0;
                return;
            }
            xp->c4 -= xp->c0;
            o->w2c = o->w2c - 1;
            for (;;) {
                i = o->w2c << 3;
                a = (int)o->pa8 + i;
                b = (int)o->pa8 - -i;
                dx = ((P8 *)a)[1].x - ((P8 *)b)->x;
                dz = ((P8 *)a)[1].z - ((P8 *)b)->z;
                dy = ((P8 *)a)[1].y - ((P8 *)b)->y;
                d = SquareRoot0(dx * dx + dz * dz);
                if (d != 0) {
                    xp->c0 = d << 8;
                    xp->bc += d << 8;
                    if (xp->bc >= 0) break;
                }
                o->w2c--;
            }
            xp->ba = ratan2(dy, d);
            b = dx;
            xp->b8 = ratan2(dz, b);
        } else {
            x->bc = v - x->c0;
            o->w2c++;
            motocrossFindPathSegment(o);
            x->c4 += x->c0;
        }
    }
}
