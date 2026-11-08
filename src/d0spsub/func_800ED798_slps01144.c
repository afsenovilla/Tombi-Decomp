// VERSION SLPS_01144 dspsub 800ED798 240
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ec390 (subScreenInitMenu.c)
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p[0x10]; short w10; char p12[0xc]; short w1e; void *p20; char p24[0x24]; void *p48, *p4c, *p50, *p54, *p58, *p5c, *p60, *p64; } Menu;
extern char D_800F2FB0[], D_800F3BB0[], D_800FBFC0[], D_80103BD8[], D_801042E8[], D_80104320[], D_80104308[], D_80104314[], D_80104330[], D_80104394[], D_801046C0[], D_801043C0[];
extern void *D_1F8003A0, *D_1F8003A4;
extern void LoadImage(RECT *r, void *p);

void func_800ED798(Menu *m)
{
    RECT r;
    r.x = 0x120;
    r.y = 0x1E0;
    r.w = 0x30;
    r.h = 0x1F;
    LoadImage(&r, D_800F2FB0);
    D_1F8003A0 = D_800F3BB0;
    D_1F8003A4 = D_800FBFC0;
    m->p20 = D_80103BD8;
    m->p48 = D_801042E8;
    m->p4c = D_80104320;
    m->p50 = D_80104308;
    m->p54 = D_80104314;
    m->p58 = D_80104330;
    m->p5c = D_80104394;
    m->p60 = D_801046C0;
    m->p64 = D_801043C0;
    m->w10 = 1;
    m->w1e = -1;
}
#endif
