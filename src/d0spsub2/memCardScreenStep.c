// FUNC 800e9308 1408 D0SPSUB2
// MATCHING 800e9308 1408
typedef struct { unsigned char state, sub, b2, b3; short timer; short x6; char p8[0x10]; short card; short x1A; short x1C; } Menu;
typedef struct { char p0[0x140]; short status, prev, p144, type; char p148[0x12]; } Card;
extern Menu MENU_STATE;
extern unsigned char D_800A45D9;
extern Card MEMCARD_CARDS[];
extern short MEMCARD_FILE_COUNT[];
extern int MEMCARD_SLOT_PRESENT[2];
extern int MEMCARD_SELECTED_SLOT, MEMCARD_PROBED;
extern short MEMCARD_CURSOR_Y_TARGET, MEMCARD_CURSOR_Y, MEMCARD_LIST_SCROLL;
extern unsigned short JOYPAD_STATE;
extern int memCardProbe(Card *c, int n);
extern int memCardFileListStep(Menu *m, Card *c);
extern int memCardFormatStep(Menu *m, Card *c);
extern void playSFX(int id);
extern void SfxPlay2(int a, int b);

int memCardScreenStep(void)
{
    Menu *m = &MENU_STATE;
    Card *c;
    int i;
    switch (D_800A45D9) {
    case 0:
        m->card = 0;
        for (i = 0; i < 2; i++) {
            c = &MEMCARD_CARDS[i];
            c->type = 0;
            c->status = -3;
            c->prev = -3;
            MEMCARD_FILE_COUNT[i] = 0;
        }
        MEMCARD_SELECTED_SLOT = 0;
        MEMCARD_PROBED = 0;
        m->x1A = 1;
        m->timer = 4;
        m->x6 = 1;
        m->b2 = 0;
        m->sub++;
        break;
    case 1:
        if (--m->timer == 0) {
            MEMCARD_PROBED = 1;
            m->timer = 0x1E;
            for (i = 1; i >= 0; i--) {
                c = &MEMCARD_CARDS[i];
                c->status = memCardProbe(c, i);
                if (c->status != c->prev) {
                    c->prev = c->status;
                    if (c->type == 1 || c->type == -1) {
                        MEMCARD_SLOT_PRESENT[i] = 1;
                        MEMCARD_SELECTED_SLOT = i + 1;
                    } else {
                        MEMCARD_SLOT_PRESENT[i] = 0;
                    }
                }
            }
        }
        if (MEMCARD_SLOT_PRESENT[0]) {
            if (MEMCARD_SLOT_PRESENT[1]) {
                if ((JOYPAD_STATE & 0x80) && MEMCARD_SELECTED_SLOT == 2) {
                    MEMCARD_SELECTED_SLOT = 1;
                    playSFX(8);
                } else if ((JOYPAD_STATE & 0x20) && MEMCARD_SELECTED_SLOT == 1) {
                    MEMCARD_SELECTED_SLOT = 2;
                    playSFX(8);
                }
            } else {
                MEMCARD_SELECTED_SLOT = 1;
            }
        } else if (MEMCARD_SLOT_PRESENT[1]) {
            MEMCARD_SELECTED_SLOT = 2;
        } else {
            MEMCARD_SELECTED_SLOT = 0;
        }
        if ((JOYPAD_STATE & 0x4000) && MEMCARD_SELECTED_SLOT) {
            m->card = MEMCARD_SELECTED_SLOT - 1;
            SfxPlay2(10, 10);
            m->x1A = 1;
            m->timer = 1;
            m->b2 = 0;
            m->sub++;
            break;
        }
        if (JOYPAD_STATE & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return -1;
        }
        break;
    case 2:
        c = &MEMCARD_CARDS[m->card];
        if (--m->timer == 0) {
            m->timer = 0x5A;
            c->status = memCardProbe(c, m->card);
            if (c->status != c->prev) {
                c->prev = c->status;
                m->b2 = 0;
            }
        }
        switch (c->status) {
        case 0:
            {
            switch (memCardFileListStep(m, c)) {
            case -1:
                m->sub--;
                break;
            case 1:
                return 1;
            default:
                return 0;
            }
            }
            break;
        case 1:
            if (m->x1C) {
                if (memCardFormatStep(m, c) == -1) m->sub--;
            } else {
                m->x1A = 3;
                if (JOYPAD_STATE & 0x2000) m->sub--;
            }
            break;
        case -1:
            c->prev = 0;
            m->timer = 1;
            m->sub--;
            break;
        case -2:
            m->x1A = 0;
            if (JOYPAD_STATE & 0x2000) {
                c->prev = 0;
                m->timer = 1;
                m->sub--;
            }
            break;
        default:
            return 0;
        }
        break;
    case 3:
        {
        int d = MEMCARD_CURSOR_Y_TARGET - MEMCARD_LIST_SCROLL;
        if (d > 0x50) {
            m->sub++;
        } else if (d < 0) {
            m->sub++;
        } else {
            MEMCARD_CURSOR_Y = MEMCARD_CURSOR_Y_TARGET;
            m->sub--;
        }
        }
        break;
    case 4:
        if (MEMCARD_CURSOR_Y_TARGET != MEMCARD_CURSOR_Y) {
            if (MEMCARD_CURSOR_Y_TARGET - MEMCARD_CURSOR_Y > 0) MEMCARD_CURSOR_Y += 8;
            else MEMCARD_CURSOR_Y -= 8;
            if (MEMCARD_CURSOR_Y - MEMCARD_LIST_SCROLL > 0x28) MEMCARD_LIST_SCROLL += 8;
            if (MEMCARD_CURSOR_Y - MEMCARD_LIST_SCROLL < 0) MEMCARD_LIST_SCROLL -= 8;
            return 0;
        }
        m->sub = 2;
        break;
    }
    return 0;
}
