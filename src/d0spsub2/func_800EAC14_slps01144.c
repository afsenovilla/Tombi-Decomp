// VERSION SLPS_01144 dspsub2 800EAC14 2836
#if defined(VERSION_SLPS_01144)
// JP memCardFileListStep (PAL d0spsub2 800e82a4, USA 800E7ECC): one card table, two-column file grid
typedef struct { unsigned char state, b1, sub, b3; short w4; short timer; char p8[0xE]; short cursor; short card; short msg; short x1C; } Menu;
typedef struct { char name[0x14]; } Entry;
typedef struct { Entry e[16]; short status, prev, n144, w146; signed char slot[16]; short w158; } Card;
typedef struct { short x; char r[0x12]; } Info;
typedef struct { char d[0x988]; } GS;
asm(".set JS_,0x1f8001fc");
extern unsigned short JS_;
extern GS D_8009E508;
extern unsigned char D_8009E51F[];
extern char D_800E38F8[];        /* save data buffer */
extern char D_800EA218[];        /* file name base */
extern char D_800EA228[];        /* file suffix format */
extern char *D_800F2A78[];       /* port prefixes */
extern char D_800F3040[];        /* file path */
extern char D_800F3060[];        /* file suffix */
extern short D_800F3098, D_800F309C, D_800F30A0; /* cursor y target, cursor y, list scroll */
extern short D_800F30A4[16];     /* file order */
extern short D_800F30C4;         /* file count */
extern Info D_800F338A[15];
extern void func_8001E388(int a, int b); /* SfxPlay2 */
extern void func_8001E2D4(int id); /* playSFX */
extern int func_8005856C(char *name); /* erase */
extern void func_800EBD14(int slot);
extern int memCardVerifyChecksum(void *p, int n);
extern int func_800EA334(Card *c, int n); /* memCardProbe */
extern short func_800EA490(Card *c, int n); /* memCardReadDirectory */
extern int memCardReadFile(char *name, void *buf, int ofs, int n);
extern int memCardWriteFile(char *name, void *buf, int n, int mode);
extern int func_80059270(char *s, const char *fmt, ...); /* sprintf */
extern char *func_80058874(char *d, const char *s); /* strcpy */
extern char *func_800588E8(char *d, const char *s, int n); /* strncat */
extern char *func_8005895C(char *d, const char *s, int n); /* strncpy */

