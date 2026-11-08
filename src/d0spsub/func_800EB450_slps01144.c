// VERSION SLPS_01144 dspsub 800EB450 504
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800e9ab0 (subScreenDrawStatusNotes.c), from the USA one
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern int D_8009E52C;
extern unsigned char D_8009E954, D_8009E959, D_8009E9E5;
extern char D_8010580C[];
extern int D_801093CC[];
extern int D_801093D0, D_801093E4, D_801093E8;
extern Text D_80111408;
extern void func_800ED0DC(int pri);

void func_800EB450(Menu *m)
{
    int n;
    if (D_8009E52C & 0x40) {
        D_80111408.x = 0xB0;
        D_80111408.y = 0x60;
        D_80111408.w = m->w10 + 1;
        D_80111408.attr = 0x7C15;
        D_80111408.p14 = D_8010580C;
        D_80111408.str = D_801093D0;
        func_800ED0DC(5);
    }
    n = D_8009E959;
    if (n >= 2) {
        D_80111408.x = 0xB0;
        D_80111408.y = 0x6F;
        D_80111408.w = m->w10 + 1;
        D_80111408.attr = 0x7C15;
        if (n == 0xFF) n = 5;
        D_80111408.str = D_801093CC[n];
        D_80111408.p14 = D_8010580C;
        func_800ED0DC(5);
    }
    if (D_8009E954 == 0xFF) {
        D_80111408.x = 0xB0;
        D_80111408.y = 0x7E;
        D_80111408.w = m->w10 + 1;
        D_80111408.attr = 0x7C15;
        D_80111408.p14 = D_8010580C;
        D_80111408.str = D_801093E4;
        func_800ED0DC(5);
    }
    if (D_8009E9E5 == 0xFF) {
        D_80111408.x = 0xB0;
        D_80111408.y = 0x8D;
        D_80111408.w = m->w10 + 1;
        D_80111408.attr = 0x7C15;
        D_80111408.p14 = D_8010580C;
        D_80111408.str = D_801093E8;
        func_800ED0DC(5);
    }
}
#endif
