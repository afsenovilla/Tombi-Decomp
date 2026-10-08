// FUNC 8004d620 1324 MAIN0
/* score 6 (ncheck): case -6 fixed (o->fC4 struct read + {short t=FC0+fC4; FC0=t+FBC;} / {short t=FBE+h; FBE=t+FBA;}). Left: init block li a0,1 is scheduled after lbu D_8009C943 (game: right after the addu, before sh c0) and sh zero,0x22 lands before sh cc instead of last. Tried: hill-climb of init lines (with setpos expanded too), raw/struct/short/ushort per init store (greedy + 1500 random order+form), F20=FCC=1 chains. b42: game sched2 puts li a0,1 right after its a0 anti-dep (addu v0,v0,a0) => in game's sched1 output li 1 precedes li 8 / setpos insns, but sched1 always puts a li just before its first use; tried local/inline-param 'one', struct (OS cast) vs raw for all 8 stores (256 masks), F20=FCC copies, plateau hill-climb 4x3000. */
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
    char pad8[2];
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
    unsigned short *q;
    char *base;
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
            { s1 = D_8007DD0C; s1 += D_8009C960 * 2; }
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
        setpos(o, F12, F16);
        FC4 = 0x14;
        FCA = D_8009C943;
        F22 = 0;
        FCC = 1;
        FD0 = 6;
        FC6 = 0xffff;
        F20 = 1;
        FCE = 8;
        *(short *)((char *)D_800A4648 + FCA * 8) = 0;
        base = D_1F800398;
        if ((unsigned)(b - 2) < 2) {
            q = (unsigned short *)(s1[0] * 2 + (int)base); FC8 = q[8] + s1[1];
        } else {
            short c = *s5;
            q = (unsigned short *)(base + (D_8007DE24[D_8009C960] * 2 + 0x10));
            if ((unsigned char)c == 0xff) {
                s1 = D_8007DD0C + D_8009C960 * 2;
                q = (unsigned short *)(s1[0] * 2 + (int)base); FC8 = q[8] + s1[1];
            } else {
                FC8 = c + *q;
            }
        }
        if (b == 1 || b == 4) {
            callit(o, a);
        }
        base = D_1F800398;
        base += *(short *)(base + 8);
        q = (unsigned short *)(base + (unsigned short)FC8 * 2);
        p = (unsigned short *)(base + *(short *)q);
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
                    {short t = FC0 + o->fC4; FC0 = t + FBC;}
                    {short t = FBE + h; FBE = t + FBA;}
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
