// FUNC 800591c0 676 MAIN0
/* score 16 (b39): p/index s0/s1 swap is a global-alloc priority tie (-dg: p 28 refs/60 insns = 1.87, index 18/38 = 1.89).
   Writing p->u0 = ... in all three x/y branches (merged by cross-jumping) adds p refs and fixes the swap, but CSE then reuses the
   f*10 index from the b5 test for u0, while the game recomputes (short)f*10 after the merge. Other route: an explicit
   int k = (i<<3)+(a<<10) index fixes regs too (score 32) but CSE merges the lh test with the f load and combine leaves a
   (use) of a spilled pseudo -> frame +8. Earlier (r9/b18): register, decl order, pointer e, types, DAT_1f800164 = p + 1. */
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct { short x, y; short pad[8]; } DRAWENV_;
typedef struct { short n, x, y, pad; } GA;
typedef struct { unsigned short f; short x, y, pad; } GE;
typedef struct { unsigned char u, v, w, h, b4, b5, pad[4]; } GT;
extern GA D_800A4648[];
extern GE D_800A4DE0[][128];
extern GT D_800A5DE4[];
extern SPRT *DAT_1f800164;
extern char *DAT_1f8001e0;
typedef struct { short clip[4]; short ofs[2]; short tw[4]; unsigned short tpage; char rest[0x68 - 0x16]; } DRAWENV;
extern void GetDrawEnv(DRAWENV *env);
extern void SetSprt(SPRT *p);
extern unsigned short GetClut(int x, int y);
extern void AddPrim(void *ot, void *p);
extern void AddDrawMode(int a, int b);

void func_800591C0(int a)
{
    DRAWENV env;
    SPRT *p;
    int i;
    short f;

    GetDrawEnv(&env);
    AddDrawMode((short)env.tpage, 1);
    for (i = 0; i < D_800A4648[a].n; i++) {
        f = D_800A4DE0[a][i].f;
        if (*(short *)&D_800A4DE0[a][i].f & 0x8000) {
            D_800A4DE0[a][i].f = f & 0x7fff;
            continue;
        }
        p = DAT_1f800164;
        SetSprt(p);
        p->code |= 1;
        f &= 0xfff;
        if (D_800A5DE4[f].b5 == 0) {
            p->x0 = D_800A4648[a].x + D_800A4DE0[a][i].x;
            if (D_800A5DE4[f].w >= 0xb)
            { p->y0 = D_800A4648[a].y + D_800A4DE0[a][i].y - 2; p->u0 = D_800A5DE4[f].u << 2; }
            else
            {    p->y0 = D_800A4648[a].y + D_800A4DE0[a][i].y; p->u0 = D_800A5DE4[f].u << 2; }
        } else {
            p->x0 = D_800A4648[a].x + D_800A4DE0[a][i].x - 0xb;
            p->y0 = D_800A4648[a].y + D_800A4DE0[a][i].y - 10;
            p->u0 = D_800A5DE4[f].u << 2;
        }
        p->v0 = D_800A5DE4[f].v;
        p->w = D_800A5DE4[f].w;
        p->h = D_800A5DE4[f].h;
        p->clut = GetClut(0x160, 0x1e3);
        AddPrim(DAT_1f8001e0 + 4, p);
        DAT_1f800164++;
    }
    AddDrawMode(0, 1);
}
