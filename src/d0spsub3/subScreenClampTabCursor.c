// FUNC 800ec3bc 52 D0SPSUB3
// MATCHING 800ec3bc 52
typedef struct { char p[0x16]; short cursor; } Menu;

int subScreenClampTabCursor(Menu *m)
{
    short n = m->cursor;
    if (n < 4) {
        if (n >= 3) {
            m->cursor = 2;
            return 1;
        }
    }
    return 0;
}
