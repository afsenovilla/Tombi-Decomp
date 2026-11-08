// VERSION SCUS_94236 optsub00 800E8570 272
#if defined(VERSION_SCUS_94236)
// USA version of PAL o0ptsub0 800e9254 (buttonConfigDrawText.c)
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef *BUTTON_CONFIG_SEL_SPRITES[];
extern SprDef BUTTON_CONFIG_SPRITES[];
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);
extern void optionsDrawLabel(short x, short y, int i);
extern void optionsDrawLabelPal2(short x, short y, int i);

void buttonConfigDrawText(unsigned int sel)
{
    SprDef *d;
    short tp;
    tp = GetTPage(0, 0, 0x200, 0);
    d = BUTTON_CONFIG_SEL_SPRITES[sel];
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    d = BUTTON_CONFIG_SPRITES;
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    optionsDrawLabelPal2(0x60, 0xB0, 0x14);
    optionsDrawLabelPal2(0x60, 0xC0, 0x16);
    optionsDrawLabelPal2(0x20, 0x48, 0x17);
}
#endif
