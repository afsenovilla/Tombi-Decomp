// FUNC 800ec89c 3424 D0SPSUB3
// MATCHING 800ec89c 3424
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
extern Inv D_8009D1A4;
extern unsigned short D_8009D2A4;
extern unsigned char D_8009D0A4[];
extern unsigned char D_8009D0E6;
extern unsigned short D_8009D2C4[];
extern unsigned char D_80100988[];
extern unsigned char D_800F68E8[][5];
extern unsigned char D_800F67C8[];
extern unsigned char D_800F584C[];
extern Cond *D_800F665C[];
extern int (*D_800F6714[])(int a, int b, int c);
extern unsigned char *D_800F67A0[];
extern short D_801008EC[], D_801008FC[], D_80100914[];
extern short D_80100918, D_8010091C, D_80100920, D_80100924;
extern short D_80100928[], D_80100948[];
extern unsigned char D_80100A90[];
extern unsigned char D_80100A94;
extern int D_80100B64;
extern volatile int D_80100B64_v;
extern char D_80100B68, D_80100B6C;
extern volatile short D_8009CD94, D_8009CD96, D_8009CDA0;
extern volatile unsigned char GAME_PLAYER_MODE, PLAYER, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP, GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID;
extern int subScreenMoveTabCursor(Menu *m);
extern int subScreenMoveGridCursor(Menu *m);
extern int subScreenMoveGridCursorN(Menu *m, int n);
extern int func_800E8950(int id);
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
            for (n = 0; n < D_8009D1A4.count; n++) D_80100988[n] = D_8009D1A4.items[n];
        }
        m->state++;
    case 1:
        if (subScreenMoveTabCursor(m)) return 0;
        pages = (D_8009D1A4.count + 9) / 10;
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
                if (D_8009D1A4.count) {
                    m->sel = m->page * 10;
                    m->state++;
                }
                break;
            case 1:
                if (D_8009D1A4.count) {
                    m->state = 4;
                    m->sel = m->page * 10;
                }
                break;
            case 2:
                md = D_8009D1A4.mode;
                if (md & 0x8000) {
                    k = 0;
                    md &= 0x7FFF;
                    D_8009D1A4.mode = md;
                    {
                        for (j = 0; j < 5; j++) {
                            int c = D_800F68E8[md][j];
                            for (i = 0; i < 0x11E; i += 2) {
                                if (D_800F67C8[i] == c && D_8009D0A4[D_800F67C8[i + 1]]) {
                                    D_8009D1A4.items[k++] = D_800F67C8[i + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009D2A4; n++) D_80100988[n] = D_8009D1A4.items[n]; }
                } else {
                    if (++D_8009D1A4.mode >= 4) {
                        D_8009D1A4.mode = 0;
                    }
                    k = 0;
                    {
                        md = D_8009D1A4.mode;
                        for (j2 = 0; j2 < 5; j2++) {
                            int c = D_800F68E8[md][j2];
                            for (i2 = 0; i2 < 0x11E; i2 += 2) {
                                if (D_800F67C8[i2] == c && D_8009D0A4[D_800F67C8[i2 + 1]]) {
                                    D_8009D1A4.items[k++] = D_800F67C8[i2 + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009D2A4; n++) D_80100988[n] = D_8009D1A4.items[n]; }
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
        pages = (D_8009D1A4.count + 9) / 10;
        if (JOYPAD_STATE & 0x400) {
            if (m->page > 0) {
                m->page--;
                m->sel -= 10;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x800) {
            if (m->page < pages - 1) {
                m->page++;
                m->sel += 10;
                if (m->sel >= D_8009D1A4.count) m->sel = D_8009D1A4.count - 1;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x2000) {
            m->sel = -1;
            SfxPlay2(0x1B, 0x1C);
            m->state = 1;
        } else if ((JOYPAD_STATE & 0x5008) && m->cursor == 0) {
            Cond *c, *base;
            int r;
            D_80100B64 = D_80100988[m->sel];
            c = D_800F665C[D_800F584C[D_80100B64]];
            base = c;
            for (;;) {
                r = D_800F6714[c->fn](c->a, c->b, c->c);
                if (c->next == -1) break;
                if (r) c = base + c->next;
                else c++;
            }
            if (r) {
                if (D_80100B64 == 7 || D_80100B64 == 14) {
                    D_80100918 = 0;
                    {
                    int n, z;
                    z = 0;
                    for (n = 0; n < 9; n++) {
                        if (D_8009D2C4[n]) D_80100928[z++] = n;
                    }
                    D_8010091C = z;
                    }
                    D_80100920 = -1;
                    SfxPlay2(10, 10);
                    m->sub = 0;
                    m->state++;
                } else {
                    switch (func_800E8950(D_80100B64)) {
                    case 1:
                        {
                            int n = 4;
                            int a = D_80100B64;
                            if (a != 0x14 && a != 0x15 && a != 0x16 && a != 0x17
                                && a != 0x1B && a != 0x21 && a != 0x22 && a != 0x20
                                && a != 0x1C && a != 0x1D) {
                                n = 1;
                                if (a == 0x18) {
                                    a = *(int *)((char *)&D_80100B64 + 0);
                                    n = 4;
                                }
                            }
                            printInfoMessage(a, n);
                        }
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
                int x = D_80100B64;
                int w;
                for (q = D_801008EC; *q != -1; q++) {
                    if (x == *q) {
                        w = 0;
                        goto found;
                    }
                }
                for (q = D_801008FC; *q != -1; q++) {
                    if (x == *q) {
                        w = 1;
                        goto found;
                    }
                }
                for (q = D_80100914; *q != -1; q++) {
                    if (x == *q) {
                        if (D_8009D0E6 < 5) w = 3;
                        else w = 2;
                        goto found;
                    }
                }
                w = 2;
            found:
                subScreenOpenMessage(0, w, 2);
                D_80100A94 = 0;
                SfxPlay2(10, 10);
                m->state = 6;
            }
        }
        return 0;
    case 3:
        switch (m->sub) {
        case 0:
            D_80100920 = -1;
            m->sub++;
        case 1:
            if (JOYPAD_STATE & 0x10) {
                if (D_80100918 > 0) {
                    D_80100918--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                if (D_80100918 < D_8010091C - 1) {
                    D_80100918++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x2000) {
                D_80100918 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->state--;
            } else if (JOYPAD_STATE & 0x5008) {
                unsigned short bits;
                z = 0;
                bits = D_8009D2C4[D_80100928[D_80100918]];
                for (n = 0; n < 9; n++) {
                    if (bits & 1) D_80100948[z++] = n;
                    bits >>= 1;
                }
                D_80100924 = z;
                D_80100920 = 0;
                SfxPlay2(10, 10);
                m->sub++;
            }
            break;
        case 2:
            if (JOYPAD_STATE & 0x10) {
                if (D_80100920 > 0) {
                    D_80100920--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                if (D_80100920 < D_80100924 - 1) {
                    D_80100920++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x2000) {
                D_80100920 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->sub--;
            } else if (JOYPAD_STATE & 0x5008) {
                unsigned char *tp;
                int n;
                int i1 = D_80100928[D_80100918], i2 = D_80100948[D_80100920];
                int x = D_80100B64_v;
                tp = D_800F67A0[i1] + i2 * 2;
                {
                register unsigned char *t __asm__("$3") = tp;
                int a = t[0];
                register int b __asm__("$5");
                D_80100B68 = a;
                b = t[1];
                __asm__("" : : "r"(t));
                D_8009CD94 = a;
                PLAYER_SUBSTATE = 0x30;
                D_8009CDA0 = 0;
                t = (unsigned char *)3;
                GAME_PLAYER_MODE = (int)t;
                PLAYER_STATE = 1;
                PLAYER_STEP = 0;
                PLAYER = (int)t;
                GVARS_USE_ITEM_PENDING = 1;
                GVARS_USE_ITEM_ID = x;
                D_80100B6C = b;
                D_8009CD96 = b;
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
                D_80100918 = -1;
                SfxPlay2(10, 10);
                return 1;
            }
            break;
        }
        break;
    case 4:
        if (subScreenMoveGridCursorN(m, D_8009D1A4.count)) return 0;
        pages = (D_8009D1A4.count + 9) / 10;
        if (JOYPAD_STATE & 0x400) {
            if (m->page > 0) {
                playSFX(8);
                m->page--;
                m->sel -= 10;
            }
        } else if (JOYPAD_STATE & 0x800) {
            if (m->page < pages - 1) {
                playSFX(8);
                m->page++;
                m->sel += 10;
                if (m->sel >= D_8009D1A4.count) m->sel = D_8009D1A4.count - 1;
            }
        } else if (JOYPAD_STATE & 0x2000) {
            m->sel = -1;
            SfxPlay2(0x1B, 0x1C);
            m->state = 1;
        } else if (JOYPAD_STATE & 0x5008) {
            subScreenOpenMessage(1, D_80100988[m->sel], 2);
            D_80100A94 = 0;
            SfxPlay2(10, 10);
            m->state++;
        }
        break;
    case 5:
        subScreenMessageStep(D_80100A90, 2);
        if (D_80100A94 == 2) m->state = 4;
        return 0;
    case 6:
        subScreenMessageStep(D_80100A90, 2);
        if (D_80100A94 == 2) m->state = 2;
        return 0;
    }
    return 0;
}
