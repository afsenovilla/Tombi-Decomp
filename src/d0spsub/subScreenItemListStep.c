// FUNC 800ed0c4 3488 D0SPSUB
// MATCHING 800ed0c4 3488
// Matching debt: v1 holds the table pointer / later value 3, a1 holds the second
// byte, and a single empty asm input keeps the pointer live through that load.
// The v0-bound object code is materialized before the null check so 0x54 fills
// its delay slot, with message 14 loaded only after the optional object stores.
// The item-handler result 2 returns 1 here: the retail jump-table entry at
// 0x800E8658 targets 0x800ED80C, not the return-0 path of the earlier WIP draft.
// Code and all three switch tables were checked after linking, without masking.
// Historical WIP diagnosis below records the pre-match allocator differences.
// Historical WIP: score 20 (was 30), case-3 confirm block only. Fixed: `int x = D_80111128_v` (second extern name declared
// volatile int; a plain scalar sinks to its first use) loads D_80111128 into a0 before the t lbus, and
// `a = t[0]; D_80111134 = a; b = t[1];` keeps the first store between the two lbus. The `li 3` position: the 3 is
// the SAME pseudo as t (`t = (unsigned char *)3; GAME_PLAYER_MODE = (int)t; PLAYER = (int)t;`): a 2-set pseudo is not a
// "birthing" insn for sched1 (sched.c birthing_insn_p needs REG_N_SETS == 1), so `li 3` sinks to the earliest free
// cycle = right after `lbu b,1(t)` (anti-dependence on t), exactly the game's spot, and the whole schedule now matches.
// Left (score 20 = registers only): t/3 "dies in 2 places" -> global alloc, so local-alloc sees v1 free and gives
// b (local) v1; the global t/3 then gets a1. The game has t/3 in v1 and b in a1, i.e. b must ALSO be global (used
// in a second block or dying twice) and lower priority than t/3 (global alloc then gives t/3 v1 and b a1). The
// delay-slot difference (`li v0,0x54` in the beqz slot, `li a0,14` left at the label) is a consequence of the same
// registers. Tried for b: a dummy use in the if/else arm (`if (b & 0x100)`, `0x54 | (b >> 8)`, `7 | (b >> 8)`: the
// front end folds the & form away (byte & 0x100), the >> forms generate code: 171-193); a function-scope
// `short *q` shared with the case-1 bits loop (`*q++ = n; z++`) changes the loop's giv (34 for the loop alone,
// 183 shared); `i2 = t[1]` (177). Untried: a natural second use of b in the x == 14 arm or after the join.
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
extern unsigned char D_8010A0C8[];
extern unsigned char D_800FFD84[][5];
extern unsigned char D_800FFC64[];
extern unsigned char D_800FECE8[];
extern Cond *D_800FFAF8[];
extern int (*D_800FFBB0[])(int a, int b, int c);
extern unsigned char *D_800FFC3C[];
extern short D_80109D88[], D_80109D98[], D_80109DB0[];
extern short D_8010A058, D_8010A05C, D_8010A060, D_8010A064;
extern short D_8010A068[], D_8010A088[];
extern unsigned char D_8010A200[];
extern unsigned char D_8010A204;
extern int D_80111128;
extern volatile int D_80111128_v;
extern char D_80111134, D_80111138;
extern volatile short D_8009CD94, D_8009CD96, D_8009CDA0;
extern volatile unsigned char GAME_PLAYER_MODE, PLAYER, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP, GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID;
extern int subScreenMoveTabCursor(Menu *m, int n);
extern int subScreenMoveGridCursor(Menu *m);
extern int subScreenMoveGridCursorN(Menu *m, int n);
extern int func_800E89F8(int id);
extern void subScreenOpenMessage(int kind, int add, int bank);
extern void subScreenMessageStep(void *o, int a);
extern Obj *allocObjectLayer2(void);
extern void printInfoMessage(int id, int kind);
extern void playSFX(int id);
extern void playSFXAndSetNote(int a, int b);
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
            for (n = 0; n < D_8009D1A4.count; n++) D_8010A0C8[n] = D_8009D1A4.items[n];
        }
        m->state++;
    case 1:
        if (subScreenMoveTabCursor(m, 1)) return 0;
        pages = (D_8009D1A4.count + 9) / 10;
        if (JOYPAD_STATE & 0x400) {
            if (m->page > 0) {
                playSFX(8);
                m->page--;
            }
        } else if (JOYPAD_STATE & 0x800) {
            if (m->page < pages - 1) {
                playSFX(8);
                m->page++;
            }
        } else if (JOYPAD_STATE & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return 1;
        } else if (JOYPAD_STATE & 0x4008) {
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
                            int c = D_800FFD84[md][j];
                            for (i = 0; i < 0x11E; i += 2) {
                                if (D_800FFC64[i] == c && D_8009D0A4[D_800FFC64[i + 1]]) {
                                    D_8009D1A4.items[k++] = D_800FFC64[i + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009D2A4; n++) D_8010A0C8[n] = D_8009D1A4.items[n]; }
                } else {
                    if (++D_8009D1A4.mode >= 4) {
                        D_8009D1A4.mode = 0;
                    }
                    k = 0;
                    {
                        md = D_8009D1A4.mode;
                        for (j2 = 0; j2 < 5; j2++) {
                            int c = D_800FFD84[md][j2];
                            for (i2 = 0; i2 < 0x11E; i2 += 2) {
                                if (D_800FFC64[i2] == c && D_8009D0A4[D_800FFC64[i2 + 1]]) {
                                    D_8009D1A4.items[k++] = D_800FFC64[i2 + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009D2A4; n++) D_8010A0C8[n] = D_8009D1A4.items[n]; }
                }
                break;
            case 4:
                m->mode = 1;
                m->cursor = 0;
                m->sel = -1;
                m->state = 0;
                break;
            case 5:
                m->mode = 2;
                m->cursor = 0;
                m->sel = -1;
                m->state = 0;
                break;
            case 6:
                m->mode = 0;
                m->state = 0;
                break;
            case 7:
                m->mode = 3;
                m->state = 0;
                playSFXAndSetNote(10, 10);
                return 2;
            case 8:
            case 9:
            case 10:
            case 11:
                SfxPlay2(10, 10);
                return 1;
            }
            playSFXAndSetNote(10, 10);
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
        } else if ((JOYPAD_STATE & 0x4008) && m->cursor == 0) {
            Cond *c, *base;
            int r;
            D_80111128 = D_8010A0C8[m->sel];
            c = D_800FFAF8[D_800FECE8[D_80111128]];
            base = c;
            for (;;) {
                r = D_800FFBB0[c->fn](c->a, c->b, c->c);
                if (c->next == -1) break;
                if (r) c = base + c->next;
                else c++;
            }
            if (r) {
                if (D_80111128 == 7 || D_80111128 == 14) {
                    D_8010A058 = 0;
                    {
                    int n, z;
                    z = 0;
                    for (n = 0; n < 9; n++) {
                        if (D_8009D2C4[n]) D_8010A068[z++] = n;
                    }
                    D_8010A05C = z;
                    }
                    D_8010A060 = -1;
                    SfxPlay2(10, 10);
                    m->sub = 0;
                    m->state++;
                } else {
                    switch (func_800E89F8(D_80111128)) {
                    case 1:
                        {
                            int n = 4;
                            int a = D_80111128;
                            if (a != 0x14 && a != 0x15 && a != 0x16 && a != 0x17
                                && a != 0x1B && a != 0x21 && a != 0x22 && a != 0x20
                                && a != 0x1C && a != 0x1D) {
                                n = 1;
                                if (a == 0x18) {
                                    a = *(int *)((char *)&D_80111128 + 0);
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
                int x = D_80111128;
                int w;
                for (q = D_80109D88; *q != -1; q++) {
                    if (x == *q) {
                        w = 0;
                        goto found;
                    }
                }
                for (q = D_80109D98; *q != -1; q++) {
                    if (x == *q) {
                        w = 1;
                        goto found;
                    }
                }
                for (q = D_80109DB0; *q != -1; q++) {
                    if (x == *q) {
                        if (D_8009D0E6 < 5) w = 3;
                        else w = 2;
                        goto found;
                    }
                }
                w = 2;
            found:
                subScreenOpenMessage(0, w, 2);
                D_8010A204 = 0;
                SfxPlay2(10, 10);
                m->state = 6;
            }
        }
        return 0;
    case 3:
        switch (m->sub) {
        case 0:
            D_8010A060 = -1;
            m->sub++;
        case 1:
            if (JOYPAD_STATE & 0x10) {
                if (D_8010A058 > 0) {
                    D_8010A058--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                if (D_8010A058 < D_8010A05C - 1) {
                    D_8010A058++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x2000) {
                D_8010A058 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->state--;
            } else if (JOYPAD_STATE & 0x4008) {
                unsigned short bits;
                z = 0;
                bits = D_8009D2C4[D_8010A068[D_8010A058]];
                for (n = 0; n < 9; n++) {
                    if (bits & 1) D_8010A088[z++] = n;
                    bits >>= 1;
                }
                D_8010A064 = z;
                D_8010A060 = 0;
                SfxPlay2(10, 10);
                m->sub++;
            }
            break;
        case 2:
            if (JOYPAD_STATE & 0x10) {
                if (D_8010A060 > 0) {
                    D_8010A060--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                if (D_8010A060 < D_8010A064 - 1) {
                    D_8010A060++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x2000) {
                D_8010A060 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->sub--;
            } else if (JOYPAD_STATE & 0x4008) {
                unsigned char *tp;
                int n;
                int i1 = D_8010A068[D_8010A058], i2 = D_8010A088[D_8010A060];
                int x = D_80111128_v;
                tp = D_800FFC3C[i1] + i2 * 2;
                {
                register unsigned char *t __asm__("$3") = tp;
                int a = t[0];
                register int b __asm__("$5");
                D_80111134 = a;
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
                D_80111138 = b;
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
                D_8010A058 = -1;
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
        } else if (JOYPAD_STATE & 0x4008) {
            subScreenOpenMessage(1, D_8010A0C8[m->sel], 2);
            D_8010A204 = 0;
            SfxPlay2(10, 10);
            m->state++;
        }
        break;
    case 5:
        subScreenMessageStep(D_8010A200, 2);
        if (D_8010A204 == 2) m->state = 4;
        return 0;
    case 6:
        subScreenMessageStep(D_8010A200, 2);
        if (D_8010A204 == 2) m->state = 2;
        return 0;
    }
    return 0;
}
