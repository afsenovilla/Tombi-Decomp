// VERSION SLPS_01144 dspsub3 800ED6E0 224
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ec390 (subScreenInitMenu.c)
typedef struct { short x, y, w, h; } RECT;
typedef struct { char p[0x10]; short w10; char p12[0xc]; short w1e; void *p20; char p24[0x24]; void *p48, *p4c, *p50, *p54, *p58, *p5c, *p60, *p64; } Menu;
extern char D_800EFE28[], D_800F0A28[], D_800F8E88[], D_800F9598[], D_800F95D0[], D_800F95B8[], D_800F95C4[], D_800F95E0[], D_800F9644[], D_800F9970[], D_800F9670[];
extern void *D_1F8003A0;
extern void LoadImage(RECT *r, void *p);

void func_800ED6E0(Menu *m)
{
    RECT r;
    r.x = 0x120;
    r.y = 0x1E0;
    r.w = 0x30;
    r.h = 0x1F;
    LoadImage(&r, D_800EFE28);
    D_1F8003A0 = D_800F0A28;
    m->p20 = D_800F8E88;
    m->p48 = D_800F9598;
    m->p4c = D_800F95D0;
    m->p50 = D_800F95B8;
    m->p54 = D_800F95C4;
    m->p58 = D_800F95E0;
    m->p5c = D_800F9644;
    m->p60 = D_800F9970;
    m->p64 = D_800F9670;
    m->w10 = 1;
    m->w1e = -1;
}
#endif
