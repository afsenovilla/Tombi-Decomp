// VERSION SLPS_01144 dspsub 800EE1B4 3284
#if defined(VERSION_SLPS_01144)
// JP twin of PAL d0spsub 800ED0C4 (ItemListStep), from the USA C. JP differences: pad bits are the byte-swapped
// PAL masks with circle/cross exchanged (0x400 -> 0x4, 0x2000 -> 0x40, 0x4008 -> 0x820, 0x10 -> 0x1000), 16 items
// per page, cursor case 7 returns 2 without the sound, the third message list gives 3 unconditionally, and the
// case-3 confirm calls func_800EB350(i1, i2) (sets D_80111400/D_80111404) instead of reading the table inline and
// drops the GAME_PLAYER_MODE store. The confirm block's statement order (CD94 store first, CD96 after the STEP
// store, the item-id load late) gives retail's schedule with plain (non-volatile) globals.
typedef struct { unsigned char b0, state, sub, p3; char p4[0xE]; short mode; short page; short cursor; char p18[6]; short sel; } Menu;
typedef struct { short next; unsigned char fn, a; short b, c; } Cond;
typedef struct { unsigned char items[0x100]; unsigned short count, mode; } Inv;
typedef struct { unsigned char b0, b1, b2, b3; } Obj;
extern unsigned short JOYPAD_STATE;
extern Inv D_8009ED4C;
extern unsigned short D_8009EE4C;
extern unsigned char D_8009EC4C[];
extern unsigned short D_8009EE6C[];
extern unsigned char D_8010A38C[];
extern unsigned char D_801057F8[][5];
extern unsigned char D_801056D8[];
extern unsigned char D_8010472C[];
extern Cond *D_80105514[];
extern int (*D_801055C8[])(int a, int b, int c);
extern short D_8010A04C[], D_8010A05C[], D_8010A074[];
extern short D_8010A31C, D_8010A320, D_8010A324, D_8010A328;
extern short D_8010A32C[], D_8010A34C[];
extern unsigned char D_8010A4C4[];
extern unsigned char D_8010A4C8;
extern int D_801113EC;
extern char D_80111400, D_80111404;
extern short D_8009E93C, D_8009E93E;
extern unsigned char D_800A7B78, D_8009E4E8, D_800A7B7D, PLAYER_STEP, D_800A7B7C, D_8009E4E9;
extern short D_8009E948;
extern int func_800ED918(Menu *m, int n);
extern int func_800EDC1C(Menu *m);
extern int func_800EDDA4(Menu *m, int n);
extern int func_800EA2BC(int id);
extern void func_800EEE88(int kind, int add, int bank);
extern void func_800F055C(void *o, int a);
extern Obj *allocObjectLayer2(void);
extern void printInfoMessage(int id, int kind);
extern void playSFX(int id);
extern void func_8001E910(int a, int b);
extern void func_8001E388(int a, int b);
extern void func_800EB350(int a, int b);

