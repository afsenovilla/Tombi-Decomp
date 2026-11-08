// VERSION SLPS_01144 dspsub3 800ED7F4 356
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub3 800ec3f0 (subScreenMoveTabCursor.c): 4x3 grid, up/down by 8
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

int func_800ED7F4(Menu *m)
{
    unsigned short pad = JOYPAD_STATE;
    if (pad & 0x8000) {
        short n = m->cursor;
        if (n < 4) return 1;
        m->cursor = n - 8;
        if (!skipRow3(m)) playSFX(8);
        return 1;
    }
    if (pad & 0x2000) {
        short n = m->cursor;
        if (n >= 4) return 1;
        m->cursor = n + 8;
        if (m->cursor >= 8) m->cursor = 11;
        if (!skipRow3(m)) playSFX(8);
        return 1;
    }
    if (pad & 0x1000) {
        unsigned short n = m->cursor;
        if ((unsigned short)(n - 1) < 3 || (unsigned short)(n - 5) < 3) {
            m->cursor = n - 1;
            if (!skipRow3(m)) playSFX(8);
        }
        return 1;
    }
    if (pad & 0x4000) {
        unsigned short n = m->cursor;
        if (n < 3 || (unsigned short)(n - 4) < 3) {
            m->cursor = n + 1;
            if (!skipRow3(m)) playSFX(8);
        }
        return 1;
    }
    return 0;
}
#endif
