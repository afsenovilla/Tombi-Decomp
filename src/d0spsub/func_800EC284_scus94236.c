// VERSION SCUS_94236 dspsub 800EC284 3352
#if defined(VERSION_SCUS_94236)
// USA version of PAL d0spsub 800ed0c4 (subScreenItemListStep.c)
// Matching debt: v1 holds the table pointer / later value 3, a1 holds the second
// byte, and a single empty asm input keeps the pointer live through that load.
// The v0-bound object code is materialized before the null check so 0x54 fills
// its delay slot, with message 14 loaded only after the optional object stores.
// The item-handler result 2 returns 1 here: the retail jump-table entry at
// 0x800E8658 targets 0x800ED80C, not the return-0 path of the earlier WIP draft.
// Code and all three switch tables were checked after linking, without masking.
// Historical WIP diagnosis below records the pre-match allocator differences.
// Historical WIP: score 20 (was 30), case-3 confirm block only. Fixed: `int x = D_80111128_v` (second extern name declared
// volatile int; a plain scalar sinks to its first use) loads D_801102F8 into a0 before the t lbus, and
// `a = t[0]; D_80110304 = a; b = t[1];` keeps the first store between the two lbus. The `li 3` position: the 3 is
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
extern Inv D_8009C50C;
extern unsigned short D_8009C60C;
extern unsigned char D_8009C40C[];
extern unsigned char D_8009C44E;
extern unsigned short D_8009C62C[];
extern unsigned char D_80109298[];
extern unsigned char D_800FEE30[][5];
extern unsigned char D_800FED10[];
extern unsigned char D_800FDD94[];
extern Cond *D_800FEBA4[];
extern int (*D_800FEC5C[])(int a, int b, int c);
extern unsigned char *D_800FECE8[];
extern short D_80108F58[], D_80108F68[], D_80108F80[];
extern short D_80109228, D_8010922C, D_80109230, D_80109234;
extern short D_80109238[], D_80109258[];
extern unsigned char D_801093D0[];
extern unsigned char D_801093D4;
extern int D_801102F8;
extern volatile int D_80111128_v;
extern char D_80110304, D_80110308;
extern volatile short D_8009C0FC, D_8009C0FE, D_8009C108;
extern volatile unsigned char GAME_PLAYER_MODE, PLAYER, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP, GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID;
extern int subScreenMoveTabCursor(Menu *m, int n);
extern int subScreenMoveGridCursor(Menu *m);
extern int subScreenMoveGridCursorN(Menu *m, int n);
extern int func_800E7D58(int id);
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
            for (n = 0; n < D_8009C50C.count; n++) D_80109298[n] = D_8009C50C.items[n];
        }
        m->state++;
    case 1:
        if (subScreenMoveTabCursor(m, 1)) return 0;
        pages = (D_8009C50C.count + 7) / 8;
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
                            int c = D_800FEE30[md][j];
                            for (i = 0; i < 0x11E; i += 2) {
                                if (D_800FED10[i] == c && D_8009C40C[D_800FED10[i + 1]]) {
                                    D_8009C50C.items[k++] = D_800FED10[i + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009C60C; n++) D_80109298[n] = D_8009C50C.items[n]; }
                } else {
                    if (++D_8009C50C.mode >= 4) {
                        D_8009C50C.mode = 0;
                    }
                    k = 0;
                    {
                        md = D_8009C50C.mode;
                        for (j2 = 0; j2 < 5; j2++) {
                            int c = D_800FEE30[md][j2];
                            for (i2 = 0; i2 < 0x11E; i2 += 2) {
                                if (D_800FED10[i2] == c && D_8009C40C[D_800FED10[i2 + 1]]) {
                                    D_8009C50C.items[k++] = D_800FED10[i2 + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009C60C; n++) D_80109298[n] = D_8009C50C.items[n]; }
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
        } else if ((JOYPAD_STATE & 0x4008) && m->cursor == 0) {
            Cond *c, *base;
            int r;
            D_801102F8 = D_80109298[m->sel];
            c = D_800FEBA4[D_800FDD94[D_801102F8]];
            base = c;
            for (;;) {
                r = D_800FEC5C[c->fn](c->a, c->b, c->c);
                if (c->next == -1) break;
                if (r) c = base + c->next;
                else c++;
            }
            if (r) {
                if (D_801102F8 == 7 || D_801102F8 == 14) {
                    D_80109228 = 0;
                    {
                    int n, z;
                    z = 0;
                    for (n = 0; n < 9; n++) {
                        if (D_8009C62C[n]) D_80109238[z++] = n;
                    }
                    D_8010922C = z;
                    }
                    D_80109230 = -1;
                    SfxPlay2(10, 10);
                    m->sub = 0;
                    m->state++;
                } else {
                    switch (func_800E7D58(D_801102F8)) {
                    case 1:
                        printInfoMessage(D_801102F8, 1);
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
                int x = D_801102F8;
                int w;
                for (q = D_80108F58; *q != -1; q++) {
                    if (x == *q) {
                        w = 0;
                        goto found;
                    }
                }
                for (q = D_80108F68; *q != -1; q++) {
                    if (x == *q) {
                        w = 1;
                        goto found;
                    }
                }
                for (q = D_80108F80; *q != -1; q++) {
                    if (x == *q) {
                        if (D_8009C44E < 5) w = 3;
                        else w = 2;
                        goto found;
                    }
                }
                w = 2;
            found:
                subScreenOpenMessage(0, w, 2);
                D_801093D4 = 0;
                SfxPlay2(10, 10);
                m->state = 6;
            }
        }
        return 0;
    case 3:
        switch (m->sub) {
        case 0:
            D_80109230 = -1;
            m->sub++;
        case 1:
            if (JOYPAD_STATE & 0x10) {
                if (D_80109228 > 0) {
                    D_80109228--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                if (D_80109228 < D_8010922C - 1) {
                    D_80109228++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x2000) {
                D_80109228 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->state--;
            } else if (JOYPAD_STATE & 0x4008) {
                unsigned short bits;
                z = 0;
                bits = D_8009C62C[D_80109238[D_80109228]];
                for (n = 0; n < 9; n++) {
                    if (bits & 1) D_80109258[z++] = n;
                    bits >>= 1;
                }
                D_80109234 = z;
                D_80109230 = 0;
                SfxPlay2(10, 10);
                m->sub++;
            }
            break;
        case 2:
            if (JOYPAD_STATE & 0x10) {
                if (D_80109230 > 0) {
                    D_80109230--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                if (D_80109230 < D_80109234 - 1) {
                    D_80109230++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x2000) {
                D_80109230 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->sub--;
            } else if (JOYPAD_STATE & 0x4008) {
                unsigned char *tp;
                int n;
                int i1 = D_80109238[D_80109228], i2 = D_80109258[D_80109230];
                int x = D_80111128_v;
                tp = D_800FECE8[i1] + i2 * 2;
                {
                register unsigned char *t __asm__("$3") = tp;
                int a = t[0];
                register int b __asm__("$5");
                D_80110304 = a;
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
                D_80110308 = b;
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
                D_80109228 = -1;
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
        } else if (JOYPAD_STATE & 0x4008) {
            subScreenOpenMessage(1, D_80109298[m->sel], 2);
            D_801093D4 = 0;
            SfxPlay2(10, 10);
            m->state++;
        }
        break;
    case 5:
        subScreenMessageStep(D_801093D0, 2);
        if (D_801093D4 == 2) m->state = 4;
        return 0;
    case 6:
        subScreenMessageStep(D_801093D0, 2);
        if (D_801093D4 == 2) m->state = 2;
        return 0;
    }
    return 0;
}
#endif
