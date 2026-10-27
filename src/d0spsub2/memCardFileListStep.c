// FUNC 800e82a4 3632 D0SPSUB2
// MATCHING 800e82a4 3632
typedef struct { unsigned char state, b1, sub, b3; short w4; short timer; char p8[0xE]; short cursor; short card; short msg; short x1C; } Menu;
typedef struct { char name[0x14]; } Entry;
typedef struct { Entry e[16]; short status, prev, n144, w146; signed char slot[16]; short w158; } Card;
typedef struct { short x6; char r[0x12]; } Info;
typedef struct { char d[0x988]; } BGame;
typedef struct { char p0[0x10]; unsigned char f10, f11; char p12[0x5BC - 0x12]; unsigned char f5bc; char p5bd[0x988 - 0x5BD]; } GS;
extern GS D_8009C960g;
extern unsigned short JOYPAD_STATE;
extern short D_1F8003D8;
extern unsigned char D_8009C977[];
extern char GAME[];
extern unsigned char GAME_PLAYER_HEALTH, GAME_PLAYER_MAX_HEALTH;
extern unsigned char D_8009CF1C;
extern unsigned short BUTTON_CONFIG_INDEX;
extern char SAVE_DATA_BUFFER[];
extern char MEMCARD_FILE_SUFFIX_FORMAT[];
extern char *MEMCARD_SAVE_PREFIXES_2[];
extern char *MEMCARD_PORT_PREFIXES_2[];
extern char MEMCARD_FILE_PATH[];
extern char MEMCARD_FILE_SUFFIX[];
extern short MEMCARD_CURSOR_Y_TARGET, MEMCARD_CURSOR_Y, MEMCARD_LIST_SCROLL;
extern short MEMCARD_FILE_ORDER[2][16];
extern short MEMCARD_FILE_COUNT[];
extern int MEMCARD_YESNO_CURSOR;
extern Info D_800F74BA[2][15];
extern void SfxPlay2(int a, int b);
extern void playSFX(int id);
extern int erase(char *name);
extern void func_800E999C(int slot);
extern int memCardVerifyChecksum(void *p, int n);
extern int memCardProbe(Card *c, int n);
extern short memCardReadDirectory(Card *c, int n);
extern int memCardReadFile(char *name, void *buf, int ofs, int n);
extern int memCardWriteFile(char *name, void *buf, int n, int mode);
extern void initGameConfig(void);
extern int sprintf(char *s, const char *fmt, ...);
extern char *strcpy(char *d, const char *s);
extern char *strncat(char *d, const char *s, int n);
extern char *strncpy(char *d, const char *s, int n);

