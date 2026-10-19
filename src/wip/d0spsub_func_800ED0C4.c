// FUNC 800ed0c4 3488 D0SPSUB
typedef struct { unsigned char b0, state, sub, p3; char p4[0xE]; short mode; short page; short cursor; char p18[6]; short sel; } Menu;
typedef struct { short next; unsigned char fn, a; short b, c; } Cond;
typedef struct { unsigned char items[0x100]; unsigned short count, mode; } Inv;
typedef struct { unsigned char b0, b1, b2, b3; } Obj;
extern unsigned short D_1F8001FC;
extern Inv D_8009D1A4;
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
extern unsigned char D_80111134, D_80111138;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern unsigned char D_8009D2B0, D_800A6038, D_800A603C, D_800A603D, D_800A603E, D_8009C940, D_8009C941;
extern int func_800EC504(Menu *m, int n);
extern int func_800ECA28(Menu *m);
extern int func_800ECBF0(Menu *m, int n);
extern int func_800E89F8(int id);
extern void func_800EDE64(int kind, int add, int bank);
extern void func_800EF538(void *o, int a);
extern Obj *allocObjectLayer2(void);
extern void printInfoMessage(int id, int kind);
extern void playSFX(int id);
extern void playSFXAndSetNote(int a, int b);
extern void SfxPlay2(int a, int b);

