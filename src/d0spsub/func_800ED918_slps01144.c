// VERSION SLPS_01144 dspsub 800ED918 772
#if defined(VERSION_SLPS_01144)
// JP version of PAL d0spsub 800ec504 (subScreenMoveTabCursor.c): 4-wide grid, up/down by 4
typedef struct { char p[0x12]; short mode; short page; short cursor; } Menu;
extern unsigned short JOYPAD_STATE;
extern void playSFX(int id);

static inline int fixCursor(Menu *m, short c)
{
    if (c < 4) {
        switch (m->mode) {
        case 0:
            m->cursor = c + 4;
            return 1;
        case 1:
            if (c >= 3) {
                m->cursor = 2;
                return 1;
            }
            break;
        case 2:
            if (c > 0) {
                m->cursor = 0;
                return 1;
            }
            break;
        case 3:
            if (c > 0) {
                m->cursor = 0;
                return 1;
            }
            break;
        }
    }
    return 0;
}

int func_800ED918(Menu *m, int sound)
{
    unsigned short pad = JOYPAD_STATE;
    if (pad & 0x8000) {
        short lim = 3;
        short c;
        if (m->mode == 0) lim = 7;
        c = m->cursor;
        if (c > lim) {
            m->cursor = c - 4;
            fixCursor(m, (short)(c - 4));
            if (sound) playSFX(8);
        }
        return 1;
    }
    if (pad & 0x2000) {
        short c = m->cursor;
        if (c < 8) {
            m->cursor = c + 4;
            if (m->cursor >= 8) m->cursor = 11;
            fixCursor(m, m->cursor);
            if (sound) playSFX(8);
        }
        return 1;
    }
    if (pad & 0x1000) {
        unsigned short c = m->cursor;
        if ((unsigned short)(c - 1) < 3 || (unsigned short)(c - 5) < 3) {
            m->cursor = c - 1;
            if (fixCursor(m, (short)(c - 1))) return 1;
            if (sound) playSFX(8);
        }
        return 1;
    }
    if (pad & 0x4000) {
        unsigned short c = m->cursor;
        if (c < 3 || (unsigned short)(c - 4) < 3) {
            m->cursor = c + 1;
            if (fixCursor(m, m->cursor)) return 1;
            if (sound) {
                /* two calls: keeps cross-jumping from merging the up/down tails (permuter) */
                if (c) playSFX(8);
                else playSFX(8);
            }
        }
        return 1;
    }
    return 0;
}
#endif
