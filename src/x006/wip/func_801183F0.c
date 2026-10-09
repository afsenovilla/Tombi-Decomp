// FUNC 801183f0 256 X006
/* score 6: only registers of the path-point pointers differ: game has base in v1, a=base+off in a0, b in a1;
   ours ties base with a (a0) and b gets a1. b written as `base - -i` stops CSE merging a/b (old copy form scored 23).
   Tried: base/off temps (int, char ptr, P8 ptr; block or function scope), both statement orders, off - -base, a=b copy. */
/* o36: still 6. Hypothesis: game base pseudo is not tied to a/b in local-alloc (dies twice or non-local); tried base temps (int/volatile), a-first, multiset a/b, ushort index, if/else deaths: 6..74. */
typedef struct { short x, y, z, pad; } P8;
typedef struct { short b4, b6, b8, ba; int bc, c0, c4; } X;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x2c - 8];
    unsigned short w2c;
    char p2e[0x76 - 0x2e];
    short w76;
    char p78[0xa8 - 0x78];
    P8 *pa8;
    char pac[0xb4 - 0xac];
    X x;
} O;
extern int SquareRoot0(int);
extern short ratan2(int, int);

void func_801183F0(O *o)
{
    X *x = &o->x;
    int dx, dz, dy, d;
    P8 *a, *b;
    int i;

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
            if (x->bc < (d << 8)) break;
            x->bc -= d << 8;
        }
        o->w2c++;
    }
    x->ba = ratan2(dy, d);
    o->w76 = dy;
    x->b8 = ratan2(dz, dx);
}