int func_800ED0C4(Menu *m)
{
    int pages;
    int i, j, j2, k;
    switch (m->state) {
    case 0:
        m->cursor = 0;
        m->page = 0;
        for (i = 0; i < D_8009D1A4.count; i++) D_8010A0C8[i] = D_8009D1A4.items[i];
        m->state++;
    case 1:
        if (func_800EC504(m, 1)) return 0;
        pages = (D_8009D1A4.count + 9) / 10;
        if (D_1F8001FC & 0x400) {
            if (m->page > 0) {
                playSFX(8);
                m->page--;
            }
        } else if (D_1F8001FC & 0x800) {
            if (m->page < pages - 1) {
                playSFX(8);
                m->page++;
            }
        } else if (D_1F8001FC & 0x2000) {
            SfxPlay2(0x1B, 0x1C);
            return 1;
        } else if (D_1F8001FC & 0x4008) {
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
                k = 0;
                if (D_8009D1A4.mode & 0x8000) {
                    D_8009D1A4.mode &= 0x7FFF;
                    {
                        unsigned char *q = D_800FFD84[D_8009D1A4.mode];
                        for (j = 0; j < 5; j++) {
                            unsigned char c = q[j];
                            for (i = 0; i < 0x11E; i += 2) {
                                if (D_800FFC64[i] == c && D_8009D0A4[D_800FFC64[i + 1]]) {
                                    D_8009D1A4.items[k++] = D_800FFC64[i + 1];
                                }
                            }
                        }
                    }
                    for (i = 0; i < D_8009D1A4.count; i++) D_8010A0C8[i] = D_8009D1A4.items[i];
                } else {
                    if (++D_8009D1A4.mode >= 4) {
                        D_8009D1A4.mode = 0;
                        k = 0;
                    }
                    {
                        unsigned char *q = D_800FFD84[D_8009D1A4.mode];
                        for (j2 = 0; j2 < 5; j2++) {
                            unsigned char c = q[j2];
                            for (i = 0; i < 0x11E; i += 2) {
                                if (D_800FFC64[i] == c && D_8009D0A4[D_800FFC64[i + 1]]) {
                                    D_8009D1A4.items[k++] = D_800FFC64[i + 1];
                                }
                            }
                        }
                    }
                    for (i = 0; i < D_8009D1A4.count; i++) D_8010A0C8[i] = D_8009D1A4.items[i];
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
        if (func_800ECA28(m)) return 0;
        pages = (D_8009D1A4.count + 9) / 10;
        if (D_1F8001FC & 0x400) {
            if (m->page > 0) {
                m->page--;
                m->sel -= 10;
                playSFX(8);
            }
        } else if (D_1F8001FC & 0x800) {
            if (m->page < pages - 1) {
                m->page++;
                m->sel += 10;
                if (m->sel >= D_8009D1A4.count) m->sel = D_8009D1A4.count - 1;
                playSFX(8);
            }
        } else if (D_1F8001FC & 0x2000) {
            m->sel = -1;
            SfxPlay2(0x1B, 0x1C);
            m->state = 1;
        } else if ((D_1F8001FC & 0x4008) && m->cursor == 0) {
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
                    k = 0;
                    for (i = 0; i < 9; i++) {
                        if (D_8009D2C4[i]) D_8010A068[k++] = i;
                    }
                    D_8010A05C = k;
                    D_8010A060 = -1;
                    SfxPlay2(10, 10);
                    m->sub = 0;
                    m->state++;
                } else {
                    switch (func_800E89F8(D_80111128)) {
                    case 1:
                        if (D_80111128 == 0x14 || D_80111128 == 0x15 || D_80111128 == 0x16 || D_80111128 == 0x17
                            || D_80111128 == 0x1B || D_80111128 == 0x21 || D_80111128 == 0x22 || D_80111128 == 0x20
                            || D_80111128 == 0x1C || D_80111128 == 0x1D) {
                            printInfoMessage(D_80111128, 4);
                        } else if (D_80111128 == 0x18) {
                            printInfoMessage(D_80111128, 4);
                        } else {
                            printInfoMessage(D_80111128, 1);
                        }
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
                for (q = D_80109D88; *q != -1; q++) {
                    if (D_80111128 == *q) {
                        k = 0;
                        goto found;
                    }
                }
                for (q = D_80109D98; *q != -1; q++) {
                    if (D_80111128 == *q) {
                        k = 1;
                        goto found;
                    }
                }
                for (q = D_80109DB0; *q != -1; q++) {
                    if (D_80111128 == *q) {
                        if (D_8009D0E6 < 5) k = 3;
                        else k = 2;
                        goto found;
                    }
                }
                k = 2;
            found:
                func_800EDE64(0, k, 2);
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
            if (D_1F8001FC & 0x10) {
                if (D_8010A058 > 0) {
                    D_8010A058--;
                    playSFX(8);
                }
            } else if (D_1F8001FC & 0x40) {
                if (D_8010A058 < D_8010A05C - 1) {
                    D_8010A058++;
                    playSFX(8);
                }
            } else if (D_1F8001FC & 0x2000) {
                D_8010A058 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->state--;
            } else if (D_1F8001FC & 0x4008) {
                unsigned short bits;
                k = 0;
                bits = D_8009D2C4[D_8010A068[D_8010A058]];
                for (i = 0; i < 9; i++) {
                    if (bits & 1) D_8010A088[k++] = i;
                    bits >>= 1;
                }
                D_8010A064 = k;
                D_8010A060 = 0;
                SfxPlay2(10, 10);
                m->sub++;
            }
            break;
        case 2:
            if (D_1F8001FC & 0x10) {
                if (D_8010A060 > 0) {
                    D_8010A060--;
                    playSFX(8);
                }
            } else if (D_1F8001FC & 0x40) {
                if (D_8010A060 < D_8010A064 - 1) {
                    D_8010A060++;
                    playSFX(8);
                }
            } else if (D_1F8001FC & 0x2000) {
                D_8010A060 = -1;
                SfxPlay2(0x1B, 0x1C);
                m->sub--;
            } else if (D_1F8001FC & 0x4008) {
                unsigned char *t = D_800FFC3C[D_8010A068[D_8010A058]] + D_8010A088[D_8010A060] * 2;
                int n;
                D_80111134 = t[0];
                D_8009CD94 = t[0];
                D_800A603D = 0x30;
                D_8009CDA0 = 0;
                D_8009D2B0 = 3;
                D_800A603C = 1;
                D_800A603E = 0;
                D_800A6038 = 3;
                D_8009C940 = 1;
                D_8009C941 = D_80111128;
                D_80111138 = t[1];
                D_8009CD96 = t[1];
                n = 7;
                if (D_80111128 == 14) {
                    Obj *o = allocObjectLayer2();
                    if (o) {
                        o->b0 = 1;
                        o->b2 = 0x54;
                        o->b3 = 1;
                    }
                    n = 14;
                }
                printInfoMessage(n, 1);
                D_8010A058 = -1;
                SfxPlay2(10, 10);
                return 1;
            }
            break;
        }
        return 0;
    case 4:
        if (func_800ECBF0(m, D_8009D1A4.count)) return 0;
        pages = (D_8009D1A4.count + 9) / 10;
        if (D_1F8001FC & 0x400) {
            if (m->page > 0) {
                m->page--;
                m->sel -= 10;
                playSFX(8);
            }
        } else if (D_1F8001FC & 0x800) {
            if (m->page < pages - 1) {
                m->page++;
                m->sel += 10;
                if (m->sel >= D_8009D1A4.count) m->sel = D_8009D1A4.count - 1;
                playSFX(8);
            }
        } else if (D_1F8001FC & 0x2000) {
            m->sel = -1;
            SfxPlay2(0x1B, 0x1C);
            m->state = 1;
        } else if (D_1F8001FC & 0x4008) {
            func_800EDE64(1, D_8010A0C8[m->sel], 2);
            D_8010A204 = 0;
            SfxPlay2(10, 10);
            m->state++;
        }
        return 0;
    case 5:
        func_800EF538(D_8010A200, 2);
        if (D_8010A204 == 2) m->state = 4;
        return 0;
    case 6:
        func_800EF538(D_8010A200, 2);
        if (D_8010A204 == 2) m->state = 2;
        return 0;
    }
    return 0;
}
