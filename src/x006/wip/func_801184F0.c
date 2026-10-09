// FUNC 801184f0 380 X006
/* score 6: only the path-point pair in the backward loop: game keeps o->pa8 in v1 and computes a/b into a0/a1, ours
   ties a with the dying pa8 temp (a0). The backward branch is an inline taking x as void * (gives the game's
   addiu a0,s0,0xb4 + move s2,a0 copy); b before a and o->w2c = o->w2c - 1 fix the rest. Tried: user-var pa8 temp,
   operand orders, &o->pa8[w2c] forms. */
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
extern void func_801183F0(O *);

static __inline__ void back(O *o, void *xp)
{
    X *x = xp;
    int dx, dz, dy, d;
    P8 *a, *b;
    int i;

    x->c4 -= x->c0;
    o->w2c = o->w2c - 1;
    for (;;) {
        i = o->w2c << 3;
        b = (P8 *)((int)o->pa8 - -i);
        a = (P8 *)((int)o->pa8 + i);
        dx = a[1].x - b->x;
        dz = a[1].z - b->z;
        dy = a[1].y - b->y;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d != 0) {
            x->c0 = d << 8;
            x->bc += d << 8;
            if (x->bc >= 0) break;
        }
        o->w2c--;
    }
    x->ba = ratan2(dy, d);
    x->b8 = ratan2(dz, dx);
}

void func_801184F0(O *o)
{
    X *x = &o->x;
    int v;

    v = x->bc + x->b6;
    x->bc = v;
    if (x->c0 < (unsigned int)v) {
        if (v < 0) {
            if (o->w2c == 0) {
                x->bc = 0;
                return;
            }
            back(o, x);
        } else {
            x->bc = v - x->c0;
            o->w2c++;
            func_801183F0(o);
            x->c4 += x->c0;
        }
    }
}
