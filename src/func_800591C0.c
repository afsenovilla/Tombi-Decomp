// FUNC 800591c0 676 MAIN0
// MATCHING 800591c0 676
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    short w, h;
} SPRT;
typedef struct { short x, y; short pad[8]; } DRAWENV_;
typedef struct { short n, pad, x, y; } GA;
typedef struct { unsigned short f; short pad, x, y; } GE;
typedef struct { unsigned char u, v, w, h, b4, b5, pad[4]; } GT;
extern GA D_800A4648[];
extern GE D_800A4DE0[][128];
extern GT D_800A5DE4[];
extern SPRT *DAT_1f800164;
/* do/while(0) macro: its loop note doubles the ref weight of p, flipping the p/index global-alloc priority (s0/s1) */
/* Tried (debt2): plain stores give p 28 refs/60 insns vs index offset 18/38 (needs one more p ref or index live >=39); comma macros, if/else inversions, block-local p/q copies, continue->if/else/goto, local types all leave the counts unchanged (score 44). */
#define SETUV0(p, u, v) do { (p)->u0 = (u); (p)->v0 = (v); } while (0)
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
            { p->y0 = D_800A4648[a].y + D_800A4DE0[a][i].y - 2; }
            else
            {    p->y0 = D_800A4648[a].y + D_800A4DE0[a][i].y; }
        } else {
            p->x0 = D_800A4648[a].x + D_800A4DE0[a][i].x - 0xb;
            p->y0 = D_800A4648[a].y + D_800A4DE0[a][i].y - 10;
        }
        SETUV0(p, D_800A5DE4[f].u << 2, D_800A5DE4[f].v);
        p->w = D_800A5DE4[f].w;
        p->h = D_800A5DE4[f].h;
        p->clut = GetClut(0x160, 0x1e3);
        AddPrim(DAT_1f8001e0 + 4, p);
        DAT_1f800164++;
    }
    AddDrawMode(0, 1);
}
