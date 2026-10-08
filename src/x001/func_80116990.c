// FUNC 80116990 2816 X001
// MATCHING 80116990 2816
// FLAGS -O2 -G0 -fno-cse-skip-blocks
typedef struct { short s0, s2; } S2;
typedef struct { char p[0x4c]; short w4c, w4e; } SC;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x30 - 8];
    short w30, w32;
    char p34[0x3c - 0x34];
    unsigned char b3c;
    char p3d[0x48 - 0x3d];
    short w48, w4a;
} O;
typedef void (*Fn)(O *);
typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6; } PL;

extern PL D_800A6038;
extern unsigned char D_800A6039[];
extern Fn D_80079A98[];
extern unsigned char D_8009CDA2;
extern unsigned char D_8009C93A, D_8009C93F, D_8009C942, D_8009CEF7, D_8009C93C, D_8009C975;
extern unsigned char D_8009CDB5, D_8009CE58, D_8009CDF1, D_8009CE48, D_8009CDBC;
extern unsigned char D_800A60DA, D_800A60E0, D_800A60D8, D_800A603C, D_800A603D, D_800A603E;
extern signed char D_8009D2B0;
extern unsigned short D_8009C962, D_8009C982;
extern unsigned int D_8009C96C;
extern short D_8009CD94, D_8009CD96, D_8009CDA0;
extern unsigned short D_800A6066;
extern S2 *D_800A6078x[];
#define D_800A6078 D_800A6078x[0]
extern short D_1F80016A, D_1F80016E, D_1F800172;
extern short D_1F8000F2;
extern int D_1F8000F0;
extern SC *D_1F8001D4;
extern unsigned int FUN_80028420(O *);
extern unsigned int FUN_80028b40(O *);
extern void FUN_80027c74(O *);
extern void FUN_800277f8(O *);
extern void FUN_80029078(O *);
extern void FUN_800216c4(void);
extern void FUN_8002715c(int);
extern int FUN_800270a0(O *, int, unsigned char);
extern void FUN_800eea7c(PL *, int, int);
extern void AnimAdvance(PL *);
extern void FUN_8005a9a4(int, int);
extern void FUN_8002a3d4(O *);

#define XU (*(unsigned short *)&D_1F80016A)
#define ZU (*(unsigned short *)&D_1F80016E)