int func_800EE1B4(Menu *m)
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
            for (n = 0; n < D_8009ED4C.count; n++) D_8010A38C[n] = D_8009ED4C.items[n];
        }
        m->state++;
    case 1:
        if (func_800ED918(m, 1)) return 0;
        pages = (D_8009ED4C.count + 15) / 16;
        if (JOYPAD_STATE & 0x4) {
            if (m->page > 0) {
                playSFX(8);
                m->page--;
            }
        } else if (JOYPAD_STATE & 0x8) {
            if (m->page < pages - 1) {
                playSFX(8);
                m->page++;
            }
        } else if (JOYPAD_STATE & 0x40) {
            func_8001E388(0x1B, 0x1C);
            return 1;
        } else if (JOYPAD_STATE & 0x820) {
            switch (m->cursor) {
            case 0:
                if (D_8009ED4C.count) {
                    m->sel = m->page * 16;
                    m->state++;
                }
                break;
            case 1:
                if (D_8009ED4C.count) {
                    m->state = 4;
                    m->sel = m->page * 16;
                }
                break;
            case 2:
                md = D_8009ED4C.mode;
                if (md & 0x8000) {
                    k = 0;
                    md &= 0x7FFF;
                    D_8009ED4C.mode = md;
                    {
                        for (j = 0; j < 5; j++) {
                            int c = D_801057F8[md][j];
                            for (i = 0; i < 0x11E; i += 2) {
                                if (D_801056D8[i] == c && D_8009EC4C[D_801056D8[i + 1]]) {
                                    D_8009ED4C.items[k++] = D_801056D8[i + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009EE4C; n++) D_8010A38C[n] = D_8009ED4C.items[n]; }
                } else {
                    if (++D_8009ED4C.mode >= 4) {
                        D_8009ED4C.mode = 0;
                    }
                    k = 0;
                    {
                        md = D_8009ED4C.mode;
                        for (j2 = 0; j2 < 5; j2++) {
                            int c = D_801057F8[md][j2];
                            for (i2 = 0; i2 < 0x11E; i2 += 2) {
                                if (D_801056D8[i2] == c && D_8009EC4C[D_801056D8[i2 + 1]]) {
                                    D_8009ED4C.items[k++] = D_801056D8[i2 + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009EE4C; n++) D_8010A38C[n] = D_8009ED4C.items[n]; }
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
                return 2;
            case 8:
            case 9:
            case 10:
            case 11:
                func_8001E388(10, 10);
                return 1;
            }
            func_8001E910(10, 10);
        }
        return 0;
    case 2:
        if (func_800EDC1C(m)) return 0;
        pages = (D_8009ED4C.count + 15) / 16;
        if (JOYPAD_STATE & 0x4) {
            if (m->page > 0) {
                m->page--;
                m->sel -= 16;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x8) {
            if (m->page < pages - 1) {
                m->page++;
                m->sel += 16;
                if (m->sel >= D_8009ED4C.count) m->sel = D_8009ED4C.count - 1;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x40) {
            m->sel = -1;
            func_8001E388(0x1B, 0x1C);
            m->state = 1;
        } else if ((JOYPAD_STATE & 0x820) && m->cursor == 0) {
            Cond *c, *base;
            int r;
            D_801113EC = D_8010A38C[m->sel];
            c = D_80105514[D_8010472C[D_801113EC]];
            base = c;
            for (;;) {
                r = D_801055C8[c->fn](c->a, c->b, c->c);
                if (c->next == -1) break;
                if (r) c = base + c->next;
                else c++;
            }
            if (r) {
                if (D_801113EC == 7 || D_801113EC == 14) {
                    D_8010A31C = 0;
                    {
                    int n, z;
                    z = 0;
                    for (n = 0; n < 9; n++) {
                        if (D_8009EE6C[n]) D_8010A32C[z++] = n;
                    }
                    D_8010A320 = z;
                    }
                    D_8010A324 = -1;
                    func_8001E388(10, 10);
                    m->sub = 0;
                    m->state++;
                } else {
                    switch (func_800EA2BC(D_801113EC)) {
                    case 1:
                        printInfoMessage(D_801113EC, 1);
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
                int x = D_801113EC;
                int w;
                for (q = D_8010A04C; *q != -1; q++) {
                    if (x == *q) {
                        w = 0;
                        goto found;
                    }
                }
                for (q = D_8010A05C; *q != -1; q++) {
                    if (x == *q) {
                        w = 1;
                        goto found;
                    }
                }
                for (q = D_8010A074; *q != -1; q++) {
                    if (x == *q) {
                        w = 3;
                        goto found;
                    }
                }
                w = 2;
            found:
                func_800EEE88(0, w, 2);
                D_8010A4C8 = 0;
                func_8001E388(10, 10);
                m->state = 6;
            }
        }
        return 0;
    case 3:
        switch (m->sub) {
        case 0:
            D_8010A324 = -1;
            m->sub++;
        case 1:
            if (JOYPAD_STATE & 0x1000) {
                if (D_8010A31C > 0) {
                    D_8010A31C--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x4000) {
                if (D_8010A31C < D_8010A320 - 1) {
                    D_8010A31C++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                D_8010A31C = -1;
                func_8001E388(0x1B, 0x1C);
                m->state--;
            } else if (JOYPAD_STATE & 0x820) {
                unsigned short bits;
                z = 0;
                bits = D_8009EE6C[D_8010A32C[D_8010A31C]];
                for (n = 0; n < 9; n++) {
                    if (bits & 1) D_8010A34C[z++] = n;
                    bits >>= 1;
                }
                D_8010A328 = z;
                D_8010A324 = 0;
                func_8001E388(10, 10);
                m->sub++;
            }
            break;
        case 2:
            if (JOYPAD_STATE & 0x1000) {
                if (D_8010A324 > 0) {
                    D_8010A324--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x4000) {
                if (D_8010A324 < D_8010A328 - 1) {
                    D_8010A324++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                D_8010A324 = -1;
                func_8001E388(0x1B, 0x1C);
                m->sub--;
            } else if (JOYPAD_STATE & 0x820) {
                int x;
                func_800EB350(D_8010A32C[D_8010A31C], D_8010A34C[D_8010A324]);
                D_8009E93C = D_80111400;
                D_8009E948 = 0;
                D_800A7B7C = 1;
                PLAYER_STEP = 0;
                D_8009E4E8 = 1;
                D_8009E93E = D_80111404;
                x = D_801113EC;
                D_800A7B7D = 0x30;
                D_8009E4E9 = x;
                D_800A7B78 = 3;
                if (x == 14) {
                    Obj *o = allocObjectLayer2();
                    if (o) {
                        o->b0 = 1;
                        o->b2 = 0x54;
                        o->b3 = 1;
                    }
                    printInfoMessage(14, 1);
                } else {
                    printInfoMessage(7, 1);
                }
                D_8010A31C = -1;
                func_8001E388(10, 10);
                return 1;
            }
            break;
        }
        break;
    case 4:
        if (func_800EDDA4(m, D_8009ED4C.count)) return 0;
        pages = (D_8009ED4C.count + 15) / 16;
        if (JOYPAD_STATE & 0x4) {
            if (m->page > 0) {
                m->page--;
                m->sel -= 16;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x8) {
            if (m->page < pages - 1) {
                m->page++;
                m->sel += 16;
                if (m->sel >= D_8009ED4C.count) m->sel = D_8009ED4C.count - 1;
                playSFX(8);
            }
        } else if (JOYPAD_STATE & 0x40) {
            m->sel = -1;
            func_8001E388(0x1B, 0x1C);
            m->state = 1;
        } else if (JOYPAD_STATE & 0x820) {
            func_800EEE88(1, D_8010A38C[m->sel], 2);
            D_8010A4C8 = 0;
            func_8001E388(10, 10);
            m->state++;
        }
        break;
    case 5:
        func_800F055C(D_8010A4C4, 2);
        if (D_8010A4C8 == 2) m->state = 4;
        return 0;
    case 6:
        func_800F055C(D_8010A4C4, 2);
        if (D_8010A4C8 == 2) m->state = 2;
        return 0;
    }
    return 0;
}
#endif
