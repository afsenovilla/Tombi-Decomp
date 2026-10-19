// FUNC 800ee37c 516 D0SPSUB
// wip: 24 differing insns (Pt load order/regs, *e reload placement). D_8010A2D4-> (not [0].) avoids CSE related-value base
typedef struct { unsigned short x, y, z, pad; } Pt;
typedef struct { unsigned char a, b, c, d, e, f; } Link;
typedef struct { int x, y, z; } Vec;
typedef struct {
    unsigned char b00, p01, b02, b03;
    char p04[4];
    short w08;
    unsigned char b0a, p0b, b0c, b0d, p0e;
    signed char b0f;
    union { struct { int x, y, z; } s; Vec v; } p;
    char p1c[2];
    short w1e;
    char p20[4];
    void *anim;
    char p28[6];
    short w2e;
    union { struct { int x, y, z; } s; Vec v; } q;
    int d3c;
    char p40[0x60];
    char *da0;
    unsigned char ba4;
    char pa5[0xF];
    unsigned short wb4;
    short wb6, wb8, wba;
    char pbc[0x18];
} Sprite;
typedef struct { unsigned short room, sub; } RS;
extern Sprite D_8010A2D4[];
extern volatile int D_1F8002EC;
extern RS D_8009C960;
extern void *D_800FECE4;
extern short *D_800FE064[];
extern Link D_800FDAB4[];
extern Pt D_800FD418[];
extern int D_80111124;

Sprite *func_800EE37C(void)
{
    short *e;
    Link *l;
    D_8010A2D4->w1e = 1;
    D_8010A2D4->b0d = 1;
    D_8010A2D4->w08 = 0x78D2;
    D_8010A2D4->b0f = -0x1E;
    D_8010A2D4->b0a = 1;
    D_8010A2D4->w2e = 1;
    D_8010A2D4->anim = D_800FECE4;
    D_8010A2D4->d3c = D_1F8002EC;
    e = &D_800FE064[D_8009C960.room][D_8009C960.sub];
    l = &D_800FDAB4[*e];
    {
        int a = l->a, b = l->b;
        short xa = D_800FD418[a].x + 8, xb = D_800FD418[b].x + 8;
        short ya = D_800FD418[a].y + 2, yb = D_800FD418[b].y + 2;
        unsigned short za = D_800FD418[a].z, zb = D_800FD418[b].z;
        D_8010A2D4->p.s.x = (xa + xb) / 2 << 16;
        D_8010A2D4->p.s.y = (ya + yb) / 2 << 16;
        D_8010A2D4->p.s.z = ((short)za + (short)zb) / 2 << 16;
    }
    D_8010A2D4->q.v = D_8010A2D4->p.v;
    D_8010A2D4->wb4 = 0;
    D_80111124 = *e;
    D_8010A2D4->wb6 = l->c;
    D_8010A2D4->wb8 = l->d;
    D_8010A2D4->wba = 1;
    return D_8010A2D4;
}
