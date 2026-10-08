// FUNC 800fb420 2252 X009
// MATCHING 800fb420 2252
#include "TOBJ.H"
typedef struct {
    char p0[2]; short w02; char p4[3];
    unsigned char b7, b8, b9, b0a; char pb;
    short w0c; short w0e; char p10[0x10];
    short w20, w22; char p24[8];
    unsigned short w2c, w2e;
} P;
typedef struct { TObj t; char c0[7]; unsigned char bc7, bc8; char c9[0xe4 - 0xc9]; int de4; } PO;
extern P *DAT_8009c330;
extern int DAT_8009d2e8;
extern int DAT_8009c934;
extern unsigned char DAT_801152e8[];
extern short DAT_80115268[][2];
extern volatile unsigned short DAT_8009d670[];
extern unsigned short DAT_1f8001fc;
extern void FUN_8001e5f4(int, int);
extern void FUN_8001e560(int, int);
extern short FUN_800fb280(void);
extern void FUN_8003f7cc(TObj *);
extern int FUN_8003facc(TObj *);
extern void FUN_800fb0c0(TObj *);
extern void FUN_800fadec(TObj *);
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fe3c(int, int);
extern int FUN_8001fe0c(int, int);

static __inline__ void setanim(TObj *o, unsigned short x)
{
    P *p = DAT_8009c330;
    p->w2c = x;
    if (p->w2e != x) {
        p->w2c = x;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0);
        DAT_8009c330->w2e = DAT_8009c330->w2c;
    }
}

static __inline__ short inc3(int q) { if (q < 3) return q + 1; return q; }
static __inline__ short dec0(int q) { return (q > 0) ? q - 1 : q; }
static __inline__ void anim31(TObj *o)
{
    if (o->wb4 > 0x28) {
        unsigned u = (o->d84 >> 3) & 0xf;
        DAT_8009c330->w2c = 0x31;
        FUN_800efc04(o);
        FUN_8001fe94(o, u);
        DAT_8009c330->w2e = DAT_8009c330->w2c;
    } else {
        DAT_8009c330->w2c = 0x31;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0xf);
        DAT_8009c330->w2e = DAT_8009c330->w2c;
    }
}