int func_800EAC14(Menu *m, Card *c)
{
    int i, j, n;
    int t;
    int tries;
    unsigned short btn;
    switch (m->sub) {
    case 0:
        if (c->w146 == 1) {
            m->msg = 1;
            m->timer = 3;
            m->sub++;
            break;
        }
        m->sub = 2;
        break;
    case 1:
        if (--m->timer != 0) return 0;
        c->n144 = func_800EA490(c, m->card);
        c->w146 = 2;
        n = 0;
        if (m->x1C) {
            for (i = 0; i < 15; i++) {
                if (c->slot[i] >= 0) {
                    D_800F30A4[n++] = i;
                }
            }
        } else {
            for (i = 0; i < 15; i++) {
                if (c->slot[i] >= 0 && D_800F338A[i].x != -1) {
                    D_800F30A4[n++] = i;
                }
            }
        }
        D_800F30C4 = n;
        for (i = 0; i < D_800F30C4 - 1; i++) {
            for (j = i + 1; j < D_800F30C4; j++) {
                if ((unsigned char)c->slot[D_800F30A4[i]] > (unsigned char)c->slot[D_800F30A4[j]]) {
                    t = D_800F30A4[i];
                    D_800F30A4[i] = D_800F30A4[j];
                    D_800F30A4[j] = t;
                }
            }
        }
        if (D_8009E51F[0] == 0xFF) {
            if (m->x1C) {
                m->cursor = 0;
                for (i = 0; i < D_800F30C4; i++) {
                    if (D_800F338A[D_800F30A4[i]].x == -1) {
                        m->cursor = i;
                        break;
                    }
                }
            } else {
                m->cursor = 0;
            }
        } else {
            m->cursor = 0;
            for (i = 0; i < D_800F30C4; i++) {
                if ((unsigned char)c->slot[D_800F30A4[i]] == D_8009E51F[0]) {
                    m->cursor = i;
                    break;
                }
            }
        }
        D_800F3098 = (m->cursor >> 1) * 56;
        D_800F309C = D_800F3098;
        D_800F30A0 = D_800F3098 - 56;
        if (D_800F30A0 < 0) D_800F30A0 = 0;
        D_8009E51F[0] = c->slot[D_800F30A4[m->cursor]];
        m->sub++;
        break;
    case 2:
        if (c->n144 == 15) {
            if (c->w158 == 0) {
                m->msg = 5;
                if (JS_ & 0x40) {
                    func_8001E388(0x1B, 0x1C);
                    return -1;
                }
                break;
            }
            if (m->x1C) m->msg = 7;
            else m->msg = 3;
        } else {
            m->msg = 3;
            if (m->x1C == 0 && c->w158 == 0) {
                if (!(JS_ & 0x40)) return 0;
                func_8001E388(0x1B, 0x1C);
                return -1;
            }
        }
        btn = JS_;
        if (btn & 0x1000) {
            if (m->cursor >= 2) {
                m->cursor -= 2;
                D_800F3098 -= 56;
                func_8001E2D4(8);
                m->b1++;
            }
            break;
        }
        if (btn & 0x4000) {
            t = D_800F30C4 - 2;
            if (m->cursor < t) {
                m->cursor += 2;
                D_800F3098 += 56;
                func_8001E2D4(8);
                m->b1++;
            }
            break;
        }
        if (btn & 0x8000) {
            if (!(m->cursor & 1)) break;
            m->cursor--;
            goto sfx;
        }
        if (btn & 0x2000) {
            t = D_800F30C4 - 1;
            if (m->cursor & 1) return 0;
            if (m->cursor < t) {
                m->cursor++;
            sfx:
                func_8001E2D4(8);
                return 0;
            }
            break;
        }
        D_8009E51F[0] = c->slot[D_800F30A4[m->cursor]];
        if (btn & 0x40) {
            func_8001E388(0x1B, 0x1C);
            return -1;
        }
        if (btn & 0x30) {
            func_8001E388(0xA, 0xA);
            if (m->x1C) {
                if (D_800F338A[D_800F30A4[m->cursor]].x >= 0) m->sub = 4;
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
        btn = JS_;
        if (btn & 0x40) {
            func_8001E388(0x1B, 0x1C);
            m->sub = 2;
        } else if (btn & 0x10) {
            func_8001E388(0xA, 0xA);
            m->msg = 6;
            m->sub++;
        }
        break;
    case 6:
        func_80058874(D_800F3040, D_800F2A78[m->card]);
        func_800588E8(D_800F3040, c->e[D_800F30A4[m->cursor]].name, 0x14);
        func_8005895C(D_800F3060, c->e[D_800F30A4[m->cursor]].name + 0xC, 8);
        if (func_8005856C(D_800F3040)) {
            m->timer = 1;
            m->sub++;
            break;
        }
        c->w146 = 0;
        m->sub = 1;
        m->timer = 1;
        break;
    case 7:
        if (--m->timer != 0) break;
        m->sub++;
        break;
    case 8:
        func_800EBD14((unsigned char)c->slot[D_800F30A4[m->cursor]]);
        func_80058874(D_800F3040, D_800F2A78[m->card]);
        func_800588E8(D_800F3040, D_800EA218, 0xC);
        func_800588E8(D_800F3040, D_800F3060, 8);
        if (func_800EA334(c, m->card) == 0) {
            if (memCardWriteFile(D_800F3040, D_800E38F8, 0xC00, 1)) {
                m->msg = 0;
                m->sub = 11;
                break;
            }
        }
        c->w146 = 0;
        m->sub = 0;
        m->w4 = 1;
        m->timer = 1;
        break;
    case 9:
        if (--m->timer != 0) break;
        m->sub++;
        break;
    case 10:
        func_80058874(D_800F3040, D_800F2A78[m->card]);
        tries = 3;
        func_800588E8(D_800F3040, c->e[D_800F30A4[m->cursor]].name, 0x14);
        do {
            if (memCardReadFile(D_800F3040, D_800E38F8, 0, 0xC00) == 0 && (memCardVerifyChecksum(D_800E38F8 + 0x200, 0xA00) & 0xFF)) {
                D_8009E508 = *(GS *)(D_800E38F8 + 0x200);
                c->w146 = 0;
                m->sub = 0;
                m->timer = 1;
                return 1;
            }
        } while (--tries > 0);
        m->msg = 0;
        m->sub = 11;
        break;
    case 11:
        if (JS_ & 0x50) {
            func_8001E388(0x1B, 0x1C);
            c->w146 = 0;
            m->sub = 1;
            m->timer = 1;
        }
        break;
    case 12:
        m->msg = 14;
        m->sub++;
        break;
    case 13:
        btn = JS_;
        if (btn & 0x40) {
            func_8001E388(0x1B, 0x1C);
            m->sub = 2;
        } else if (btn & 0x10) {
            m->msg = 10;
            m->timer = 4;
            func_8001E388(0xA, 0xA);
            m->sub = 9;
        }
        break;
    case 14:
        m->msg = 14;
        m->sub++;
        break;
    case 15:
        btn = JS_;
        if (btn & 0x40) {
            func_8001E388(0x1B, 0x1C);
            m->sub = 2;
            break;
        }
        if (btn & 0x10) {
            func_80059270(D_800F3060, D_800EA228, (unsigned char)c->slot[D_800F30A4[m->cursor]]);
            m->msg = 6;
            m->timer = 4;
            func_8001E388(0xA, 0xA);
            m->sub = 7;
            break;
        }
        return 0;
    }
    return 0;
}
#endif
