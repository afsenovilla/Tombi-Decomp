// FUNC 800e9d08 596 O0PTSUB0
// MATCHING 800e9d08 596
// FLAGS -O2 -G0 -fno-strength-reduce
typedef struct S { char p[0x48]; short w48; unsigned short w4a; unsigned short w4c; unsigned short w4e; } S;
typedef struct { short x, y, tp, pad; void *str; void *font; void *pal; } MenuItem;
extern S *TASK_C;
extern short TITLE_MENU_GLYPHS[];
extern char TITLE_FONT[];
extern unsigned char *TITLE_MENU_GLYPH_PALETTES;
extern void *TITLE_ARROW_LEFT_STRING, *TITLE_ARROW_RIGHT_STRING, *TITLE_ARROW_LEFT_PALETTE, *TITLE_ARROW_RIGHT_PALETTE;
extern void titleDrawString(MenuItem *m, int pri, int wrap, int pal);
extern void titleDrawCarouselGlyph(int c, int ang, int off, unsigned char pal);

void titleDrawMenuCarousel(int angle, int sel)
{
    MenuItem m;
    short *s;
    short idx;
    unsigned char *pal;
    int ang;

    s = TITLE_MENU_GLYPHS;
    idx = -1;
    pal = TITLE_MENU_GLYPH_PALETTES;
    ang = angle;
    while (*s != -3) {
        if ((((short)ang + 0x20) & 0xFF) < 0x40 && idx == -1) idx = 0;
        if (*s != 0) {
            if ((((short)ang + 0x20) & 0xFF) < 0x40) titleDrawCarouselGlyph(*s, idx, -1, *pal++);
            else titleDrawCarouselGlyph(*s, (short)ang, 4, *pal++);
        }
        if (idx >= 0) idx++;
        s++;
        ang += 4;
    }
    if (TASK_C->w4c == 0) {
        switch (sel) {
        case 0:
            m.x = 0xCC;
            m.y = 0xB8;
            m.tp = 6;
            m.font = TITLE_FONT;
            m.str = TITLE_ARROW_RIGHT_STRING;
            m.pal = TITLE_ARROW_RIGHT_PALETTE;
            titleDrawString(&m, 0, 1, 1);
            break;
        case 1:
            m.x = 0x64;
            m.y = 0xB8;
            m.tp = 6;
            m.font = TITLE_FONT;
            m.str = TITLE_ARROW_LEFT_STRING;
            m.pal = TITLE_ARROW_LEFT_PALETTE;
            titleDrawString(&m, 0, 1, 1);
            m.x = 0xD0;
            m.y = 0xB8;
            m.tp = 6;
            m.font = TITLE_FONT;
            m.str = TITLE_ARROW_RIGHT_STRING;
            m.pal = TITLE_ARROW_RIGHT_PALETTE;
            titleDrawString(&m, 0, 1, 1);
            break;
        case 2:
            m.x = 0x70;
            m.y = 0xB8;
            m.tp = 6;
            m.font = TITLE_FONT;
            m.str = TITLE_ARROW_LEFT_STRING;
            m.pal = TITLE_ARROW_LEFT_PALETTE;
            titleDrawString(&m, 0, 1, 1);
            break;
        }
    }
}
