// VERSION SLPS_01144 dspsub3 800EB398 504
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800e9ab0 (subScreenDrawStatusNotes.c), from the USA one
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern int D_8009E52C;
extern unsigned char D_8009E954, D_8009E959, D_8009E9E5;
extern char D_800FAABC[];
extern int D_800FE67C[];
extern int D_800FE680, D_800FE694, D_800FE698;
extern Text D_800FF57C;
extern void func_800ED024(int pri);

void func_800EB398(Menu *m)
{
    int n;
    if (D_8009E52C & 0x40) {
        D_800FF57C.x = 0xB0;
        D_800FF57C.y = 0x60;
        D_800FF57C.w = m->w10 + 1;
        D_800FF57C.attr = 0x7C15;
        D_800FF57C.p14 = D_800FAABC;
        D_800FF57C.str = D_800FE680;
        func_800ED024(5);
    }
    n = D_8009E959;
    if (n >= 2) {
        D_800FF57C.x = 0xB0;
        D_800FF57C.y = 0x6F;
        D_800FF57C.w = m->w10 + 1;
        D_800FF57C.attr = 0x7C15;
        if (n == 0xFF) n = 5;
        D_800FF57C.str = D_800FE67C[n];
        D_800FF57C.p14 = D_800FAABC;
        func_800ED024(5);
    }
    if (D_8009E954 == 0xFF) {
        D_800FF57C.x = 0xB0;
        D_800FF57C.y = 0x7E;
        D_800FF57C.w = m->w10 + 1;
        D_800FF57C.attr = 0x7C15;
        D_800FF57C.p14 = D_800FAABC;
        D_800FF57C.str = D_800FE694;
        func_800ED024(5);
    }
    if (D_8009E9E5 == 0xFF) {
        D_800FF57C.x = 0xB0;
        D_800FF57C.y = 0x8D;
        D_800FF57C.w = m->w10 + 1;
        D_800FF57C.attr = 0x7C15;
        D_800FF57C.p14 = D_800FAABC;
        D_800FF57C.str = D_800FE698;
        func_800ED024(5);
    }
}
#endif
