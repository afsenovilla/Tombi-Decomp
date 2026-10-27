// FUNC 800e972c 108 O0PTSUB0
// MATCHING 800e972c 108
typedef struct { short x, y, n, pad; int a; void *b; void *c; } MenuItem;
extern char TITLE_FONT[];
extern int TITLE_STRINGS[];
extern void *TITLE_STRING_PALETTES[];
extern void titleDrawString(MenuItem *m, int a, int b, int c);

void titleDrawText(short x, short y, int i)
{
    MenuItem m;
    m.n = 6;
    m.x = x;
    m.y = y;
    m.a = TITLE_STRINGS[i];
    m.b = TITLE_FONT;
    m.c = TITLE_STRING_PALETTES[i];
    titleDrawString(&m, 0, 1, 1);
}
