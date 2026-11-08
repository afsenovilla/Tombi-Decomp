// FUNC 800e90d0 388 O0PTSUB0
// MATCHING 800e90d0 388
// VERSION SCUS_94236 optsub00 800E83EC 388
#if defined(VERSION_SCUS_94236)
// USA version of PAL o0ptsub0 800E90D0 (optionsMenuDrawText.c): PAL C of src/o0ptsub0/optionsMenuDrawText.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef OPTIONS_MENU_SPRITES[];
extern unsigned short D_8009D612;
extern short OPTIONS_PAD_HAS_ACTUATOR;
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);
extern void optionsDrawLabel(short x, short y, int i);
extern void optionsDrawLabelPal2(short x, short y, int i);

void optionsMenuDrawText(int sound, int vib)
{
    SprDef *d;
    short tp;
    tp = GetTPage(0, 0, 0x200, 0);
    d = OPTIONS_MENU_SPRITES;
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    optionsDrawLabel(0x88, 0xe, 0);
    optionsDrawLabel(0x50, 0x2c, 1);
    optionsDrawLabel(0xB0, 0x2c, sound + 6);
    optionsDrawLabel(0x50, 0x44, 2);
    optionsDrawLabel(0xB0, 0x44, vib + 9);
    optionsDrawLabelPal2(0x60, 0xb0, 0x14);
    optionsDrawLabelPal2(0x60, 0xc0, 0x16);
    if (OPTIONS_PAD_HAS_ACTUATOR != 0) {
        optionsDrawLabel(0x50, 0x5c, 3);
        optionsDrawLabel(0xB0, 0x5c, D_8009D612 + 0xB);
        optionsDrawLabel(0x50, 0x74, 4);
        optionsDrawLabel(0x50, 0x8c, 5);
    } else {
        optionsDrawLabel(0x50, 0x5c, 4);
        optionsDrawLabel(0x50, 0x74, 5);
    }
}
#else
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef OPTIONS_MENU_SPRITES[];
extern unsigned short D_8009D612;
extern short OPTIONS_PAD_HAS_ACTUATOR;
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);
extern void optionsDrawLabel(short x, short y, int i);
extern void optionsDrawLabelPal2(short x, short y, int i);

void optionsMenuDrawText(int sound, int vib)
{
    SprDef *d;
    short tp;
    tp = GetTPage(0, 0, 0x200, 0);
    d = OPTIONS_MENU_SPRITES;
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    optionsDrawLabel(0x88, 0x1E, 0);
    optionsDrawLabel(0x50, 0x3C, 1);
    optionsDrawLabel(0xB0, 0x3C, sound + 6);
    optionsDrawLabel(0x50, 0x54, 2);
    optionsDrawLabel(0xB0, 0x54, vib + 9);
    optionsDrawLabelPal2(0x60, 0xC0, 0x14);
    optionsDrawLabelPal2(0x60, 0xD0, 0x16);
    if (OPTIONS_PAD_HAS_ACTUATOR != 0) {
        optionsDrawLabel(0x50, 0x6C, 3);
        optionsDrawLabel(0xB0, 0x6C, D_8009D612 + 0xB);
        optionsDrawLabel(0x50, 0x84, 4);
        optionsDrawLabel(0x50, 0x9C, 5);
    } else {
        optionsDrawLabel(0x50, 0x6C, 4);
        optionsDrawLabel(0x50, 0x84, 5);
    }
}
#endif
