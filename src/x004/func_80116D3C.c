// FUNC 80116d3c 6596 X004
// MATCHING 80116d3c 6596
typedef struct { unsigned short frac; short whole; } FP;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[7];
    signed char b0f;
    int x;
    short yf, yw;
    int z;
    char p1c[0x2e - 0x1c];
    unsigned short animFrame;
    char p30[0x40 - 0x30];
    FP *h;
    char p44[0x69 - 0x44];
    unsigned char b69;
    char p6a[0x9c - 0x6a];
    unsigned char b9c, b9d, b9e;
    char p9f[0xa2 - 0x9f];
    unsigned char ba2;
    char pa3[0xaa - 0xa3];
    unsigned char baa, bab, bac;
    char pad[0xe0 - 0xad];
    short we0;
} PL;
typedef struct { char a[4]; unsigned char b4; char p[3]; } E8;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x3f - 0x34];
    unsigned char b3f;
    char p40[0x5a - 0x40];
    short w5a;
    char p5c[0x68 - 0x5c];
    E8 *p68;
    unsigned char b6c, b6d, b6e, b6f;
} O;
typedef void (*Fn)(O *);

extern PL D_800A6038;
extern Fn D_80079A98[];
extern E8 *D_801305DC[];
extern unsigned char D_8009CDA2[];
extern unsigned char D_8009C93A, D_8009C93E, D_8009C93F, D_8009C940, D_8009C942;
extern unsigned char D_8009D2C3, D_8009CDD5, D_8009CDF1, D_8009CFEE;
extern signed char D_8009D2B0x[];
#define D_8009D2B0 D_8009D2B0x[0]
extern unsigned char D_8009C982b;
extern unsigned short D_8009C982;
extern unsigned short D_8009C962;
extern short D_1F80016Ax[], D_1F80016Ex[], D_1F800172;
#define D_1F80016A D_1F80016Ax[0]
#define D_1F80016E D_1F80016Ex[0]
extern unsigned short D_1F8001FC;
extern short D_1F8000F2x[];
#define D_1F8000F2 D_1F8000F2x[0]
extern unsigned int FUN_80028420(O *);
extern unsigned int FUN_80028b40(O *);
extern void FUN_80027c74(O *);
extern void FUN_800277f8(O *);
extern void func_80029078(O *);
extern int FUN_8002715c(int);
extern int FUN_800270a0(O *, int, int);
extern void FUN_800216c4(void);
extern void loadSectionBounds(O *);
extern void AnimAdvance(PL *);
extern void FUN_800eea7c(PL *, int, int);
extern void FUN_800eeae4(PL *, int, int);
extern void func_80116C0C(O *);
extern void func_80116AE0(O *);

#define XU (*(unsigned short *)&D_1F80016Ax[0])
#define ZU (*(unsigned short *)&D_1F80016Ex[0])

#define T1(n) \
    if (FUN_800270a0(o, 1, n)) { \
        D_8009D2B0 = 0; \
        D_8009C93F = 1; \
        D_8009C942 = 1; \
    } \
    break;

#define T2A(n, k) \
    if (!FUN_800270a0(o, 2, n)) break; \
    o->w5a = k; \
    o->step = 4; \
    D_8009D2B0 = 0; \
    D_8009C93F = 1; \
    D_8009C942 = 1; \
    break;

#define T2B(n, k) \
    if (!FUN_800270a0(o, 2, n)) break; \
    D_8009C93F = 1; \
    D_8009C942 = 1; \
    D_8009D2B0 = 0; \
    o->w5a = k; \
    o->step = 4; \
    break;

#define F6047() \
    if (D_1F80016E < -0x3f2) \
        D_800A6038.b0f = 0; \
    else if (D_1F80016E < -0x3b5) \
        D_800A6038.b0f = -8;

static __inline__ short blocked(void)
{
    if (D_800A6038.bac >= 2) return 1;
    if (D_800A6038.b69 == 0) return 1;
    if (D_800A6038.b9c | D_800A6038.b9e | D_800A6038.baa | D_8009C940) return 1;
    if (D_1F8001FC & 0x10) return 0;
    return 1;
}

