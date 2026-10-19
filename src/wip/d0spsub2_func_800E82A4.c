// FUNC 800e82a4 3632 D0SPSUB2
typedef struct { unsigned char state, b1, sub, b3; short w4; short timer; char p8[0xE]; short cursor; short card; short msg; short x1C; } Menu;
typedef struct { char name[0x14]; } Entry;
typedef struct { Entry e[16]; short status, prev, n144, w146; signed char slot[16]; short w158; } Card;
typedef struct { short x6; char r[0x12]; } Info;
typedef struct { char d[0x988]; } BGame;
extern unsigned short D_1F8001FC;
extern short D_1F8003D8;
extern unsigned char D_8009C977[];
extern char D_8009C960[];
extern unsigned char D_8009C970, D_8009C971;
extern unsigned char D_8009CF1C;
extern unsigned short D_8009F3DC;
extern char D_800E1E28[];
extern char D_800E810C[];
extern char *D_800F6A1C[];
extern char *D_800F6A3C[];
extern char D_800F7098[];
extern char D_800F70B8[];
extern short D_800F70F0, D_800F70F4, D_800F70F8;
extern short D_800F70FC[2][16];
extern short D_800F713C[];
extern int D_800F7148;
extern Info D_800F74BA[2][15];
extern void SfxPlay2(int a, int b);
extern void playSFX(int id);
extern int erase(char *name);
extern void func_800E999C(int slot);
extern int func_800EA118(void *p, int n);
extern int func_800EC77C(Card *c, int n);
extern short func_800EC8FC(Card *c, int n);
extern int func_800ECCC8(char *name, void *buf, int ofs, int n);
extern int func_800ECD94(char *name, void *buf, int n, int mode);
extern void initGameConfig(void);
extern int sprintf(char *s, const char *fmt, ...);
extern char *strcpy(char *d, const char *s);
extern char *strncat(char *d, const char *s, int n);
extern char *strncpy(char *d, const char *s, int n);

