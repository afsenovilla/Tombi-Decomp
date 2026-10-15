// FUNC 800ec504 1316 D0SPSUB
typedef struct { char p[0x12]; short mode; short page; short cursor; } Menu;
extern unsigned short D_1F8001FC;
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

int func_800EC504(Menu *m, int sound)
{
    short c = m->cursor;
    if (c < 4) {
        if (D_1F8001FC & 0x10) {
            if (sound) playSFX(8);
            c = --m->cursor;
            if (c < 0) {
                switch (m->mode) {
                case 0:
                    m->cursor = 6;
                    break;
                case 1:
                    m->cursor = 4;
                    break;
                case 2:
                    m->cursor = 5;
                    break;
                case 3:
                    m->cursor = 7;
                    break;
                }
            } else {
                fixCursor(m, c);
            }
            return 1;
        }
        if (D_1F8001FC & 0x40) {
            if ((c >= 0 && c < 3) || (c >= 4 && c < 7)) {
                m->cursor = c + 1;
                if (fixCursor(m, m->cursor)) return 1;
                if (sound) goto sfx;
            }
            return 1;
        }
        if (D_1F8001FC & 0x80) {
            switch (m->mode) {
            case 0:
                m->cursor = 5;
                break;
            case 1:
                break;
            case 2:
                m->cursor = 4;
                break;
            case 3:
                m->cursor = 6;
                break;
            }
            fixCursor(m, m->cursor);
            if (m->mode == 1) return 0;
            goto sfx;
        }
        if (D_1F8001FC & 0x20) {
            switch (m->mode) {
            case 0:
                m->cursor = 7;
                break;
            case 1:
                m->cursor = 5;
                break;
            case 2:
                m->cursor = 6;
                break;
            }
            fixCursor(m, m->cursor);
            if (m->mode == 3) return 0;
            goto sfx;
        }
        return 0;
    }
    if (c < 8) {
        if (D_1F8001FC & 0x80) {
            switch (m->cursor) {
            case 5:
            case 6:
            case 7:
                m->cursor--;
                goto moved;
            }
            return 1;
        }
        if (D_1F8001FC & 0x20) {
            switch (m->cursor) {
            case 4:
            case 5:
            case 6:
                m->cursor++;
                goto moved;
            }
            return 1;
        }
        if (D_1F8001FC & 0x10) {
            m->cursor = 8;
        moved:
            if (sound) goto sfx;
            return 1;
        }
        if (D_1F8001FC & 0x40) {
            switch (m->mode) {
            case 1:
            case 2:
                m->cursor = 0;
                if (sound) goto sfx;
                return 1;
            }
        }
        return 0;
    }
    if (D_1F8001FC & 0x40) {
        switch (m->mode) {
        case 1:
            m->cursor = 4;
            break;
        case 0:
            m->cursor = 6;
            break;
        case 2:
            m->cursor = 5;
            break;
        case 3:
            m->cursor = 7;
            break;
        }
    sfx:
        playSFX(8);
        return 1;
    }
    return 0;
}
