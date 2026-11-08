// VERSION SLPS_01144 dspsub 800F24DC 272
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800f0e0c (subScreenMapDrawAreaName.c)
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern int D_80108F1C[], D_801091F4[];
extern int D_80108F70, D_80108F84;
extern char D_8010580C[];
extern int D_801113E8;
extern Text D_80111408;
extern void func_800ED0DC(int pri);

void func_800F24DC(Menu *m, int i)
{
    D_80111408.x = 0x38;
    D_80111408.y = 0x18;
    if (GAME == 0xE) D_80111408.str = D_80108F1C[GAME_SELECTED_SECTION + GAME];
    else D_80111408.str = D_801091F4[i];
    switch (D_801113E8) {
    case 0x26:
    case 0x27:
    case 0x2E:
        D_80111408.str = D_80108F70;
        break;
    case 0xC:
        D_80111408.str = D_80108F84;
        break;
    }
    D_80111408.w = m->w10 + 1;
    D_80111408.attr = 0x7C15;
    D_80111408.p14 = D_8010580C;
    func_800ED0DC(5);
}
#endif
