// VERSION SLPS_01144 dspsub3 800EDC50 3148
#if defined(VERSION_SLPS_01144)
// JP DSPSUB3 twin of DSPSUB func_800EE1B4 (ItemListStep), with the five D0SPSUB3 differences of the USA file
// src/d0spsub3/func_800EBAD4_scus94236.c (tab cursor call (m), confirm mask 0x830 = JP of 0x5008, no mode cases
// 4..7 and func_8001E388 in the default tail, state 1 page store before playSFX(8) + return 0, state 4 playSFX(8)
// first). JP-only on top: two message lists (w 0 / 1, else 2) and no D_800A7B78 = 3 store in the confirm block.
typedef struct { unsigned char b0, state, sub, p3; char p4[0xE]; short mode; short page; short cursor; char p18[6]; short sel; } Menu;
typedef struct { short next; unsigned char fn, a; short b, c; } Cond;
typedef struct { unsigned char items[0x100]; unsigned short count, mode; } Inv;
typedef struct { unsigned char b0, b1, b2, b3; } Obj;
extern unsigned short JOYPAD_STATE;
extern Inv D_8009ED4C;
extern unsigned short D_8009EE4C;
extern unsigned char D_8009EC4C[];
extern unsigned short D_8009EE6C[];
extern unsigned char D_800FF394[];
extern unsigned char D_800FAAA8[][5];
extern unsigned char D_800FA988[];
extern unsigned char D_800F99DC[];
extern Cond *D_800FA7C4[];
extern int (*D_800FA878[])(int a, int b, int c);
extern short D_800FF2FC[], D_800FF30C[];
extern short D_800FF324, D_800FF328, D_800FF32C, D_800FF330;
extern short D_800FF334[], D_800FF354[];
extern unsigned char D_800FF49C[];
extern unsigned char D_800FF4A0;
extern int D_800FF570;
extern char D_800FF574, D_800FF578;
extern short D_8009E93C, D_8009E93E;
extern unsigned char D_8009E4E8, D_800A7B7D, PLAYER_STEP, D_800A7B7C, D_8009E4E9;
extern short D_8009E948;
extern int func_800ED7F4(Menu *m);
extern int func_800ED958(Menu *m);
extern int func_800EDAE0(Menu *m, int n);
extern int func_800EA204(int id);
extern void func_800EE89C(int kind, int add, int bank);
extern void func_800EEA58(void *o, int a);
extern Obj *allocObjectLayer2(void);
extern void printInfoMessage(int id, int kind);
extern void playSFX(int id);
extern void func_8001E388(int a, int b);
extern void func_800EB298(int a, int b);