void func_80116D3C(O *o)
{
    PL *p = &D_800A6038;

    switch (o->step) {
    case 0:
        if (D_8009CDA2[0] != 0) {
            if (FUN_80028420(o) & FUN_80028b40(o)) D_8009CDA2[0] = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    case 1:
        if (D_8009C93A != 0) o->step++;
        FUN_80027c74(o);
        FUN_800277f8(o);
        func_80029078(o);
        break;
    case 2:
        D_80079A98[o->subtype](o);
        switch (D_8009C962) {
        case 0:
            F6047();
            if (D_1F800172 == 0x3de && (unsigned short)(ZU + 0x428) < 0x10 && (unsigned)(XU - 0x37b) < 0x12) {
                T1(0xb);
            }
            if (p->ba2 == 2) {
                if (D_1F80016E >= -0x185) {
                    if ((unsigned)(XU - 0x24a) < 0x60) {
                        FUN_8002715c(0x27a);
                        if (D_1F800172 >= 0x358) {
                            T1(1);
                        }
                    }
                    if ((unsigned)(XU - 0x356) < 0x60) {
                        FUN_8002715c(0x386);
                        if (D_1F800172 < 0x196) break;
                        T1(2);
                    }
                    if ((unsigned)(XU - 0x3b0) < 0x60) {
                        FUN_8002715c(0x3e0);
                        if (D_1F800172 < 0x196) break;
                        T1(3);
                    }
                    break;
                }
                if (D_1F80016E >= -0x1df) {
                    if ((unsigned)(XU - 0x1ee) < 0x60) {
                        FUN_8002715c(0x21e);
                        if (D_1F800172 < 0x358) break;
                        T1(4);
                    }
                    if ((unsigned)(XU - 0x356) < 0x60) {
                        FUN_8002715c(0x386);
                        if (D_1F800172 < 0x1f0) break;
                        T1(5);
                    }
                    break;
                }
                if (D_1F80016E >= -0x2ed) {
                    if ((unsigned)(XU - 0x356) < 0x60) {
                        FUN_8002715c(0x386);
                        if (D_1F800172 < 0x358) break;
                        T1(6);
                    }
                    if ((unsigned)(XU - 0x3b0) < 0x60) {
                        FUN_8002715c(0x3e0);
                        if (D_1F800172 < 0x3b2) break;
                        T1(7);
                    }
                }
                break;
            }
            if (blocked()) break;
            if ((unsigned)(XU - 0x1a2) < 0x40) {
                if (D_1F800172 == 0x384) {
                    T1(9);
                }
                if (D_1F800172 == 0x32a) {
                    T1(10);
                }
            }
            if ((unsigned)(XU - 0x30a) < 0x40 && D_1F800172 == 0x168) {
                T2A(8, 0x32a);
            }
            if ((unsigned)(XU - 0x364) < 0x40 && D_1F800172 == 0x21c) {
                T2A(8, 0x384);
            }
            if ((unsigned)(XU - 0x4cc) < 0x40) {
                if (D_1F800172 == 0x32a) {
                    T2A(8, 0x4ec);
                }
                if (D_1F800172 == 0x384) {
                    T2B(8, 0x4ec);
                }
            }
            if ((unsigned)(XU - 0x526) < 0x40 && D_1F800172 == 0x32a) {
                T2B(8, 0x546);
            }
            break;
        case 1:
            F6047();
            if (!(D_8009D2C3 & 8) && D_8009CDD5 != 0xff) {
                func_80116C0C(o);
                break;
            }
            if (D_1F80016A >= 0x653 && D_1F800172 == 0x4ec && D_1F80016E >= -0x225
                && FUN_800270a0(o, 1, 5)) {
                D_8009D2B0 = 0;
                D_8009C93F = 1;
                D_8009C942 = 1;
                D_8009CFEE = 1;
            }
            if (p->ba2 == 2) {
                if (D_1F80016E < -0x12b) break;
                if ((unsigned)(XU - 0x2a1) >= 0x60) break;
                FUN_8002715c(0x2d1);
                if (D_1F800172 >= 0x519) break;
                T1(1);
            }
            if (D_1F80016A < 0x78 && D_1F800172 == 0x546) {
                T1(0);
            }
            if (blocked()) break;
            switch (D_1F800172) {
            case 0x32a:
                if ((unsigned)(XU - 0x1fc) < 0x40) {
                    T2B(3, 0x21c);
                }
                break;
            case 0x384:
                if ((unsigned)(XU - 0x148) < 0x40) {
                    T2B(3, 0x168);
                }
                if ((unsigned)(XU - 0x1fc) < 0x40) {
                    T2B(3, 0x21c);
                }
                if ((unsigned)(XU - 0x30a) < 0x40) {
                    T2B(3, 0x32a);
                }
                break;
            case 0x492:
                if ((unsigned)(XU - 0x30a) < 0x40) {
                    T2B(3, 0x32a);
                }
                if ((unsigned)(XU - 0x364) < 0x40) {
                    T2B(3, 0x384);
                }
                break;
            case 0x4ec:
                if ((unsigned)(XU - 0x30a) < 0x40 && D_1F80016E < -0x1c2 && p->yw >= -0x1dd) {
                    T2B(3, 0x32a);
                }
                if ((unsigned)(XU - 0x418) < 0x40 && D_1F80016E < -0x1f4) {
                    T1(2);
                }
                if ((unsigned)(XU - 0x4cc) < 0x40) {
                    T1(4);
                }
                break;
            }
            break;
        case 2:
            F6047();
            if (!(D_8009D2C3 & 8) && D_8009CDD5 != 0xff) {
                func_80116AE0(o);
                break;
            }
            if (D_1F800172 == 0x276 && (unsigned short)(ZU + 0x415) < 0xc && (unsigned)(XU - 0x3d3) < 0x12) {
                T1(0xa);
            }
            if (p->ba2 == 2) {
                if (D_1F80016E >= -0x185) {
                    if ((unsigned)(XU - 0x465) < 0x60) {
                        FUN_8002715c(0x495);
                        if (D_1F800172 < 0x196) break;
                        T1(1);
                    }
                    if ((unsigned)(XU - 0x352) < 0x60) {
                        FUN_8002715c(0x382);
                        if (D_1F800172 < 0x196) break;
                        T1(2);
                    }
                    break;
                }
                if (D_1F80016E >= -0x1df) {
                    if ((unsigned)(XU - 0x3b0) < 0x60) {
                        FUN_8002715c(0x3e0);
                        if (D_1F800172 < 0x1f0) break;
                        T1(3);
                    }
                    break;
                }
                if (D_1F80016E >= -0x239) {
                    if (p->step != 0x20) break;
                    if ((unsigned)(XU - 0x438) < 0x60) {
                        FUN_8002715c(0x468);
                        if (D_1F800172 < 0x1f0) break;
                        T1(4);
                    }
                    if ((unsigned)(XU - 0x357) < 0x60) {
                        FUN_8002715c(0x387);
                        if (D_1F800172 < 0x24a) break;
                        T1(5);
                    }
                    break;
                }
                if (D_1F80016E >= -0x293) {
                    if ((unsigned)(XU - 0x2fb) < 0x60) {
                        FUN_8002715c(0x32b);
                        if (D_1F800172 < 0x24a) break;
                        T1(6);
                    }
                }
                break;
            }
            if (D_1F80016A >= 0x6d0 && D_1F800172 == 0x168) {
                T1(7);
            }
            if (blocked()) break;
            if (D_1F80016A < 0x28a && D_1F800172 == 0x21c) {
                T1(8);
            }
            if ((unsigned)(XU - 0x580) < 0x40) {
                if (D_1F800172 == 0x168) {
                    T2B(9, 0x5a0);
                }
                if (D_1F800172 == 0x1c2) {
                    T2B(9, 0x5a0);
                }
                if (D_1F800172 == 0x21c) {
                    T2B(9, 0x5a0);
                }
                break;
            }
            if ((unsigned)(XU - 0x526) < 0x40 && D_1F800172 == 0x21c) {
                T2B(9, 0x546);
            }
            if ((unsigned)(XU - 0x256) < 0x40 && D_1F80016E >= -0x180 && D_1F800172 == 0x168) {
                T1(0xb);
            }
            break;
        case 3:
            F6047();
            if (p->ba2 == 2) {
                if (D_1F80016E >= -0x185) {
                    if ((unsigned)(XU - 0x1ee) >= 0x60) break;
                    FUN_8002715c(0x21e);
                    if (D_1F800172 >= 0x573) break;
                    T1(1);
                }
                if (p->yw >= -0x280) break;
                if (D_1F80016E < -0x293) break;
                if ((unsigned)(XU - 0x248) >= 0x60) break;
                FUN_8002715c(0x278);
                if (D_1F800172 >= 0x4bf) break;
                T1(2);
            }
            if (blocked()) break;
            switch (D_1F800172) {
            case 0x546:
                if ((unsigned)(XU - 0x1fc) < 0x40) {
                    T2B(5, 0x21c);
                }
                if ((unsigned)(XU - 0x30a) < 0x40) {
                    T1(4);
                }
                break;
            case 0x5a0:
                if ((unsigned)(XU - 0x148) < 0x40) {
                    T2B(5, 0x168);
                }
                if ((unsigned)(XU - 0x30a) < 0x40) {
                    T1(3);
                }
                break;
            }
            break;
        case 6:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x7b) {
                T1(0);
            }
            break;
        case 7:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x59) {
                T1(0);
            }
            break;
        case 8:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x163) {
                T1(0);
            }
            break;
        case 9:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x6b) {
                T1(0);
            }
            break;
        case 11: {
            signed char *q;
            if (D_1F80016E < -0x1f4) {
                o->w30 = o->w32 = -0x3ac;
                if (D_1F80016A < 0x14 && D_1F80016E >= -0x36f) {
                    T1(0);
                }
                if (D_1F80016A < 0x11f) break;
                if (D_1F80016E < -0x36f) break;
                q = &D_8009D2B0;
                if (*q == 3) break;
                if (D_8009CDF1 != 0xff) break;
                o->step = 6;
                goto tail11;
            }
            o->w30 = o->w32 = -100;
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x27) {
                T1(1);
            }
            if (D_1F80016A < 0x11f) break;
            if (D_1F80016E < -0x27) break;
            q = &D_8009D2B0;
            if (*q == 3) break;
            o->step = 10;
        tail11:
            *q = 0;
            D_800A6038.active = 3;
            D_800A6038.b04 = 5;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            break;
        }
        case 13:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x16d) {
                T1(0);
            }
            if (D_1F80016A < 0x26d) break;
            if (D_1F80016E < -0x3d) break;
            T1(1);
        case 10:
        case 19:
            if (D_1F800172 < -0x3c) {
                T1(0);
            }
            break;
        case 14:
            if (D_1F80016E < -0xfa) {
                T1(D_8009C982b & 1);
            }
            break;
        case 4:
        case 5:
        case 12:
        case 15:
        case 16:
        case 17:
        case 18:
            if (D_1F80016A < 0x14 && D_1F80016E >= -0x45) {
                T1(0);
            }
            break;
        }
        break;
    case 3:
        D_8009C942 = 1;
        D_80079A98[o->subtype](o);
        break;
    case 4:
        if (p->active == 2 || p->bac >= 2) {
            o->subtype = 0;
            o->step = 2;
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_8009CDA2[0] = 0;
            break;
        }
        D_8009C93F = 1;
        D_8009C942 = 1;
        {
        E8 *e = D_801305DC[D_8009C962] + D_8009C982;
        unsigned char t;
        o->p68 = e;
        t = e->b4;
        o->b3f = t;
        if (t == 0)
            p->animFrame = 0;
        else
            p->animFrame = 1;
        p->b04 = 5;
        p->step = 0x41;
        p->state = 0;
        }
        FUN_800eeae4(p, 0, 0);
        if (FUN_8002715c(o->w5a)) o->step++;
        break;
    case 5:
        D_80079A98[o->subtype](o);
        if (o->subtype != 0) break;
        o->step = 2;
        FUN_800216c4();
        D_8009C93F = 0;
        D_8009C93E = 0;
        p->b04 = 1;
        p->step = 0;
        p->state = 0;
        loadSectionBounds(o);
        o->b6c = 0;
        o->b6d = 1;
        o->b6e = 2;
        o->b6f = 10;
        break;
    case 8:
        D_1F8000F2 += 2;
        if (D_1F8000F2 < -0x63) break;
        D_1F8000F2 = -100;
        o->w32 = -100;
        o->w30 = -100;
        o->step++;
        D_800A6038.yw = -0x22;
        FUN_800eea7c(&D_800A6038, 1, 0);
        D_800A6038.animFrame = 1;
        break;
    case 9:
        D_800A6038.h->whole -= 2;
        if (D_800A6038.h->whole < 0x114) {
            D_800A6038.we0 = 0x5a;
            D_800A6038.active = 3;
            D_800A6038.b04 = 1;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            o->step = 2;
        } else {
            AnimAdvance(&D_800A6038);
        }
        break;
    case 6:
    case 10:
        if (D_800A6038.state != 2) break;
        o->step++;
        FUN_800eea7c(&D_800A6038, 1, 0);
        D_800A6038.b04 = 5;
        D_800A6038.step = 0x65;
        D_800A6038.state = 0;
        D_800A6038.animFrame = 0;
        break;
    case 7:
    case 11:
        AnimAdvance(&D_800A6038);
        D_800A6038.h->whole += 1;
        if (D_800A6038.h->whole < 0x1b9) break;
        o->step++;
        break;
    case 12:
        D_1F8000F2 -= 2;
        if (D_1F8000F2 >= -0x3ac) break;
        D_1F8000F2 = -0x3ac;
        o->w32 = -0x3ac;
        o->w30 = -0x3ac;
        o->step++;
        D_800A6038.yw = -0x36a;
        FUN_800eea7c(&D_800A6038, 1, 0);
        D_800A6038.animFrame = 1;
        break;
    case 13:
        D_800A6038.h->whole -= 2;
        if (D_800A6038.h->whole < 0x114) {
            D_800A6038.we0 = 0x5a;
            D_800A6038.active = 3;
            D_800A6038.b04 = 1;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            o->step = 2;
        } else {
            AnimAdvance(&D_800A6038);
        }
        break;
    }
}