void func_80116990(O *o)
{
    switch (o->step) {
    case 0: {
        unsigned char *f = &D_8009CDA2;
        if (*f != 0) {
            if (FUN_80028420(o) & FUN_80028b40(o)) *f = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    }
    case 1:
        if (D_8009C93A != 0) o->step++;
        FUN_80027c74(o);
        o->b3c = 0;
        FUN_800277f8(o);
        FUN_80029078(o);
        break;
    case 2:
        D_80079A98[o->subtype](o);
        switch (D_8009C962) {
        case 0: {
            unsigned short *p = &D_8009C982;
            if (*p != 0) {
                *p = 0;
                break;
            }
            if (D_1F80016A >= 0xb6e && D_1F80016E < -0x1d6) {
                FUN_800216c4();
                D_8009C962 = 1;
                *p = 0;
                break;
            }
            if (D_1F80016A >= 0xb70 && D_1F80016E >= -0x118) {
                if (D_8009CDB5 == 0) break;
                goto t11;
            }
            if (D_1F80016A >= 0x75) break;
            if (D_1F80016E < -0xc7) break;
            goto t10;
        }
        case 1: {
            unsigned short *p = &D_8009C982;
            short x;
            if (*p != 0) {
                *p = 0;
                break;
            }
            x = D_1F80016A;
            if (x < 0xb6d) {
                D_8009C962 = 0;
                FUN_800216c4();
                *p = 0;
                break;
            }
            if (x >= 0x109b && D_1F80016E >= -0x179) goto t12;
            if (D_8009CDB5 == 0) break;
            if (D_800A60DA != 2) break;
            if ((unsigned short)(XU - 0x102f) >= 0x60) break;
            FUN_8002715c(0x105f);
            if (D_1F800172 < 0x51) break;
            goto t11;
        }
        case 2: {
            unsigned short *p = &D_8009C982;
            if (*p == 0) {
                if (D_1F80016A < 0x65) goto t10;
                if (D_1F80016A >= 0x779 && D_1F80016E >= -0x1c6) goto t11;
                if (D_8009CE58 == 0) break;
                if (D_1F80016A < 0x6e3) break;
                if ((unsigned short)(ZU + 0x46) >= 0x1e) break;
                goto t12;
            } else {
                *p = 0;
            }
            break;
        }
        case 3: {
            unsigned short *p = &D_8009C982;
            unsigned short k = *p;
            switch (k) {
            case 0:
                if (D_1F80016E < -0x5dc && (unsigned short)(XU - 0x50f) < 0x60 && D_800A60DA != 0) {
                    FUN_8002715c(0x53f);
                    break;
                }
                if (D_8009CDF1 == 0xff && (unsigned short)(XU - 0x69c) < 0x13 && D_1F80016E >= -0x29d) {
                    o->step = 4;
                    D_8009C93F = 1;
                    D_8009C942 = 1;
                    D_8009CEF7 = 0;
                    D_8009D2B0 = 0;
                    D_800A6038.b0 = 5;
                    D_800A603C = 5;
                    D_800A603D = 0;
                    D_800A603E = 0;
                    break;
                }
                if (D_1F80016A < 0x17e && D_1F80016E >= -0x18f) goto t10;
                if (D_1F80016A >= 0x7fc && D_1F80016E >= -0x293) {
                t11:
                    FUN_800270a0(o, 1, 1);
                    break;
                }
                if (D_1F80016A >= 0x821) D_8009C982 = 2;
                break;
            case 1:
                *p = 0;
                break;
            case 2:
                if (D_1F80016A < 0x820) {
                    *p = 0;
                    break;
                }
                if (D_1F80016A < 0xb4b) break;
                if (D_1F80016E < -0x1d5) break;
                FUN_800270a0(o, 1, k);
                break;
            default:
                D_8009C982 = 0;
                break;
            }
            break;
        }
        case 4: {
            unsigned short *p = &D_8009C982;
            if (*p == 0) {
                if (D_8009C96C > 999999 && D_8009CE48 != 0 && (unsigned short)(XU - 0xb5c) < 0x20) {
                    D_800A60E0 = 8;
                    D_800A60D8 = 0x21;
                    if (D_1F800172 >= 0x881) {
                        FUN_8002715c(0xb6c);
                        if (D_1F800172 >= 0x8d1) {
                            o->step = 8;
                            D_800A603C = 5;
                            D_800A6039[0] = 0;
                            D_800A603D = 0x41;
                            D_800A603E = 0;
                            break;
                        }
                    }
                }
                if (D_1F80016A >= 0xfab && D_1F80016E >= -0x1cb) {
                t10:
                    FUN_800270a0(o, 1, 0);
                    break;
                }
                if ((unsigned short)(XU - 0x72e) < 0x20 && D_1F800172 >= 0x881) {
                    FUN_8002715c(0x73e);
                    if (D_1F800172 >= 0x895) {
                    t12:
                        FUN_800270a0(o, 1, 2);
                        break;
                    }
                }
                if ((unsigned short)(XU - 0xb28) < 0x80 && D_1F800172 < 0x880) {
                    FUN_8002715c(0xb68);
                    if (D_1F800172 < 0x80c) FUN_800270a0(o, 1, 1);
                }
                if (D_1F800172 < 0x87a) {
                    short *q = &D_1F8000F2;
                    *q += 2;
                    if (o->w32 < *q) *q = o->w32;
                }
                break;
            } else {
                *p = 0;
            }
            break;
        }
        }
        break;
    case 3:
        if (D_8009C93F == 0) D_80079A98[o->subtype](o);
        break;
    case 4: {
        unsigned char *f = &D_800A6038.b6;
        if (*f == 2) {
            FUN_800eea7c(&D_800A6038, 1, 0);
            D_800A603C = 5;
            D_800A603D = 0x41;
            *f = 0;
            D_800A6066 = 1;
            if (D_8009CDBC == 0xff) {
                o->step = 6;
                o->w4a = 0;
            } else {
                o->step++;
            }
        }
        FUN_8002a3d4(o);
        break;
    }
    case 5:
        AnimAdvance(&D_800A6038);
        D_800A6078->s2--;
        if (D_800A6078->s2 < 0x6ae) {
            o->step++;
            FUN_8005a9a4(0x18, 0);
            o->w4a = 0x154;
        }
        FUN_8002a3d4(o);
        break;
    case 6: {
        PL *pl;
        if (o->w4a == 0) {
            pl = &D_800A6038;
            AnimAdvance(pl);
            D_800A6078->s2--;
            if (D_800A6078->s2 < 0x67c) {
                o->step++;
                FUN_800eea7c(pl, 0, 0);
                o->w4a = 0x3c;
            }
            FUN_8002a3d4(o);
        } else {
            o->w4a--;
        }
        break;
    }
    case 7:
        D_800A6039[0] = 0;
        if (--o->w4a == -1) {
            FUN_800270a0(o, 1, 3);
            o->step = 3;
            D_8009C93F = 0;
        }
        break;
    case 8:
        o->w48 = 0x3c;
        o->step++;
        break;
    case 9:
        if (--o->w48 == -1) {
            D_8009CD94 = 0xd;
            D_8009CD96 = 0;
            D_8009CDA0 = 0;
            D_8009C93C = 0;
            D_8009C975 = 3;
            o->step++;
        }
    case 10: {
        short *q;
        if (D_8009C975 == 1) {
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
        }
        q = &D_1F8000F2;
        *q -= 2;
        if (*q < o->w30) D_1F8000F0 = o->w30 << 16;
        break;
    }
    }
}
