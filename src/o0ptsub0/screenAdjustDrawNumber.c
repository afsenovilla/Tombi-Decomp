// FUNC 800e98b4 328 O0PTSUB0
// MATCHING 800e98b4 328
typedef struct { short x, y, n, pad; int a; void *b; void *c; } MenuItem;
extern char OPTIONS_FONT[];
extern int OPTIONS_DIGIT_STRINGS[];
extern int OPTIONS_PLUS_STRING, OPTIONS_MINUS_STRING;
extern void titleDrawString(MenuItem *m, int a, int b, int c);

void screenAdjustDrawNumber(short x, short y, int n)
{
    MenuItem m;
    m.n = 9;
    m.x = x;
    m.y = y;
    m.b = OPTIONS_FONT;
    if (n > 0) {
        m.a = OPTIONS_PLUS_STRING;
        titleDrawString(&m, 0, 1, 0);
    } else if (n < 0) {
        n = -n;
        m.a = OPTIONS_MINUS_STRING;
        titleDrawString(&m, 0, 1, 0);
    }
    m.x += 8;
    if (n >= 10) {
        m.a = OPTIONS_DIGIT_STRINGS[n / 10];
        m.b = OPTIONS_FONT;
        titleDrawString(&m, 0, 1, 0);
        n %= 10;
        m.x += 8;
    }
    m.a = OPTIONS_DIGIT_STRINGS[n];
    m.b = OPTIONS_FONT;
    titleDrawString(&m, 0, 1, 0);
}
