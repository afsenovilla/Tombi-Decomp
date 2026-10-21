// FUNC 800e950c 408 O0PTSUB0
// MATCHING 800e950c 408
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef D_800EB3BC[];
extern short D_8009D4FC, D_8009D4FE;
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);
extern void optionsDrawLabel(short x, short y, int i);
extern void optionsDrawLabelPal2(short x, short y, int i);
extern void optionsDrawLabelPal(short x, short y, int i, int c);
extern void screenAdjustDrawNumber(short x, short y, int n);

void screenAdjustDrawText(int dir)
{
    SprDef *d;
    short tp;
    tp = GetTPage(0, 0, 0x200, 0);
    d = D_800EB3BC;
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    optionsDrawLabel(0x68, 0x20, 4);
    optionsDrawLabel(0xA0, 0x60, 0xD);
    optionsDrawLabel(0xA0, 0x80, 0xE);
    optionsDrawLabelPal(0x20, 0x70, 0x1D, dir == 3 ? 3 : 0);
    optionsDrawLabelPal(0x60, 0x70, 0x1E, dir == 4 ? 3 : 0);
    optionsDrawLabelPal(0x40, 0x50, 0x1F, dir == 1 ? 3 : 0);
    optionsDrawLabelPal(0x40, 0x90, 0x20, dir == 2 ? 3 : 0);
    screenAdjustDrawNumber(0x100, 0x60, D_8009D4FC - 4);
    screenAdjustDrawNumber(0x100, 0x80, D_8009D4FE - 0xC);
    optionsDrawLabelPal2(0x60, 0xC0, 0x16);
    optionsDrawLabelPal2(0x60, 0xD0, 0x15);
}
