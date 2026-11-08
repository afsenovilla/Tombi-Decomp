// VERSION SLPS_01144 dspsub 800F25EC 240
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800f0f20 (subScreenMapDrawRoomName.c)
typedef struct { char p[0x10]; short w10; } Menu;
typedef struct { int a, b, c; } Tri;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern int *D_801091DC;
extern int *D_80109350[];
extern Tri *D_80109358;
extern char D_8010580C[];
extern int D_801113E8;
typedef struct { short x, y, w; unsigned short attr; int img, pC, str; void *p14; } Text;
extern Text D_80111408;
extern void func_800ED0DC(int pri);

void func_800F25EC(Menu *m, int i, int j)
{
    if (i == 5) D_80111408.x = 0x96;
    else D_80111408.x = 0x86;
    D_80111408.y = 0x18;
    if (GAME == 0xE) D_80111408.str = D_801091DC[GAME_SELECTED_SECTION];
    else D_80111408.str = D_80109350[i][j];
    if (D_801113E8 == 0xC) { register Tri *t asm("v1") = D_80109358; D_80111408.str = t->c; }
    D_80111408.w = m->w10 + 1;
    D_80111408.attr = 0x7C15;
    D_80111408.p14 = D_8010580C;
    func_800ED0DC(5);
}
#endif
