// VERSION SLPS_01144 x01 80115C64 2808
// FLAGS -O2 -G0 -fno-cse-skip-blocks
#if defined(VERSION_SLPS_01144)
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
typedef struct { short s0, s2; } S2;

#define SH(o) (*(short *)(0x1f800000 + (o)))
extern short PX_, PY_, PD_, PCY_;
extern int PCY32_;
typedef struct { char p[0x4c]; short w4c, w4e; } SC;
extern SC *TC_;
asm(".set TC_,0x1f8001d4");
static __inline__ short *qsp(void) { short *q; asm("lui %0,0x1f80\n\taddiu %0,%0,242" : "=r"(q)); return q; }
asm(".set PX_,0x1f80016a");
asm(".set PY_,0x1f80016e");
asm(".set PD_,0x1f800172");
asm(".set PCY_,0x1f8000f2");
asm(".set PCY32_,0x1f8000f0");
#define XS PX_
#define YS PY_
#define DS PD_
#define XU (*(unsigned short *)&PX_)
#define YU (*(unsigned short *)&PY_)

extern unsigned char D_8009E4E2, D_8009E4E7, D_8009E4EA, D_8009E4E4, D_8009E51D, D_8009E94A, D_8009E95D, D_8009E964, D_8009E999, D_8009E9F0, D_8009EA00, D_8009EE58;
extern unsigned short D_8009E52A, D_8009E93C, D_8009E93E, D_8009E948, GAME_SELECTED_SECTION;
extern unsigned int D_8009E514;
extern unsigned char D_800A7B78, D_800A7B79, D_800A7B7C, D_800A7B7D, D_800A7B7E, D_800A7C18, D_800A7C1A, D_800A7C20;
extern unsigned short D_800A7BA6;
extern S2 *PLAYER_H[];
extern Fn D_8007582C[];

extern unsigned int func_80029230(O *);
extern unsigned int func_80029950(O *);
extern void func_80028A84(O *);
extern void func_80028608(O *);
extern void func_80029E88(O *);
extern void func_80022098(void);
extern void func_80027F50(int);
extern int func_80027EA8(O *, int, unsigned char);
extern void func_800EEC38(void *, int, int);
extern void func_80020E38(void *);
extern void func_8001C988(int, int);
extern void func_8002B1E0(O *);

