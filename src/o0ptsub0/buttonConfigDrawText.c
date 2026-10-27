// FUNC 800e9254 696 O0PTSUB0
// MATCHING 800e9254 696
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef BUTTON_CONFIG_SPRITES[];
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);
extern void optionsDrawLabel(short x, short y, int i);
extern void optionsDrawLabelPal2(short x, short y, int i);

void buttonConfigDrawText(unsigned int sel)
{
    SprDef *d;
    short tp;
    switch (sel) {
    case 0:
        optionsDrawLabelPal2(0x20, 0x54, 0x18);
        optionsDrawLabelPal2(0xE4, 0x3C, 0x19);
        optionsDrawLabelPal2(0xF4, 0x5C, 0x1A);
        optionsDrawLabelPal2(0xE4, 0x9C, 0x1B);
        optionsDrawLabelPal2(0x54, 0x84, 0x1C);
        break;
    case 1:
        optionsDrawLabel(0x38, 0x50, 0xF);
        optionsDrawLabelPal2(0xE4, 0x3C, 0x1B);
        optionsDrawLabelPal2(0xF4, 0x5C, 0x19);
        optionsDrawLabelPal2(0xE4, 0x9C, 0x1C);
        optionsDrawLabelPal2(0x54, 0x84, 0x1A);
        break;
    case 2:
        optionsDrawLabel(0x38, 0x50, 0x10);
        optionsDrawLabelPal2(0xE4, 0x3C, 0x19);
        optionsDrawLabelPal2(0xF4, 0x5C, 0x1C);
        optionsDrawLabelPal2(0xE4, 0x9C, 0x1B);
        optionsDrawLabelPal2(0x54, 0x84, 0x1A);
        break;
    case 3:
        optionsDrawLabel(0x38, 0x50, 0x11);
        optionsDrawLabelPal2(0xE4, 0x3C, 0x19);
        optionsDrawLabelPal2(0xF4, 0x5C, 0x1A);
        optionsDrawLabelPal2(0xE4, 0x9C, 0x1C);
        optionsDrawLabelPal2(0x54, 0x84, 0x1B);
        break;
    case 4:
        optionsDrawLabel(0x38, 0x50, 0x12);
        optionsDrawLabelPal2(0xE4, 0x3C, 0x1B);
        optionsDrawLabelPal2(0xF4, 0x5C, 0x1C);
        optionsDrawLabelPal2(0xE4, 0x9C, 0x1A);
        optionsDrawLabelPal2(0x54, 0x84, 0x19);
        break;
    case 5:
        optionsDrawLabel(0x38, 0x50, 0x13);
        optionsDrawLabelPal2(0xE4, 0x3C, 0x1C);
        optionsDrawLabelPal2(0xF4, 0x5C, 0x1B);
        optionsDrawLabelPal2(0xE4, 0x9C, 0x1A);
        optionsDrawLabelPal2(0x54, 0x84, 0x19);
        break;
    }
    tp = GetTPage(0, 0, 0x200, 0);
    d = BUTTON_CONFIG_SPRITES;
    while (d->x != -1) {
        titleDrawSprite(d++, tp, 0);
    }
    optionsDrawLabel(0x74, 0x28, 5);
    optionsDrawLabelPal2(0x60, 0xC0, 0x14);
    optionsDrawLabelPal2(0x60, 0xD0, 0x16);
    optionsDrawLabelPal2(0x20, 0x40, 0x17);
}