int memCardFileListStep(Menu *m, Card *c)
{
    int i, j, n;
    int t;
    int tries;
    unsigned char *g;
    short y;
    unsigned short btn;
    switch (m->sub) {
    case 0:
        if (c->w146 == 1) {
            m->msg = 1;
            m->timer = 3;
            m->sub++;
            break;
        }
        if (m->cursor >= MEMCARD_FILE_COUNT[m->card]) m->cursor = MEMCARD_FILE_COUNT[m->card] - 1;
        MEMCARD_CURSOR_Y_TARGET = m->cursor * 0x28;
        MEMCARD_CURSOR_Y = MEMCARD_CURSOR_Y_TARGET;
        MEMCARD_LIST_SCROLL = MEMCARD_CURSOR_Y_TARGET - 0x28;
        if (MEMCARD_LIST_SCROLL < 0) MEMCARD_LIST_SCROLL = 0;
        D_8009C977[0] = c->slot[MEMCARD_FILE_ORDER[m->card][m->cursor]];
        m->sub = 2;
        break;
    case 1:
        if (--m->timer != 0) return 0;
        c->n144 = memCardReadDirectory(c, m->card);
        c->w146 = 2;
        n = 0;
        if (m->x1C) {
            for (i = 0; i < 15; i++) {
                if (c->slot[i] >= 0) {
                    MEMCARD_FILE_ORDER[m->card][n++] = i;
                }
            }
        } else {
            for (i = 0; i < 15; i++) {
                if (c->slot[i] >= 0 && D_800F74BA[m->card][i].x6 != -1) {
                    MEMCARD_FILE_ORDER[m->card][n++] = i;
                }
            }
        }
        MEMCARD_FILE_COUNT[m->card] = n;
        for (i = 0; i < MEMCARD_FILE_COUNT[m->card] - 1; i++) {
            for (j = i + 1; j < MEMCARD_FILE_COUNT[m->card]; j++) {
                if ((unsigned char)c->slot[MEMCARD_FILE_ORDER[m->card][i]] > (unsigned char)c->slot[MEMCARD_FILE_ORDER[m->card][j]]) {
                    t = MEMCARD_FILE_ORDER[m->card][i];
                    MEMCARD_FILE_ORDER[m->card][i] = MEMCARD_FILE_ORDER[m->card][j];
                    MEMCARD_FILE_ORDER[m->card][j] = t;
                }
            }
        }
        if (D_8009C977[0] == 0xFF) {
            if (m->x1C) {
                m->cursor = 0;
                for (i = 0; i < MEMCARD_FILE_COUNT[m->card]; i++) {
                    if (D_800F74BA[m->card][MEMCARD_FILE_ORDER[m->card][i]].x6 == -1) {
                        m->cursor = i;
                        break;
                    }
                }
            } else {
                m->cursor = 0;
            }
        } else {
            m->cursor = 0;
            for (i = 0; i < MEMCARD_FILE_COUNT[m->card]; i++) {
                if ((unsigned char)c->slot[MEMCARD_FILE_ORDER[m->card][i]] == D_8009C977[0]) {
                    m->cursor = i;
                    break;
                }
            }
        }
        MEMCARD_CURSOR_Y_TARGET = m->cursor * 0x28;
        MEMCARD_CURSOR_Y = MEMCARD_CURSOR_Y_TARGET;
        MEMCARD_LIST_SCROLL = MEMCARD_CURSOR_Y_TARGET - 0x28;
        if (MEMCARD_LIST_SCROLL < 0) MEMCARD_LIST_SCROLL = 0;
        D_8009C977[0] = c->slot[MEMCARD_FILE_ORDER[m->card][m->cursor]];
        m->sub++;
        break;
    case 2:
        if (c->n144 == 15) {
            if (c->w158 == 0) {
                m->msg = 5;
                if (JOYPAD_STATE & 0x2000) {
                    SfxPlay2(0x1B, 0x1C);
                    return -1;
                }
                break;
            }
            if (m->x1C) m->msg = 7;
            else m->msg = 3;
        } else {
            m->msg = 3;
            if (m->x1C == 0 && c->w158 == 0) {
                if (!(JOYPAD_STATE & 0x2000)) return 0;
                SfxPlay2(0x1B, 0x1C);
                return -1;
            }
        }
        btn = JOYPAD_STATE;
        if (btn & 0x10) {
            if (m->cursor > 0) {
                m->cursor--;
                MEMCARD_CURSOR_Y_TARGET -= 0x28;
                playSFX(8);
                m->b1++;
            }
            break;
        }
        if (btn & 0x40) {
            if (m->cursor < MEMCARD_FILE_COUNT[m->card] - 1) {
                m->cursor++;
                MEMCARD_CURSOR_Y_TARGET += 0x28;
                playSFX(8);
                m->b1++;
            }
            break;
        }
        D_8009C977[0] = c->slot[MEMCARD_FILE_ORDER[m->card][m->cursor]];
        if (btn & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return -1;
        }
        if (btn & 0x4000) {
            SfxPlay2(0xA, 0xA);
            MEMCARD_YESNO_CURSOR = 1;
            if (m->x1C) {
                if (D_800F74BA[m->card][MEMCARD_FILE_ORDER[m->card][m->cursor]].x6 >= 0) m->sub = 4;
                else m->sub = 14;
            } else {
                m->sub = 12;
            }
        }
        break;
    case 4:
        m->msg = 9;
        m->sub++;
        break;
    case 5:
        if (MEMCARD_YESNO_CURSOR == 0) {
            if (JOYPAD_STATE & 0x20) {
                MEMCARD_YESNO_CURSOR = 1;
                playSFX(8);
                return 0;
            }
            if (JOYPAD_STATE & 0x2000) {
                SfxPlay2(0x1B, 0x1C);
                m->sub = 2;
                break;
            }
            if (JOYPAD_STATE & 0x4000) {
                SfxPlay2(0xA, 0xA);
                m->msg = 6;
                m->sub++;
            }
            break;
        }
        if (JOYPAD_STATE & 0x80) {
            MEMCARD_YESNO_CURSOR = 0;
            playSFX(8);
            return 0;
        }
        if (JOYPAD_STATE & 0x6000) {
            SfxPlay2(0x1B, 0x1C);
            m->sub = 2;
        }
        break;
    case 6:
        strcpy(MEMCARD_FILE_PATH, MEMCARD_PORT_PREFIXES_2[m->card]);
        strncat(MEMCARD_FILE_PATH, c->e[MEMCARD_FILE_ORDER[m->card][m->cursor]].name, 0x14);
        strncpy(MEMCARD_FILE_SUFFIX, c->e[MEMCARD_FILE_ORDER[m->card][m->cursor]].name + 0xC, 8);
        if (erase(MEMCARD_FILE_PATH)) {
            m->timer = 1;
            m->sub++;
            break;
        }
        c->w146 = 0;
        m->sub = 1;
        m->timer = 1;
        break;
    case 7:
        if (--m->timer != 0) return 0;
        m->sub++;
        break;
    case 8:
        func_800E999C((unsigned char)c->slot[MEMCARD_FILE_ORDER[m->card][m->cursor]]);
        strcpy(MEMCARD_FILE_PATH, MEMCARD_PORT_PREFIXES_2[m->card]);
        strncat(MEMCARD_FILE_PATH, MEMCARD_SAVE_PREFIXES_2[D_1F8003D8], 0xC);
        strncat(MEMCARD_FILE_PATH, MEMCARD_FILE_SUFFIX, 8);
        if (memCardProbe(c, m->card)) {
            c->w146 = 0;
            m->sub = 0;
            m->w4 = 1;
            m->timer = 1;
            break;
        }
        if (memCardWriteFile(MEMCARD_FILE_PATH, SAVE_DATA_BUFFER, 0xC00, 1)) {
            m->msg = 0;
            m->sub = 11;
            break;
        }
        m->msg = 15;
        m->sub = 16;
        m->timer = 0x28;
        break;
    case 9:
        if (--m->timer != 0) return 0;
        initGameConfig();
        m->sub++;
        break;
    case 10:
        strcpy(MEMCARD_FILE_PATH, MEMCARD_PORT_PREFIXES_2[m->card]);
        tries = 3;
        strncat(MEMCARD_FILE_PATH, c->e[MEMCARD_FILE_ORDER[m->card][m->cursor]].name, 0x14);
        do {
            if (memCardReadFile(MEMCARD_FILE_PATH, SAVE_DATA_BUFFER, 0, 0xC00) == 0 && (memCardVerifyChecksum(SAVE_DATA_BUFFER + 0x200, 0xA00) & 0xFF)) {
                D_8009C960g = *(GS *)(SAVE_DATA_BUFFER + 0x200);
                D_8009C960g.f5bc = BUTTON_CONFIG_INDEX;
                D_8009C960g.f10 = D_8009C960g.f11;
                m->msg = 15;
                m->sub = 17;
                m->timer = 0x28;
                return 0;
            }
        } while (--tries > 0);
        m->msg = 0;
        m->sub = 11;
        break;
    case 11:
        if (JOYPAD_STATE & 0x6000) {
            SfxPlay2(0x1B, 0x1C);
            c->w146 = 0;
            m->sub = 1;
            m->timer = 1;
        }
        break;
    case 13:
        if (MEMCARD_YESNO_CURSOR == 0) {
            if (JOYPAD_STATE & 0x20) {
                MEMCARD_YESNO_CURSOR = 1;
                playSFX(8);
                return 0;
            }
            if (JOYPAD_STATE & 0x2000) {
                SfxPlay2(0x1B, 0x1C);
                m->sub = 2;
                break;
            }
            if (JOYPAD_STATE & 0x4000) {
                m->msg = 10;
                m->timer = 4;
                SfxPlay2(0xA, 0xA);
                m->sub = 9;
            }
            break;
        }
        if (JOYPAD_STATE & 0x80) {
            MEMCARD_YESNO_CURSOR = 0;
            playSFX(8);
            return 0;
        }
        if (JOYPAD_STATE & 0x6000) {
            SfxPlay2(0x1B, 0x1C);
            m->sub = 2;
        }
        break;
    case 12:
    case 14:
        m->msg = 14;
        m->sub++;
        break;
    case 15:
        if (MEMCARD_YESNO_CURSOR == 0) {
            if (JOYPAD_STATE & 0x20) {
                MEMCARD_YESNO_CURSOR = 1;
                playSFX(8);
                return 0;
            }
            if (JOYPAD_STATE & 0x2000) {
                SfxPlay2(0x1B, 0x1C);
                m->sub = 2;
                break;
            }
            if (JOYPAD_STATE & 0x4000) {
                sprintf(MEMCARD_FILE_SUFFIX, MEMCARD_FILE_SUFFIX_FORMAT, (unsigned char)c->slot[MEMCARD_FILE_ORDER[m->card][m->cursor]]);
                m->msg = 6;
                m->timer = 4;
                SfxPlay2(0xA, 0xA);
                m->sub = 7;
                break;
            }
            return 0;
        }
        if (JOYPAD_STATE & 0x80) {
            MEMCARD_YESNO_CURSOR = 0;
            playSFX(8);
            break;
        }
        if (JOYPAD_STATE & 0x6000) {
            SfxPlay2(0x1B, 0x1C);
            m->sub = 2;
        }
        break;
    case 16:
        if (--m->timer != 0) return 0;
        c->w146 = 0;
        m->sub = 0;
        m->w4 = 1;
        m->timer = 1;
        break;
    case 17:
        if (--m->timer != 0) return 0;
        c->w146 = 0;
        m->sub = 0;
        m->timer = 1;
        return 1;
    }
    return 0;
}