void FUN_800fb420(TObj *o)
{
    P *p;
    int v;
    short f;
    unsigned int x;
    unsigned short w;
    int a;
    unsigned short m;
    unsigned short w2;
#define M m
#define M2 m
    int t;

    switch (o->state) {
    case 0:
        o->wb2 = 3;
        o->b9c = 0;
        o->ba4 = 0;
        o->b9e = 0;
        *(unsigned char *)&o->waa = 0;
        if (*(unsigned char *)&o->wac == 2) {
            DAT_8009d2e8 = ((PO *)o)->de4;
            DAT_8009c330->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        *(unsigned char *)&o->wac = 0;
        if (o->b69) {
            DAT_8009c330->w20 = 0;
            DAT_8009c330->w22 = 0;
            DAT_8009c330->b8 = 0;
            DAT_8009c934 = 0;
            FUN_8001e5f4(0x1c, 0x7f);
            ((PO *)o)->bc7 = 1;
            o->b9c = 0;
            *(unsigned char *)&o->wac = 0;
            o->b9d = 0;
            ((PO *)o)->bc8 = 0;
            o->d8c = DAT_801152e8[o->wb0];
            o->step = 0;
            o->state = 0;
            break;
        }
        if (o->velX == 0) {
            w = o->w7a;
            m = o->animFrame;
            a = (w - 0xa0) & 0xff;
            if ((M & 6) == 6) {
                v = 0xc0;
                if (M & 1) v = 0x40;
                o->d84 = v;
                o->wb2 = 0;
            } else if (a > 0x40) {
                if (M & 1) o->d84 = 0x10; else o->d84 = 0x90;
                o->wb2 = 4;
            } else {
                if (M & 1) o->d84 = 0x100 - (short)w; else o->d84 = (short)w;
                o->wb2 = 2;
            }
        } else if (!((m = o->animFrame) & 4)) {
            w2 = o->w7a;
            if (((w2 - 0xa0) & 0xff) > 0x40) {
                if (M2 & 1) o->d84 = 0x10; else o->d84 = 0x90;
                o->wb2 = 3;
            } else {
                if (M2 & 1) o->d84 = 0x100 - (short)w2; else o->d84 = (short)w2;
                o->wb2 = 3;
                if ((unsigned short)(o->velX + 0x100) < 0x200) o->wb2 = 3;
                if ((unsigned short)(o->velX + 0x200) < 0x400) o->wb2 = 3;
            }
        }
        {
            P *q = DAT_8009c330;
            q->b8 = 0;
            q->w20 = 0;
            q->w22 = 0;
        }
        DAT_8009c330->b9 = 0;
        DAT_8009c330->b0a = 0;
        DAT_8009c330->w0c = 0;
        DAT_8009c330->w02 = 0;
        DAT_8009c330->w0e = 0;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b9d = 0;
        FUN_8001e5f4(0x26, 0x7f);
        setanim(o, 0x31);
        DAT_8009c330->b9 = DAT_8009c330->b7 & 1;
        if (o->animFrame & 1) o->d84 = 0x100 - o->w7a; else o->d84 = o->w7a;
        o->d38 = o->wb4 << 16;
        o->animFrame = DAT_8009c330->b9;
        o->timer = 0;
        o->velH = o->h->p.whole;
        o->velV = o->y.p.whole;
        o->state++;
        if (o->wb2 == 0) {
            setanim(o, 0x34);
            o->state = 2;
            break;
        }
    case 1:
        if (o->animFrame & 1) {
            f = (unsigned char)o->d84 - 0x3e;
            if ((unsigned)f < 4) {
                if (o->wb2 >= 4) o->wb2 = 3;
                o->d84 = 0x41;
                FUN_8001e560(3, 0);
                if (DAT_8009d670[0] & 0x80) o->wb2 = inc3(o->wb2);
                if (DAT_8009d670[0] & 0x20) { int q = o->wb2; if (q > 0) q--; o->wb2 = q; }
            }
        } else {
            f = (unsigned char)o->d84 - 0xbe;
            if ((unsigned)f < 4) {
                if (o->wb2 >= 4) o->wb2 = 3;
                o->d84 = 0xc1;
                FUN_8001e560(3, 0);
                if (DAT_8009d670[0] & 0x20) o->wb2 = inc3(o->wb2);
                if (DAT_8009d670[0] & 0x80) { int q = o->wb2; if (q > 0) q--; o->wb2 = q; }
            }
        }
        o->d84 += FUN_800fb280();
        if ((o->d84 & 0xff) < 0x80) o->animFrame = 1; else o->animFrame = 0;
        p = DAT_8009c330;
        if (p->b0a == 0) {
            f = 0;
            o->d88 = (short)FUN_8001fe3c((unsigned char)o->d84, DAT_80115268[o->wb2][0]);
            o->h->p.whole = FUN_8001fe3c((o->d88 + 0xc0) & 0xff, o->wb4) + o->d30;
            o->y.p.whole = FUN_8001fe0c((o->d88 + 0xc0) & 0xff, o->wb4) + o->d34;
            FUN_8003f7cc(o);
            if (o->ba6 == 0) f = FUN_8003facc(o) == 0;
            if ((f << 16) == 0) {
                DAT_8009c330->b0a = 1;
                x = (unsigned char)o->d84;
                if ((unsigned char)(x - 0x40) < 0x80) {
                    DAT_8009c330->w0c = (0x100 - x) & 0xff;
                    o->d84 = *(unsigned char *)&o->d84;
                    DAT_8009c330->b9 = 0;
                } else {
                    DAT_8009c330->w0c = -x & 0xff;
                    o->d84 = *(unsigned char *)&o->d84;
                    DAT_8009c330->b9 = 1;
                }
                if (DAT_8009c330->w0c < o->d84) o->d84 |= 0x100;
            }
        } else if (p->b9) {
            if (p->w0c >= o->d84) p->b9 = 0;
        } else {
            if (o->d84 >= p->w0c) {
                o->d84 += 4;
                p->b0a = 0;
            }
        }
        o->d8c = *(unsigned char *)&o->d88;
        anim31(o);
        FUN_800fb0c0(o);
        if (o->wb2 == 0) {
            setanim(o, 0x34);
            o->state++;
        }
        FUN_800fadec(o);
        o->d84 = *(unsigned char *)&o->d84;
        break;
    case 2:
        o->h->p.whole = FUN_8001fe3c(0xc0, o->wb4) + o->d30;
        o->y.p.whole = FUN_8001fe0c(0xc0, o->wb4) + o->d34;
        FUN_800fb0c0(o);
        if (DAT_1f8001fc & 0xa0) {
            o->wb2 = 1;
            o->state = 1;
        }
        FUN_800fadec(o);
        break;
    }
}
