// FUNC 800e96a4 136 O0PTSUB0
// MATCHING 800e96a4 136
typedef struct { short x, y, u, v, w, h, cx, cy; } SprDef;
extern SprDef OPTIONS_FRAME_SPRITES[];
extern unsigned short GetTPage(int tp, int abr, int x, int y);
extern void titleDrawSprite(SprDef *d, short tpage, unsigned char pri);

void optionsDrawFrame(void)
{
    SprDef *d;
    short tp;
    tp = GetTPage(1, 0, 0x180, 0);
    d = OPTIONS_FRAME_SPRITES;
    while (d->x != -1) {
#ifdef PORT2
        {   /* widescreen: the checkered wallpaper (80-px tiles, four to a screen) repeats a screen to each side, so it
               fills the wide side strips too (the drawing area clips the rest) */
            extern int port_widescreen;
            if (port_widescreen) {
                SprDef c = *d;
                c.x = d->x - 320; titleDrawSprite(&c, tp, 4);
                c.x = d->x + 320; titleDrawSprite(&c, tp, 4);
            }
        }
#endif
        titleDrawSprite(d++, tp, 4);
    }
}
