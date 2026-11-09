// VERSION SCUS_94236 dspsub3 800EBAD4 3288
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub3 800ec89c (subScreenItemListStep.c)
// D0SPSUB3 twin of D0SPSUB func_800ED0C4 (same source, five retail differences): state 1 calls
// subScreenMoveTabCursor(m) (not (m, 1)), the confirm mask is 0x5008 (not 0x4008), the cursor switch of state 1 has
// no mode cases 4..7 (its jump table at 0x800E85F4 sends 3..7 to the default tail, which calls SfxPlay2(10, 10) here,
// not playSFXAndSetNote) and 8..11 to the SfxPlay2/return 1 case, the page change of state 1 is stored BEFORE
// playSFX(8) with an explicit `return 0` in both arms (without it the two `sh` cross-jump into one block and the frame
// grows to 192), and state 4 calls playSFX(8) BEFORE the page/sel update (so it no longer cross-jumps with state 2).
// Carries the func_800ED0C4 match idioms: v1 holds the table pointer / later value 3, a1 holds the second byte, an
// empty asm input keeps the pointer live through that load, the v0-bound object code 0x54 is materialized before
// the allocObjectLayer2 null check, and the item-handler result 2 returns 1 (the retail jump table at 0x800E8658
// targets the return-1 path). Code and switch tables checked after linking, without masking.
typedef struct { unsigned char b0, state, sub, p3; char p4[0xE]; short mode; short page; short cursor; char p18[6]; short sel; } Menu;
typedef struct { short next; unsigned char fn, a; short b, c; } Cond;
typedef struct { unsigned char items[0x100]; unsigned short count, mode; } Inv;
typedef struct { unsigned char b0, b1, b2, b3; } Obj;
extern unsigned short JOYPAD_STATE;
extern Inv D_8009C50C;
extern unsigned short D_8009C60C;
extern unsigned char D_8009C40C[];
extern unsigned char D_8009C44E;
extern unsigned short D_8009C62C[];
extern unsigned char D_800FFC80[];
extern unsigned char D_800F5A8C[][5];
extern unsigned char D_800F596C[];
extern unsigned char D_800F49F0[];
extern Cond *D_800F5800[];
extern int (*D_800F58B8[])(int a, int b, int c);
extern unsigned char *D_800F5944[];
extern short D_800FFBE4[], D_800FFBF4[], D_800FFC0C[];
extern short D_800FFC10, D_800FFC14, D_800FFC18, D_800FFC1C;
extern short D_800FFC20[], D_800FFC40[];
extern unsigned char D_800FFD88[];
extern unsigned char D_800FFD8C;
extern int D_800FFE5C;
extern volatile int D_80100B64_v;
extern char D_800FFE60, D_800FFE64;
extern volatile short D_8009C0FC, D_8009C0FE, D_8009C108;
extern volatile unsigned char GAME_PLAYER_MODE, PLAYER, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP, GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID;
extern int subScreenMoveTabCursor(Menu *m);
extern int subScreenMoveGridCursor(Menu *m);
extern int subScreenMoveGridCursorN(Menu *m, int n);
extern int func_800E7CB0(int id);
extern void subScreenOpenMessage(int kind, int add, int bank);
extern void subScreenMessageStep(void *o, int a);
extern Obj *allocObjectLayer2(void);
extern void printInfoMessage(int id, int kind);
extern void playSFX(int id);
extern void SfxPlay2(int a, int b);

