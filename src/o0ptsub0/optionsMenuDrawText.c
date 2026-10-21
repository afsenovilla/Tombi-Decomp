// FUNC 800e90d0 388 O0PTSUB0
// MATCHING 800e90d0 388
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef D_800EB1DC[];
extern unsigned short D_8009D612;
extern short D_800EB480;
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);
extern void optionsDrawLabel(short x, short y, int i);
extern void optionsDrawLabelPal2(short x, short y, int i);

void optionsMenuDrawText(int sound, int vib)
{
    SprDef *d;
    short tp;
    tp = GetTPage(0, 0, 0x200, 0);
    d = D_800EB1DC;
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
    if (D_800EB480 != 0) {
        optionsDrawLabel(0x50, 0x6C, 3);
        optionsDrawLabel(0xB0, 0x6C, D_8009D612 + 0xB);
        optionsDrawLabel(0x50, 0x84, 4);
        optionsDrawLabel(0x50, 0x9C, 5);
    } else {
        optionsDrawLabel(0x50, 0x6C, 4);
        optionsDrawLabel(0x50, 0x84, 5);
    }
}
