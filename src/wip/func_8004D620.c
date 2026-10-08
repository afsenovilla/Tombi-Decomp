// FUNC 8004d620 1324 MAIN0
/* score 68 (ncheck): fields >= 0xbe partly raw (per-field struct/raw greedy search), inline callit; left: store order in the init block after the switch, TBL address regs (a0), s1 = D_8007DD0C reg, case -6 adds */
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
    char pad3[6];
    short f1E;
    short f20;
    char pad4[10];
    short f2C;
    char pad5[134];
    short fB4;
    short fB6;
    char pad6[2];
    short fBA;
    short fBC;
    char pad7[6];
    short fC4;
    unsigned short fC6;
    char pad8[6];
    short fCE;
    short fD0;
} O;
#define F00 o->f00
#define F02 o->f02
#define F0A o->f0A
#define F0F o->f0F
#define F10 (*(int *)((char *)o + 0x10))
#define F12 (*(short *)((char *)o + 0x12))
#define F14 o->u14.d
#define F16 (*(short *)((char *)o + 0x16))
#define F18 (*(int *)((char *)o + 0x18))
#define F1E o->f1E
#define F20 o->f20
#define F22 (*(short *)((char *)o + 0x22))
#define F2C o->f2C
#define FB4 o->fB4
#define FB6 o->fB6
#define FBA o->fBA
#define FBC o->fBC
#define FBE (*(short *)((char *)o + 0xbe))
#define FC0 (*(short *)((char *)o + 0xc0))
#define FC2 (*(short *)((char *)o + 0xc2))
#define FC4 o->fC4
#define FC6 o->fC6
#define FC8 (*(short *)((char *)o + 0xc8))
#define FCA (*(unsigned short *)((char *)o + 0xca))
#define FCC (*(short *)((char *)o + 0xcc))
#define FCE o->fCE
#define FD0 o->fD0
typedef struct G { short n, f2, f4, f6; } G;
extern G D_800A4648[];
extern unsigned char D_8009C943;
extern unsigned short D_8009C960;
extern unsigned char *D_8007DD34[];
extern unsigned char *D_8007DE38[];
extern unsigned char D_8007DD0C[];
extern unsigned char D_8007DE24[];
extern signed char D_8007DE88[];
extern unsigned short D_8007D454[];
extern unsigned short D_8007D5A4[];
extern char *D_1F800398;
extern void playSFX(int);
extern O *ObjAlloc(void);
extern void FUN_8004db4c(O *, int);
extern void func_8004DC10(O *, int);

static __inline__ void setpos(O *o, short x, short y)
{
    FBC = 0;
    FBA = -((unsigned short)FC2 >> 1);
    FC0 = y + FBC;
    FBE = x + FBA;
}

static __inline__ void callit(O *o, int a)
{
    int i = 0;
    signed char c;
    do {
        c = D_8007DE88[i];
        if (c == -1) { FUN_8004db4c(o, 0); return; }
        i++;
    } while (c != a);
    FUN_8004db4c(o, 1);
}

#define TBL(x) (*(unsigned short *)(D_1F800398 + (x) * 2 + 0x10))

O *func_8004D620(int a, int b)
{
    int i;
    unsigned char *s1;
    unsigned char *s5;
    short flag;
    O *o;
    unsigned short *p;
    unsigned short k;
    char *base;
    char *t;
    int r;
    unsigned short h;
    unsigned int n;

    D_8009C943 = 0xff;
    for (i = 2; i >= 0; i--) {
        if (D_800A4648[i].n == -1) {
            D_8009C943 = i;
            D_800A4648[i].f2 = i;
            break;
        }
    }
    if (D_8009C943 == 0xff) return 0;
    if (b == 1 || b == 4) playSFX(0xc);
    if ((unsigned)(b - 2) < 2) {
        s1 = D_8007DD34[a];
        s1 += D_8009C960 * 2;
        if (*s1 == 0xff)
            s1 = D_8007DD0C; s1 += D_8009C960 * 2;
    } else {
        s5 = D_8007DE38[D_8009C960];
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
        F14 = (0xd4 - (3 - D_8009C943) * 20) << 16;
        switch (b) {
        case 0:
            FC2 = D_8007D454[a] + 0x60;
            break;
        case 1:
            FC2 = D_8007D454[a] + 0x58;
            break;
        case 2:
        case 3:
            FC2 = D_8007D5A4[a] + 0x10;
            break;
        case 4:
            FC2 = D_8007D454[a] + 0x78;
            break;
        }
        FC4 = 0x14;
        FC6 = 0xffff;
        setpos(o, F12, F16);
        FCE = 8;
        FD0 = 6;
        FCA = D_8009C943;
        FCC = 1;
        F20 = 1;
        F22 = 0;
        *(short *)((char *)D_800A4648 + FCA * 8) = 0;
        t = D_1F800398;
        if ((unsigned)(b - 2) < 2) {
            FC8 = *(unsigned short *)(s1[0] * 2 + t + 0x10) + s1[1];
        } else {
            short c = *s5;
            unsigned short *q = (unsigned short *)(t + (D_8007DE24[D_8009C960] * 2 + 0x10));
            if ((unsigned char)c == 0xff) {
                s1 = D_8007DD0C + D_8009C960 * 2;
                FC8 = *(unsigned short *)(s1[0] * 2 + t + 0x10) + s1[1];
            } else {
                FC8 = c + *q;
            }
        }
        if (b == 1 || b == 4) {
            callit(o, a);
        }
        base = D_1F800398 + *(short *)(D_1F800398 + 8);
        p = (unsigned short *)(base + *(short *)(base + (unsigned short)FC8 * 2));
        while (1) {
            k = *p++;
            if ((unsigned short)(k + 2) < 2) {
                if (b == 0 || b == 3) FUN_8004db4c(o, 2);
                return o;
            }
            if (flag == 0 || (short)k == -12) {
                switch ((short)k) {
                case -5:
                    FCE = FBE + 8;
                    FD0 = FC0 + 8;
                    break;
                case -6:
                    h = (unsigned short)FC2 >> 1;
                    p += 2;
                    FBC = 0;
                    FBA = -h;
                    FC0 += FC4 + FBC;
                    FBE = FBE + h + FBA;
                    break;
                case -8:
                    FC6 = *p++;
                    break;
                case -12:
                    if (b != 1 && b != 4) flag ^= 1;
                    break;
                default:
                    func_8004DC10(o, (short)k);
                    break;
                case -7: case -4: case -3: case -2: case -1:
                    break;
                }
            }
        }
    }
    return o;
}
