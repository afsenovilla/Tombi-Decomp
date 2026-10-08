// FUNC 80021b20 1016 MAIN0
// r11: score 162. Structure right (smax/smin inlines give the copies and frame). Left: reg alloc (w4c should be v1, q a1, off a3), clamp of F2/p34 block order, V3 struct copy at end unverified.
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
extern int D_1F8000EC[];
extern unsigned char D_8009CDA2;
extern void FUN_8002757c(C *);
#define FLAG (*(unsigned short *)0x1F8001C8)
#define SFLAG (*(short *)0x1F8001C8)

static __inline__ int smax(int a, short b) { short r = a; if (a < b) r = b; return r; }
static __inline__ int smin(short a, short b) { short r = a; if (b < a) r = b; return r; }

void func_80021B20(void)
{
    C *c;
    short *p, *q;
    int off, x;
    char pad[0x10];
    c = &D_800A4550;
    c->b0 = 1;
    p = (short *)&D_80078B7C[D_8009C960[0]][D_8009C962];
    D_800A457C = *p++;
    D_800A457E = *p++;
    D_800A4580 = *p++;
    D_800A4582 = *p;
    if (!(FLAG & 1)) {
        D_800A4584 = D_1F8000EC;
        D_800A4588 = D_1F8000EC + 2;
    } else {
        D_800A4588 = D_1F8000EC;
        D_800A4584 = D_1F8000EC + 2;
    }
    *(int *)0x1F8000E0 = 0;
    *(int *)0x1F8000E4 = 0;
    if ((D_8009C960[0] == 4 && D_8009C962 < 4) || *(int *)D_8009C960 == 0x50000)
        *(int *)0x1F8000E8 = 0xFDC00000;
    else
        *(int *)0x1F8000E8 = 0xFDE00000;
    q = D_80079474[D_8009C960[0]][D_8009C962] + D_8009C982 * 4;
    switch ((*(O **)0x1F8001D4)->w4c) {
    case 1: case 2:
        off = -0x38;
        break;
    case 4: case 5: case 6:
        off = 0;
        break;
    }
    if (!(FLAG & 1)) {
        *(int *)0x1F8000EC = smax(*q++, c->w2c) << 16;
        *(int *)0x1F8000F0 = smin(*q++ + off, c->w32) << 16;
        *(int *)0x1F8000F4 = *q << 16;
    } else {
        *(int *)0x1F8000EC = *q++ << 16;
        *(int *)0x1F8000F0 = smin(*q++ + off, c->w32) << 16;
        *(int *)0x1F8000F4 = smax(*q, c->w2c) << 16;
    }
    x = *(short *)0x1F8000F2;
    if (x < c->w30) *(int *)0x1F8000F0 = c->w30 << 16;
    else if (c->w32 < x) *(int *)0x1F8000F0 = c->w32 << 16;
    x = ((SP *)c->p34)->y;
    if (c->w2e < x) *c->p34 = c->w2e << 16;
    else if (x < c->w2c) *c->p34 = c->w2c << 16;
    if (D_8009CDA2) {
        c->d20 = 0x1000;
        if (FLAG & 1) c->d24 = -0x1000;
        else c->d24 = 0x1000;
        c->d78 = 0;
    } else {
        c->d20 = 0;
        c->d24 = 0;
        c->d78 = 0;
    }
    c->d7c = SFLAG * 0x5A00;
    FUN_8002757c(c);
    *(V3 *)0x1F800174 = c->pos;
    *(int *)0x1F800180 = c->pos.x;
    *(int *)0x1F800184 = c->pos.y + *(int *)0x1F8000E4;
    *(int *)0x1F800188 = c->pos.z;
}
