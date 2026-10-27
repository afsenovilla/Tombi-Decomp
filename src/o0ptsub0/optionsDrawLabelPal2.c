// FUNC 800e97f4 104 O0PTSUB0
// MATCHING 800e97f4 104
typedef struct { short x, y, n, pad; int a; void *b; void *c; } MenuItem;
extern char OPTIONS_FONT[];
extern char OPTIONS_PAL2_GLYPH_PALETTES[];
extern int OPTIONS_STRINGS[];
extern void titleDrawString(MenuItem *m, int a, int b, int c);

void optionsDrawLabelPal2(short x, short y, int i)
{
    MenuItem m;
    m.x = x;
    m.y = y;
    m.n = 2;
    m.a = OPTIONS_STRINGS[i];
    m.b = OPTIONS_FONT;
    m.c = OPTIONS_PAL2_GLYPH_PALETTES;
    titleDrawString(&m, 0, 1, 2);
}
