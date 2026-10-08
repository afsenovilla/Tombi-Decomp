// FUNC 80058d28 624 MAIN0
// MATCHING 80058d28 624
#include "TOBJ.H"
typedef struct {
    unsigned int tag;
    unsigned char r0, g0, b0, code;
    short x0, y0;
    unsigned char u0, v0;
    unsigned short clut;
    unsigned short w, h;
} Sprt;
typedef struct { unsigned char a, pa, b, pb, c, pc; } Col6;

extern Sprt *D_1f800164;
extern int D_1f8001e0;
extern unsigned char DAT_8009cd98[];
extern unsigned char DAT_8009cd9c[];
extern unsigned char DAT_8009cd9f;
extern unsigned char DAT_8009d0a4[];
extern unsigned char DAT_8009c990;
extern short DAT_800802dc[];
extern short DAT_800802e4[];
extern Col6 DAT_800802ec[];
extern void FUN_800594e4(TObj *, int, int, int);
extern void FUN_8005e74c(Sprt *);
extern unsigned short FUN_8005e420(int, int);
extern void FUN_8005e580(int, Sprt *);
extern void FUN_80059464(int, int);
extern void FUN_80059728(short *, int, int, int);

void FUN_80058d28(TObj *o, int idx, short x, short y, short flag)
{
    Sprt *p;
    unsigned char *s;
    short r[4];
    int c;

    if (flag)
        FUN_800594e4(o, (short)(x + 8), y, *(unsigned short *)((void **)o->movetab)[DAT_8009cd98[idx] + 1]);
    p = D_1f800164;
    s = (unsigned char *)o->b.raw + *(short *)(*(volatile int *)&o->b.raw + (*(unsigned short *)o->d34 << 2) + 2);
    FUN_8005e74c(p);
    p->code |= 1;
    p->x0 = x;
    p->y0 = y;
    p->u0 = s[0];
    p->v0 = s[1];
    p->w = s[10];
    p->h = s[11];
    c = *((unsigned char *)o + idx + 0x64) + 0x1e4;
    p->clut = FUN_8005e420(0x150, idx * 4 + c);
    FUN_8005e580(D_1f8001e0 + 4, p);
    D_1f800164++;
    FUN_80059464(0x14, 1);
    r[0] = x + 0x13;
    r[1] = y + 2;
    if (!flag && DAT_8009d0a4[DAT_800802dc[idx]] && DAT_8009c990 == DAT_800802e4[idx])
        r[2] = DAT_8009cd9f;
    else
        r[2] = DAT_8009cd9c[idx];
    r[3] = 4;
    FUN_80059728(r, 0xff, 0xff, 0);
    r[0] = x + 0x13;
    r[1] = y + 2;
    r[2] = 0x3d;
    r[3] = 4;
    FUN_80059728(r, DAT_800802ec[idx].a, DAT_800802ec[idx].b, DAT_800802ec[idx].c);
}