void func_80115C64_slps01144(O *o)
{
    switch (o->step) {
    case 0: {
        unsigned char *f = &D_8009E94A;
        if (*f != 0) {
            if (func_80029230(o) & func_80029950(o)) *f = 0;
            break;
        }
        o->subtype = 0;
        o->step++;
        break;
    }
    case 1:
        if (D_8009E4E2 != 0) o->step++;
        func_80028A84(o);
        o->b3c = 0;
        func_80028608(o);
        func_80029E88(o);
        break;
    case 2:
        D_8007582C[o->subtype](o);
        switch (GAME_SELECTED_SECTION) {
        case 0: {
            unsigned short *p = &D_8009E52A;
            if (*p != 0) {
                *p = 0;
                break;
            }
            if (XS >= 0xb6e && YS < -0x1d6) {
                func_80022098();
                GAME_SELECTED_SECTION = 1;
                *p = 0;
                break;
            }
            if (XS >= 0xb70 && YS >= -0x118) {
                if (D_8009E95D == 0) break;
                goto t11;
            }
            if (XS >= 0x75) break;
            if (YS < -0xc7) break;
            goto t10;
        }
        case 1: {
            unsigned short *p = &D_8009E52A;
            short x;
            if (*p != 0) {
                *p = 0;
                break;
            }
            x = XS;
            if (x < 0xb6d) {
                GAME_SELECTED_SECTION = 0;
                func_80022098();
                *p = 0;
                break;
            }
            if (x >= 0x109b && YS >= -0x179) goto t12;
            if (D_8009E95D == 0) break;
            if (D_800A7C1A != 2) break;
            if ((unsigned short)(XU - 0x102f) >= 0x60) break;
            func_80027F50(0x105f);
            if (DS < 0x51) break;
            goto t11;
        }
        case 2: {
            unsigned short *p = &D_8009E52A;
            if (*p == 0) {
                if (XS < 0x65) goto t10;
                if (XS >= 0x779 && YS >= -0x1c6) goto t11;
                if (D_8009EA00 == 0) break;
                if (XS < 0x6e3) break;
                if ((unsigned short)(YU + 0x46) >= 0x1e) break;
                goto t12;
            } else {
                *p = 0;
            }
            break;
        }
        case 3: {
            unsigned short *p = &D_8009E52A;
            unsigned short k = *p;
            switch (k) {
            case 0:
                if (YS < -0x5dc && (unsigned short)(XU - 0x50f) < 0x60 && D_800A7C1A != 0) {
                    func_80027F50(0x53f);
                    break;
                }
                if (D_8009E999 == 0xff && (unsigned short)(XU - 0x69c) < 0x13 && YS >= -0x29d) {
                    o->step = 4;
                    D_8009E4E7 = 1;
                    D_8009E4EA = 1;
                    D_8009EE58 = 0;
                    D_800A7B78 = 5;
                    D_800A7B7C = 5;
                    D_800A7B7D = 0;
                    D_800A7B7E = 0;
                    break;
                }
                if (XS < 0x17e && YS >= -0x18f) goto t10;
                if (XS >= 0x7fc && YS >= -0x293) {
                t11:
                    func_80027EA8(o, 1, 1);
                    break;
                }
                if (XS >= 0x821) D_8009E52A = 2;
                break;
            case 1:
                *p = 0;
                break;
            case 2:
                if (XS < 0x820) {
                    *p = 0;
                    break;
                }
                if (XS < 0xb4b) break;
                if (YS < -0x1d5) break;
                func_80027EA8(o, 1, k);
                break;
            default:
                D_8009E52A = 0;
                break;
            }
            break;
        }
        case 4: {
            unsigned short *p = &D_8009E52A;
            if (*p == 0) {
                if (D_8009E514 > 999999 && D_8009E9F0 != 0 && (unsigned short)(XU - 0xb5c) < 0x20) {
                    D_800A7C20 = 8;
                    D_800A7C18 = 0x21;
                    if (DS >= 0x881) {
                        func_80027F50(0xb6c);
                        if (DS >= 0x8d1) {
                            o->step = 8;
                            D_800A7B7C = 5;
                            D_800A7B79 = 0;
                            D_800A7B7D = 0x41;
                            D_800A7B7E = 0;
                            break;
                        }
                    }
                }
                if (XS >= 0xfab && YS >= -0x1cb) {
                t10:
                    func_80027EA8(o, 1, 0);
                    break;
                }
                if ((unsigned short)(XU - 0x72e) < 0x20 && DS >= 0x881) {
                    func_80027F50(0x73e);
                    if (DS >= 0x895) {
                    t12:
                        func_80027EA8(o, 1, 2);
                        break;
                    }
                }
                if ((unsigned short)(XU - 0xb28) < 0x80 && DS < 0x880) {
                    func_80027F50(0xb68);
                    if (DS < 0x80c) func_80027EA8(o, 1, 1);
                }
                if (DS < 0x87a) {
                    short *q = qsp();
                    *q += 2;
                    if (*q > o->w32) *q = o->w32;
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
        if (D_8009E4E7 == 0) D_8007582C[o->subtype](o);
        break;
    case 4: {
        unsigned char *f = &D_800A7B7E;
        if (*f == 2) {
            func_800EEC38(f - 6, 1, 0);
            D_800A7B7C = 5;
            D_800A7B7D = 0x41;
            *f = 0;
            D_800A7BA6 = 1;
            if (D_8009E964 == 0xff) {
                o->step = 6;
                o->w4a = 0;
            } else {
                o->step++;
            }
        }
        func_8002B1E0(o);
        break;
    }
    case 5:
        func_80020E38(&D_800A7B78);
        PLAYER_H[0]->s2--;
        if (PLAYER_H[0]->s2 < 0x6ae) {
            o->step++;
            func_8001C988(0x18, 0);
            o->w4a = 0x154;
        }
        func_8002B1E0(o);
        break;
    case 6: {
        unsigned char *pl;
        if (o->w4a == 0) {
            pl = &D_800A7B78;
            func_80020E38(pl);
            PLAYER_H[0]->s2--;
            if (PLAYER_H[0]->s2 < 0x67c) {
                o->step++;
                func_800EEC38(pl, 0, 0);
                o->w4a = 0x3c;
            }
            func_8002B1E0(o);
        } else {
            o->w4a--;
        }
        break;
    }
    case 7:
        D_800A7B79 = 0;
        asm volatile("" ::: "memory");
        if (--o->w4a == -1) {
            func_80027EA8(o, 1, 3);
            o->step = 3;
            D_8009E4E7 = 0;
        }
        break;
    case 8:
        o->w48 = 0x3c;
        o->step++;
        break;
    case 9:
        if (--o->w48 == -1) {
            D_8009E93C = 0xd;
            D_8009E93E = 0;
            D_8009E948 = 0;
            D_8009E4E4 = 0;
            D_8009E51D = 3;
            o->step++;
        }
    case 10: {
        short *q;
        if (D_8009E51D == 1) {
            TC_->w4c = 7;
            TC_->w4e = 0;
        }
        q = qsp();
        *q -= 2;
        if (*q < o->w30) PCY32_ = o->w30 << 16;
        break;
    }
    }
}
#endif