int subScreenItemListStep(Menu *m)
{
    int pages;
    int i, j, j2, k;
    int c14; int z, n; int i2;
    unsigned short md;
    switch (m->state) {
    case 0:
        m->cursor = 0;
        m->page = 0;
        {
            int n;
            for (n = 0; n < D_8009C50C.count; n++) D_800FFC80[n] = D_8009C50C.items[n];
        }
        m->state++;
    case 1:
        if (subScreenMoveTabCursor(m)) return 0;
        pages = (D_8009C50C.count + 7) / 8;
        if (JOYPAD_STATE & 0x400) {
            if (m->page > 0) {
                m->page--;
                playSFX(8);
                return 0;
            }
        } else if (JOYPAD_STATE & 0x800) {
            if (m->page < pages - 1) {
                m->page++;
                playSFX(8);
                return 0;
            }
        } else if (JOYPAD_STATE & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return 1;
        } else if (JOYPAD_STATE & 0x5008) {
            switch (m->cursor) {
            case 0:
                if (D_8009C50C.count) {
                    m->sel = m->page * 8;
                    m->state++;
                }
                break;
            case 1:
                if (D_8009C50C.count) {
                    m->state = 4;
                    m->sel = m->page * 8;
                }
                break;
            case 2:
                md = D_8009C50C.mode;
                if (md & 0x8000) {
                    k = 0;
                    md &= 0x7FFF;
                    D_8009C50C.mode = md;
                    {
                        for (j = 0; j < 5; j++) {
                            int c = D_800F5A8C[md][j];
                            for (i = 0; i < 0x11E; i += 2) {
                                if (D_800F596C[i] == c && D_8009C40C[D_800F596C[i + 1]]) {
                                    D_8009C50C.items[k++] = D_800F596C[i + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009C60C; n++) D_800FFC80[n] = D_8009C50C.items[n]; }
                } else {
                    if (++D_8009C50C.mode >= 4) {
                        D_8009C50C.mode = 0;
                    }
                    k = 0;
                    {
                        md = D_8009C50C.mode;
                        for (j2 = 0; j2 < 5; j2++) {
                            int c = D_800F5A8C[md][j2];
                            for (i2 = 0; i2 < 0x11E; i2 += 2) {
                                if (D_800F596C[i2] == c && D_8009C40C[D_800F596C[i2 + 1]]) {
                                    D_8009C50C.items[k++] = D_800F596C[i2 + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009C60C; n++) D_800FFC80[n] = D_8009C50C.items[n]; }
                }
                break;
            case 8:
            case 9:
            case 10:
            case 11:
                SfxPlay2(10, 10);
                return 1;
            }
            SfxPlay2(10, 10);
        }
        return 0;
    case 2:
        if (subScreenMoveGridCursor(m)) return 0;
        pages = (D_8009C50C.count + 7) / 8;
        if (JOYPAD_STATE & 0x400) {
            if (m->page > 0) {
                m->page--;
                m->sel -= 8;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x800) {
            if (m->page < pages - 1) {
                m->page++;
                m->sel += 8;
                if (m->sel >= D_8009C50C.count) m->sel = D_8009C50C.count - 1;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x2000) {
            m->sel = -1;
            SfxPlay2(0x1B, 0x1C);
            m->state = 1;
        } else if ((JOYPAD_STATE & 0x5008) && m->cursor == 0) {
            Cond *c, *base;
            int r;
            D_800FFE5C = D_800FFC80[m->sel];
            c = D_800F5800[D_800F49F0[D_800FFE5C]];
            base = c;
            for (;;) {
                r = D_800F58B8[c->fn](c->a, c->b, c->c);
                if (c->next == -1) break;
                if (r) c = base + c->next;
                else c++;
            }
            if (r) {
                if (D_800FFE5C == 7 || D_800FFE5C == 14) {
                    D_800FFC10 = 0;
                    {
                    int n, z;
                    z = 0;
                    for (n = 0; n < 9; n++) {
                        if (D_8009C62C[n]) D_800FFC20[z++] = n;
                    }
                    D_800FFC14 = z;
                    }
                    D_800FFC18 = -1;
                    SfxPlay2(10, 10);
                    m->sub = 0;
                    m->state++;
                } else {
                    switch (func_800E7CB0(D_800FFE5C)) {
                    case 1:
                        printInfoMessage(D_800FFE5C, 1);
                        return 1;
                    case 2:
                        return 1;
                    case 3:
                        printInfoMessage(0x1E, 2);
                        return 1;
                    case 4:
                        printInfoMessage(0x1F, 2);
                        return 1;
                    case 5:
                        printInfoMessage(0x20, 2);
                        return 1;
                    }
                }
            } else {
                short *q;
                int x = D_800FFE5C;
                int w;
                for (q = D_800FFBE4; *q != -1; q++) {
                    if (x == *q) {
                        w = 0;
                        goto found;
                    }
                }
                for (q = D_800FFBF4; *q != -1; q++) {
                    if (x == *q) {
                        w = 1;
                        goto found;
                    }
                }
                for (q = D_800FFC0C; *q != -1; q++) {
                    if (x == *q) {
                        if (D_8009C44E < 5) w = 3;
                        else w = 2;
                        goto found;
                    }
                }
                w = 2;
            found:
                subScreenOpenMessage(0, w, 2);
                D_800FFD8C = 0;
                SfxPlay2(10, 10);
                m->state = 6;
            }
        }
        return 0;
    case 3:
        switch (m->sub) {
        case 0:
            D_800FFC18 = -1;
            m->sub++;
        case 1:
            if (JOYPAD_STATE & 0x10) {
                if (D_800FFC10 > 0) {
                    D_800FFC10--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                if (D_800FFC10 < D_800FFC14 - 1) {
                    D_800FFC10++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x2000) {
                D_800FFC10 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->state--;
            } else if (JOYPAD_STATE & 0x5008) {
                unsigned short bits;
                z = 0;
                bits = D_8009C62C[D_800FFC20[D_800FFC10]];
                for (n = 0; n < 9; n++) {
                    if (bits & 1) D_800FFC40[z++] = n;
                    bits >>= 1;
                }
                D_800FFC1C = z;
                D_800FFC18 = 0;
                SfxPlay2(10, 10);
                m->sub++;
            }
            break;
        case 2:
            if (JOYPAD_STATE & 0x10) {
                if (D_800FFC18 > 0) {
                    D_800FFC18--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                if (D_800FFC18 < D_800FFC1C - 1) {
                    D_800FFC18++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x2000) {
                D_800FFC18 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->sub--;
            } else if (JOYPAD_STATE & 0x5008) {
                unsigned char *tp;
                int n;
                int i1 = D_800FFC20[D_800FFC10], i2 = D_800FFC40[D_800FFC18];
                int x = D_80100B64_v;
                tp = D_800F5944[i1] + i2 * 2;
                {
                register unsigned char *t __asm__("$3") = tp;
                int a = t[0];
                register int b __asm__("$5");
                D_800FFE60 = a;
                b = t[1];
                __asm__("" : : "r"(t));
                D_8009C0FC = a;
                PLAYER_SUBSTATE = 0x30;
                D_8009C108 = 0;
                t = (unsigned char *)3;
                GAME_PLAYER_MODE = (int)t;
                PLAYER_STATE = 1;
                PLAYER_STEP = 0;
                PLAYER = (int)t;
                GVARS_USE_ITEM_PENDING = 1;
                GVARS_USE_ITEM_ID = x;
                D_800FFE64 = b;
                D_8009C0FE = b;
                }
                if (x == 14) {
                    Obj *o = allocObjectLayer2();
                    register int code __asm__("$2") = 0x54;
                    if (o) {
                        o->b0 = 1;
                        o->b2 = code;
                        o->b3 = 1;
                    }
                    printInfoMessage(14, 1);
                } else {
                    printInfoMessage(7, 1);
                }
                D_800FFC10 = -1;
                SfxPlay2(10, 10);
                return 1;
            }
            break;
        }
        break;
    case 4:
        if (subScreenMoveGridCursorN(m, D_8009C50C.count)) return 0;
        pages = (D_8009C50C.count + 7) / 8;
        if (JOYPAD_STATE & 0x400) {
            if (m->page > 0) {
                playSFX(8);
                m->page--;
                m->sel -= 8;
            }
        } else if (JOYPAD_STATE & 0x800) {
            if (m->page < pages - 1) {
                playSFX(8);
                m->page++;
                m->sel += 8;
                if (m->sel >= D_8009C50C.count) m->sel = D_8009C50C.count - 1;
            }
        } else if (JOYPAD_STATE & 0x2000) {
            m->sel = -1;
            SfxPlay2(0x1B, 0x1C);
            m->state = 1;
        } else if (JOYPAD_STATE & 0x5008) {
            subScreenOpenMessage(1, D_800FFC80[m->sel], 2);
            D_800FFD8C = 0;
            SfxPlay2(10, 10);
            m->state++;
        }
        break;
    case 5:
        subScreenMessageStep(D_800FFD88, 2);
        if (D_800FFD8C == 2) m->state = 4;
        return 0;
    case 6:
        subScreenMessageStep(D_800FFD88, 2);
        if (D_800FFD8C == 2) m->state = 2;
        return 0;
    }
    return 0;
}
#endif