int func_800E82A4(Menu *m, Card *c)
{
    int i, j, n;
    int tries;
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
        if (m->cursor >= D_800F713C[m->card]) m->cursor = D_800F713C[m->card] - 1;
        y = m->cursor * 0x28;
        D_800F70F0 = y;
        D_800F70F4 = y;
        D_800F70F8 = y - 0x28;
        if (D_800F70F8 < 0) D_800F70F8 = 0;
        D_8009C977[0] = c->slot[D_800F70FC[m->card][m->cursor]];
        m->sub = 2;
        break;
    case 1:
        if (--m->timer != 0) return 0;
        c->n144 = func_800EC8FC(c, m->card);
        c->w146 = 2;
        n = 0;
        if (m->x1C) {
            for (i = 0; i < 15; i++) {
                if (c->slot[i] >= 0) {
                    D_800F70FC[m->card][n++] = i;
                }
            }
        } else {
            for (i = 0; i < 15; i++) {
                if (c->slot[i] >= 0 && D_800F74BA[m->card][i].x6 != -1) {
                    D_800F70FC[m->card][n++] = i;
                }
            }
        }
        D_800F713C[m->card] = n;
        for (i = 0; i < D_800F713C[m->card] - 1; i++) {
            for (j = i + 1; j < D_800F713C[m->card]; j++) {
                short a = D_800F70FC[m->card][i];
                short b = D_800F70FC[m->card][j];
                if ((unsigned char)c->slot[b] < (unsigned char)c->slot[a]) {
                    D_800F70FC[m->card][i] = b;
                    D_800F70FC[m->card][j] = a;
                }
            }
        }
        if (D_8009C977[0] == 0xFF) {
            if (m->x1C) {
                m->cursor = 0;
                for (i = 0; i < D_800F713C[m->card]; i++) {
                    if (D_800F74BA[m->card][D_800F70FC[m->card][i]].x6 == -1) {
                        m->cursor = i;
                        break;
                    }
                }
            } else {
                m->cursor = 0;
            }
        } else {
            m->cursor = 0;
            for (i = 0; i < D_800F713C[m->card]; i++) {
                if ((unsigned char)c->slot[D_800F70FC[m->card][i]] == D_8009C977[0]) {
                    m->cursor = i;
                    break;
                }
            }
        }
        y = m->cursor * 0x28;
        D_800F70F0 = y;
        D_800F70F4 = y;
        D_800F70F8 = y - 0x28;
        if (D_800F70F8 < 0) D_800F70F8 = 0;
        D_8009C977[0] = c->slot[D_800F70FC[m->card][m->cursor]];
        m->sub++;
        break;
    case 2:
        if (c->n144 == 15) {
            if (c->w158 == 0) {
                m->msg = 5;
                if (D_1F8001FC & 0x2000) {
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
                if (!(D_1F8001FC & 0x2000)) return 0;
                SfxPlay2(0x1B, 0x1C);
                return -1;
            }
        }
        btn = D_1F8001FC;
        if (btn & 0x10) {
            if (m->cursor > 0) {
                m->cursor--;
                D_800F70F0 -= 0x28;
                playSFX(8);
                m->b1++;
            }
            break;
        }
        if (btn & 0x40) {
            if (m->cursor < D_800F713C[m->card] - 1) {
                m->cursor++;
                D_800F70F0 += 0x28;
                playSFX(8);
                m->b1++;
            }
            break;
        }
        D_8009C977[0] = c->slot[D_800F70FC[m->card][m->cursor]];
        if (btn & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return -1;
        }
        if (btn & 0x4000) {
            SfxPlay2(0xA, 0xA);
            D_800F7148 = 1;
            if (m->x1C) {
                if (D_800F74BA[m->card][D_800F70FC[m->card][m->cursor]].x6 >= 0) m->sub = 4;
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
        if (D_800F7148 == 0) {
            btn = D_1F8001FC;
            if (btn & 0x20) {
                D_800F7148 = 1;
                goto snd;
            }
            if (btn & 0x2000) goto cancel;
            if (btn & 0x4000) {
                SfxPlay2(0xA, 0xA);
                m->msg = 6;
                m->sub++;
            }
            break;
        }
    else5:
        btn = D_1F8001FC;
        if (btn & 0x80) goto clr;
        goto chk;
    case 6:
        strcpy(D_800F7098, D_800F6A3C[m->card]);
        strncat(D_800F7098, c->e[D_800F70FC[m->card][m->cursor]].name, 0x14);
        strncpy(D_800F70B8, c->e[D_800F70FC[m->card][m->cursor]].name + 0xC, 8);
        if (erase(D_800F7098)) {
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
        func_800E999C((unsigned char)c->slot[D_800F70FC[m->card][m->cursor]]);
        strcpy(D_800F7098, D_800F6A3C[m->card]);
        strncat(D_800F7098, D_800F6A1C[D_1F8003D8], 0xC);
        strncat(D_800F7098, D_800F70B8, 8);
        if (func_800EC77C(c, m->card)) {
            c->w146 = 0;
            m->sub = 0;
            m->w4 = 1;
            m->timer = 1;
            break;
        }
        if (func_800ECD94(D_800F7098, D_800E1E28, 0xC00, 1)) {
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
        tries = 3;
        strcpy(D_800F7098, D_800F6A3C[m->card]);
        strncat(D_800F7098, c->e[D_800F70FC[m->card][m->cursor]].name, 0x14);
        do {
            if (func_800ECCC8(D_800F7098, D_800E1E28, 0, 0xC00) == 0 && (func_800EA118(D_800E1E28 + 0x200, 0xA00) & 0xFF)) {
                *(BGame *)D_8009C960 = *(BGame *)(D_800E1E28 + 0x200);
                D_8009CF1C = D_8009F3DC;
                D_8009C970 = D_8009C971;
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
        if (D_1F8001FC & 0x6000) {
            SfxPlay2(0x1B, 0x1C);
            c->w146 = 0;
            m->sub = 1;
            m->timer = 1;
        }
        break;
    case 13:
        if (D_800F7148 == 0) {
            btn = D_1F8001FC;
            if (btn & 0x20) {
                D_800F7148 = 1;
                goto snd;
            }
            if (btn & 0x2000) goto cancel;
            if (btn & 0x4000) {
                m->msg = 10;
                m->timer = 4;
                SfxPlay2(0xA, 0xA);
                m->sub = 9;
            }
            break;
        }
        goto else5;
    case 12:
    case 14:
        m->msg = 14;
        m->sub++;
        break;
    case 15:
        if (D_800F7148 == 0) {
            btn = D_1F8001FC;
            if (btn & 0x20) {
                D_800F7148 = 1;
                goto snd;
            }
            if (btn & 0x2000) goto cancel;
            if (btn & 0x4000) {
                sprintf(D_800F70B8, D_800E810C, (unsigned char)c->slot[D_800F70FC[m->card][m->cursor]]);
                m->msg = 6;
                m->timer = 4;
                SfxPlay2(0xA, 0xA);
                m->sub = 7;
                break;
            }
            return 0;
        }
        btn = D_1F8001FC;
        if (btn & 0x80) {
        clr:
            D_800F7148 = 0;
        snd:
            playSFX(8);
            return 0;
        }
    chk:
        if (btn & 0x6000) {
        cancel:
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
