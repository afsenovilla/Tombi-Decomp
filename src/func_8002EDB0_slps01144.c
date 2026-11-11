// VERSION SLPS_01144 slps01144 8002EDB0 1516
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gamestate.h"
typedef struct O {
    unsigned char f00;
    char pad0[1];
    unsigned char f02;
    char pad1[7];
    unsigned char f0A;
    char pad2[4];
    unsigned char f0F;
    union { int d; struct { short lo, hi; } p; } u10;
    union { int d; struct { short lo, hi; } p; } u14;
    int f18;
    char pad3[2];
    short f1E;
    char pad4[148];
    short fB4;
    short fB6;
    char pad5[2];
    short fBA;
    char pad6[8];
    short fC4;
    unsigned short fC6;
    char pad7[2];
    unsigned short fCA;
    short fCC;
    short fCE;
} O;
#define F00 o->f00
#define F02 o->f02
#define F0A o->f0A
#define F0F o->f0F
#define F10 (*(int *)((char *)o + 0x10))
#define F12 (*(short *)((char *)o + 0x12))
#define F14 o->u14.d
#define F16 (*(short *)((char *)o + 0x16))
#define F18 o->f18
#define F1E o->f1E
#define F20 (*(short *)((char *)o + 0x20))
#define F22 (*(short *)((char *)o + 0x22))
#define F2C (*(short *)((char *)o + 0x2c))
#define FB4 o->fB4
#define FB6 o->fB6
#define FBA o->fBA
#define FBC (*(short *)((char *)o + 0xbc))
#define FBE (*(short *)((char *)o + 0xbe))
#define FC0 (*(short *)((char *)o + 0xc0))
#define FC2 (*(short *)((char *)o + 0xc2))
#define FC4 (*(short *)((char *)o + 0xc4))
#define FC6 o->fC6
#define FC8 (*(short *)((char *)o + 0xc8))
#define FCA o->fCA
#define FCC (*(short *)((char *)o + 0xcc))
#define FCE o->fCE
#define FD0 (*(short *)((char *)o + 0xd0))
typedef struct G { short n, f2, f4, f6; } G;
extern G D_800A6190[];
extern unsigned char INFO_MESSAGE_SLOT;
extern unsigned char *D_800765D0[];
extern unsigned char *D_8007688C[];
extern unsigned char D_800765A8[];
extern unsigned char D_80076878[];
extern unsigned short D_800766C0[];
extern unsigned short D_80076800[];
extern char *WFM3_DATA;
extern void playSFX(int);
extern O *ObjAlloc(void);
extern int loadMessageGlyph(int, int, int);
typedef struct { unsigned short id; short pad; short x, y; } GE;
extern GE D_800A6920[][128];
extern unsigned char D_800A7926[];

static __inline__ void setpos(O *o, short x, short y)
{
    FBC = 0;
    FBA = -((unsigned short)FC2 >> 1);
    FC0 = y + FBC;
    FBE = x + FBA;
}