int func_800EDC50(Menu *m)
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
            for (n = 0; n < D_8009ED4C.count; n++) D_800FF394[n] = D_8009ED4C.items[n];
        }
        m->state++;
    case 1:
        if (func_800ED7F4(m)) return 0;
        pages = (D_8009ED4C.count + 15) / 16;
        if (JOYPAD_STATE & 0x4) {
            if (m->page > 0) {
                m->page--;
                playSFX(8);
                return 0;
            }
        } else if (JOYPAD_STATE & 0x8) {
            if (m->page < pages - 1) {
                m->page++;
                playSFX(8);
                return 0;
            }
        } else if (JOYPAD_STATE & 0x40) {
            func_8001E388(0x1B, 0x1C);
            return 1;
        } else if (JOYPAD_STATE & 0x830) {
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
                            int c = D_800FAAA8[md][j];
                            for (i = 0; i < 0x11E; i += 2) {
                                if (D_800FA988[i] == c && D_8009EC4C[D_800FA988[i + 1]]) {
                                    D_8009ED4C.items[k++] = D_800FA988[i + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009EE4C; n++) D_800FF394[n] = D_8009ED4C.items[n]; }
                } else {
                    if (++D_8009ED4C.mode >= 4) {
                        D_8009ED4C.mode = 0;
                    }
                    k = 0;
                    {
                        md = D_8009ED4C.mode;
                        for (j2 = 0; j2 < 5; j2++) {
                            int c = D_800FAAA8[md][j2];
                            for (i2 = 0; i2 < 0x11E; i2 += 2) {
                                if (D_800FA988[i2] == c && D_8009EC4C[D_800FA988[i2 + 1]]) {
                                    D_8009ED4C.items[k++] = D_800FA988[i2 + 1];
                                }
                            }
                        }
                    }
                    { int n; for (n = 0; n < D_8009EE4C; n++) D_800FF394[n] = D_8009ED4C.items[n]; }
                }
                break;
            case 8:
            case 9:
            case 10:
            case 11:
                func_8001E388(10, 10);
                return 1;
            }
            func_8001E388(10, 10);
        }
        return 0;
    case 2:
        if (func_800ED958(m)) return 0;
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
        } else if ((JOYPAD_STATE & 0x830) && m->cursor == 0) {
            Cond *c, *base;
            int r;
            D_800FF570 = D_800FF394[m->sel];
            c = D_800FA7C4[D_800F99DC[D_800FF570]];
            base = c;
            for (;;) {
                r = D_800FA878[c->fn](c->a, c->b, c->c);
                if (c->next == -1) break;
                if (r) c = base + c->next;
                else c++;
            }
            if (r) {
                if (D_800FF570 == 7 || D_800FF570 == 14) {
                    D_800FF324 = 0;
                    {
                    int n, z;
                    z = 0;
                    for (n = 0; n < 9; n++) {
                        if (D_8009EE6C[n]) D_800FF334[z++] = n;
                    }
                    D_800FF328 = z;
                    }
                    D_800FF32C = -1;
                    func_8001E388(10, 10);
                    m->sub = 0;
                    m->state++;
                } else {
                    switch (func_800EA204(D_800FF570)) {
                    case 1:
                        printInfoMessage(D_800FF570, 1);
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
                int x = D_800FF570;
                int w;
                for (q = D_800FF2FC; *q != -1; q++) {
                    if (x == *q) {
                        w = 0;
                        goto found;
                    }
                }
                for (q = D_800FF30C; *q != -1; q++) {
                    if (x == *q) {
                        w = 1;
                        goto found;
                    }
                }
                w = 2;
            found:
                func_800EE89C(0, w, 2);
                D_800FF4A0 = 0;
                func_8001E388(10, 10);
                m->state = 6;
            }
        }
        return 0;
    case 3:
        switch (m->sub) {
        case 0:
            D_800FF32C = -1;
            m->sub++;
        case 1:
            if (JOYPAD_STATE & 0x1000) {
                if (D_800FF324 > 0) {
                    D_800FF324--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x4000) {
                if (D_800FF324 < D_800FF328 - 1) {
                    D_800FF324++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                D_800FF324 = -1;
                func_8001E388(0x1B, 0x1C);
                m->state--;
            } else if (JOYPAD_STATE & 0x830) {
                unsigned short bits;
                z = 0;
                bits = D_8009EE6C[D_800FF334[D_800FF324]];
                for (n = 0; n < 9; n++) {
                    if (bits & 1) D_800FF354[z++] = n;
                    bits >>= 1;
                }
                D_800FF330 = z;
                D_800FF32C = 0;
                func_8001E388(10, 10);
                m->sub++;
            }
            break;
        case 2:
            if (JOYPAD_STATE & 0x1000) {
                if (D_800FF32C > 0) {
                    D_800FF32C--;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x4000) {
                if (D_800FF32C < D_800FF330 - 1) {
                    D_800FF32C++;
                    playSFX(8);
                }
            } else if (JOYPAD_STATE & 0x40) {
                D_800FF32C = -1;
                func_8001E388(0x1B, 0x1C);
                m->sub--;
            } else if (JOYPAD_STATE & 0x830) {
                int x;
                func_800EB298(D_800FF334[D_800FF324], D_800FF354[D_800FF32C]);
                D_8009E93C = D_800FF574;
                D_8009E948 = 0;
                D_800A7B7C = 1;
                PLAYER_STEP = 0;
                D_8009E4E8 = 1;
                D_8009E93E = D_800FF578;
                x = D_800FF570;
                D_800A7B7D = 0x30;
                D_8009E4E9 = x;
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
                D_800FF324 = -1;
                func_8001E388(10, 10);
                return 1;
            }
            break;
        }
        break;
    case 4:
        if (func_800EDAE0(m, D_8009ED4C.count)) return 0;
        pages = (D_8009ED4C.count + 15) / 16;
        if (JOYPAD_STATE & 0x4) {
            if (m->page > 0) {
                playSFX(8);
                m->page--;
                m->sel -= 16;
            }
        } else if (JOYPAD_STATE & 0x8) {
            if (m->page < pages - 1) {
                playSFX(8);
                m->page++;
                m->sel += 16;
                if (m->sel >= D_8009ED4C.count) m->sel = D_8009ED4C.count - 1;
            }
        } else if (JOYPAD_STATE & 0x40) {
            m->sel = -1;
            func_8001E388(0x1B, 0x1C);
            m->state = 1;
        } else if (JOYPAD_STATE & 0x830) {
            func_800EE89C(1, D_800FF394[m->sel], 2);
            D_800FF4A0 = 0;
            func_8001E388(10, 10);
            m->state++;
        }
        break;
    case 5:
        func_800EEA58(D_800FF49C, 2);
        if (D_800FF4A0 == 2) m->state = 4;
        return 0;
    case 6:
        func_800EEA58(D_800FF49C, 2);
        if (D_800FF4A0 == 2) m->state = 2;
        return 0;
    }
    return 0;
}
#endif
