// FUNC 80021b20 1016 MAIN0
// score 90 -> b29: 76 by full-address score (tools ncheck --score is blind to symbol offsets): final 6-int copy as extern int D_1F800174[] array (stores keep loads after them). scratchpad as D_1F8000E0[] array (keeps load order), shared-store clamps (v = w30; if (x < v || (v = w32, v < x)) store), d7c in both branches, final copy as 6 int stores. Left: a0/a2 swap in smax/smin inline copies (r vs b param copy), w4c reg (v1 vs a0) and q address order. b18 tried: 12 smax/smin inline forms (return-style, int r, copies), caller-scope macro vars (162+), D_8009C962 as D_8009C960[1] / D_8009C982 as array (no change), q split, hill-climb of the top block (no gain). b29: r/b pseudos: -dl shows b (3 refs/4 insns) outranks r (3 refs/7 insns) so b gets a0; ~15 more inline forms, caller-scope r/b (104+), q index forms all unchanged. b46: q computed before w (game loads D_1F8001D4 after D_8009C982) no gain (82 = current ncheck score); split q += idx*4 forms 113-118.
typedef struct { int x, y, z; } V3;
typedef struct { short x, y; } SP;
typedef struct { short a, b, c, d; } S4;
typedef struct {
    unsigned char b0; char p1[7]; V3 pos; char p14[0x20 - 0x14]; int d20; int d24;
    char p28[0x2c - 0x28]; short w2c, w2e, w30, w32; int *p34; char p38[0x78 - 0x38]; int d78; int d7c;
} C;
typedef struct { char p[0x4c]; unsigned short w4c; } O;
extern C D_800A4550;
extern unsigned short D_8009C960[];
extern unsigned short D_8009C962;
extern unsigned short D_8009C982;
extern S4 *D_80078B7C[];
extern short **D_80079474[];
extern short D_800A457C, D_800A457E, D_800A4580, D_800A4582;
extern int *D_800A4584, *D_800A4588;
extern int D_1F8000E0[];
extern int D_1F800174[];
extern unsigned char D_8009CDA2;
extern O *D_1F8001D4;
extern void FUN_8002757c(C *);
#define FLAG (*(unsigned short *)0x1F8001C8)
#define SFLAG (*(short *)0x1F8001C8)

static __inline__ int smax(int a, short b) { short r = a; if (a < b) r = b; return r; }
static __inline__ int smin(short a, short b) { short r = a; if (b < a) r = b; return r; }

void func_80021B20(void)
{
    C *c;
    short *p, *q;
    int off, x, v;
    unsigned short w;
    char pad[0x10];
    c = &D_800A4550;
    c->b0 = 1;
    p = (short *)&D_80078B7C[D_8009C960[0]][D_8009C962];
    D_800A457C = *p++;
    D_800A457E = *p++;
    D_800A4580 = *p++;
    D_800A4582 = *p;
    if (!(FLAG & 1)) {
        D_800A4584 = &D_1F8000E0[3];
        D_800A4588 = &D_1F8000E0[5];
    } else {
        D_800A4588 = &D_1F8000E0[3];
        D_800A4584 = &D_1F8000E0[5];
    }
    D_1F8000E0[0] = 0;
    D_1F8000E0[1] = 0;
    if ((D_8009C960[0] == 4 && D_8009C962 < 4) || *(int *)D_8009C960 == 0x50000)
        D_1F8000E0[2] = 0xFDC00000;
    else
        D_1F8000E0[2] = 0xFDE00000;
    w = D_1F8001D4->w4c;
    q = D_80079474[D_8009C960[0]][D_8009C962] + D_8009C982 * 4;
    switch (w) {
    case 1: case 2:
        off = -0x38;
        break;
    case 4: case 5: case 6:
        off = 0;
        break;
    }
    if (!(FLAG & 1)) {
        D_1F8000E0[3] = smax(*q++, c->w2c) << 16;
        D_1F8000E0[4] = smin(*q++ + off, c->w32) << 16;
        D_1F8000E0[5] = *q << 16;
    } else {
        D_1F8000E0[3] = *q++ << 16;
        D_1F8000E0[4] = smin(*q++ + off, c->w32) << 16;
        D_1F8000E0[5] = smax(*q, c->w2c) << 16;
    }
    x = *(short *)0x1F8000F2;
    v = c->w30;
    if (x < v || (v = c->w32, v < x))
        D_1F8000E0[4] = v << 16;
    v = c->w2e;
    if (v < ((SP *)c->p34)->y || (v = c->w2c, ((SP *)c->p34)->y < v))
        *c->p34 = v << 16;
    if (D_8009CDA2) {
        c->d20 = 0x1000;
        if (FLAG & 1) c->d24 = -0x1000;
        else c->d24 = 0x1000;
        c->d78 = 0;
        c->d7c = SFLAG * 0x5A00;
    } else {
        c->d20 = 0;
        c->d24 = 0;
        c->d78 = 0;
        c->d7c = SFLAG * 0x5A00;
    }
    FUN_8002757c(c);
    D_1F800174[0] = c->pos.x;
    D_1F800174[1] = c->pos.y;
    D_1F800174[2] = c->pos.z;
    D_1F800174[3] = c->pos.x;
    D_1F800174[4] = c->pos.y + D_1F8000E0[1];
    D_1F800174[5] = c->pos.z;
}
