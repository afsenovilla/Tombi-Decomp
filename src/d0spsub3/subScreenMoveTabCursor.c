// FUNC 800ec3f0 308 D0SPSUB3
// MATCHING 800ec3f0 308
typedef struct { char p[0x16]; short cursor; } Menu;
extern unsigned short JOYPAD_STATE;
extern void playSFX(int id);

static inline int skipRow3(Menu *m)
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

int subScreenMoveTabCursor(Menu *m)
{
    short n = m->cursor;
    if (n < 4) {
        unsigned short pad = JOYPAD_STATE;
        if (pad & 0x10) {
            playSFX(8);
            if (--m->cursor < 0) m->cursor = 8;
            else skipRow3(m);
            return 1;
        }
        if (pad & 0x40) {
            if ((unsigned)n < 3 || (unsigned short)(n - 4) < 3) {
                m->cursor = n + 1;
                if (!skipRow3(m)) playSFX(8);
            }
            return 1;
        }
        return 0;
    }
    if (!(JOYPAD_STATE & 0x40)) return 0;
    m->cursor = 0;
    playSFX(8);
    return 1;
}