O *printInfoMessage(int a, int b)
{
    int i, off;
    unsigned char *s5;
    short flag;
    O *o;
    unsigned short *p;
    unsigned short k;
    unsigned short *q, *q2, *q3;
    register char *base asm("$4");
    unsigned short h;
    short n;
    int r;
    short g;

    { int m;
    INFO_MESSAGE_SLOT = 0xff;
    i = 2; m = -1; off = 16;
    for (; i >= 0; i--) {
        int t;
        asm("" : "=r"(t) : "0"(off));
        if (*(short *)((char *)D_800A6190 + t) == m) {
            INFO_MESSAGE_SLOT = i;
            *(short *)((char *)D_800A6190 + off + 2) = i;
            break;
        }
        off = t - 8;
    }}
    if (INFO_MESSAGE_SLOT == 0xff) return 0;
    if (b == 1) playSFX(0xc);
    if ((unsigned)(b - 2) < 2) {
        p = (unsigned short *)D_800765D0[a];
        p = (unsigned short *)((char *)p + GAME.selectedArea * 2);
        if (*(unsigned char *)p == 0xff)
            { p = (unsigned short *)D_800765A8; p = (unsigned short *)((char *)p + GAME.selectedArea * 2); }
    } else {
        s5 = D_8007688C[GAME.selectedArea];
        s5 += a;
    }
    flag = 0;
    o = ObjAlloc();
    if (o != 0) {
        F00 = 1;
        F02 = 0x1c;
        F0A = 0xc;
        F1E = 0x14;
        F2C = 0;
        F0F = 0;
        F10 = 0xa00000;
        F18 = 0;
        FB4 = a;
        FB6 = b;
        F14 = (0xb4 - (2 - INFO_MESSAGE_SLOT) * 20) << 16;
        if ((unsigned)(b - 2) < 2) FC2 = D_80076800[a] + 0x10;
        else if (b == 0) FC2 = D_800766C0[a] + 0x40;
        else FC2 = D_800766C0[a] + 0x4c;
        FC4 = 0x14;
        *(unsigned short *)((char *)o + 0xc6) = 0xffff;
        setpos(o, F12, F16);
        i = 1;
        *(unsigned short *)((char *)o + 0xca) = INFO_MESSAGE_SLOT;
        F20 = i;
        *(short *)((char *)o + 0xce) = 8;
        FD0 = 7;
        F22 = 0;
        o->fCC = i;
        *(short *)((char *)D_800A6190 + FCA * 8) = 0;
        base = WFM3_DATA;
        if ((unsigned)(b - 2) < 2) {
            q = (unsigned short *)(((unsigned char *)p)[0] * 2 + (int)base); FC8 = q[8] + ((unsigned char *)p)[1];
        } else {
            short c = *s5;
            q = (unsigned short *)(base + (D_80076878[GAME.selectedArea] * 2 + 0x10));
            if ((unsigned char)c == 0xff) {
                p = (unsigned short *)(D_800765A8 + GAME.selectedArea * 2);
                q = (unsigned short *)(((unsigned char *)p)[0] * 2 + (int)base); FC8 = q[8] + ((unsigned char *)p)[1];
            } else {
                FC8 = c + *q;
            }
        }
        base = WFM3_DATA;
        base += *(short *)(base + 8);
        q = (unsigned short *)(base + (unsigned short)FC8 * 2);
        p = (unsigned short *)(base + *(short *)q);
        while (1) {
            k = *p++;
            if ((unsigned short)(k + 2) < 2) return o;
            if (flag == 0 || (short)k == -12) {
                switch ((short)k) {
                case -3:            /* (the retail jump table: -4 is a no-op like -7 .. -1; the first JP link said -4) */
                    FCE = FBE + 8;
                    FD0 = FD0 + 13;
                    break;
                case -5:
                    FCE = FBE + 8;
                    FD0 = FC0 + 8;
                    break;
                case -6: {
                    unsigned short t2, t4, t3;
                    t2 = FC2;
                    t4 = FC4;
                    FC2 = *p++;
                    FC4 = *p++;
                    t3 = *p;
                    FBC = 0;
                    FBA = -((unsigned short)FC2 >> 1);
                    FCE = 8;
                    FD0 = 8;
                    { short z = FBE + (t2 >> 1); FBE = t3 + (z + FBA); }
                    p++;
                    { short z = FC0 + t4; FC0 = z + FBC; }
                    break;
                }
                case -8:
                    FC6 = *p++;
                    break;
                case -12:
                    if (b != 1) flag ^= 1;
                    break;
                default:
                    a = D_800A6190[FCA].n++;
                    r = loadMessageGlyph(FCA, k & 0xfff, 0);
                    g = r;
                    if ((k & 0x7000) == 0x4000) {
                        D_800A6920[FCA][a].id = r | 0x4000;
                        D_800A6920[FCA][a].x = FC2 - 16;
                        D_800A6920[FCA][a].y = FC4 - 16;
                    } else {
                        D_800A6920[FCA][a].id = g;
                        D_800A6920[FCA][a].x = FCE;
                        D_800A6920[FCA][a].y = FD0;
                    }
                    if ((k & 0x7000) == 0x5000 || (k & 0x7000) != 0x6000)
                        FCE += D_800A7926[g * 10];
                    break;
                case -7: case -4: case -2: case -1:
                    break;
                }
            }
        }
    }
    return o;
}
#endif
