// FUNC 801099b8 1176 X016
// MATCHING 801099b8 1176
#include "TOBJ.H"
extern unsigned char *D_8009C330;
extern unsigned short D_8009D670;
extern unsigned char D_8009D2B0;
extern int D_8009C984[];
extern unsigned char D_801152E8[];
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_8001e560(int, int);
extern short FUN_800408d8(TObj *, short, int);
extern int FUN_80040278(TObj *, short, short);
extern short FUN_80041ca8(TObj *, short, int);
extern void FUN_8003fb90(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_800ee428(TObj *);
extern void FUN_800ef1ac(TObj *);

#define DIR(o, a, b) (o->h->p.whole + ((o->animFrame & 1) ? (a) : (b)))

void FUN_801099b8(TObj *o)
{
    volatile unsigned short *k;
    short r;
    short x;
    short y;

    switch (o->state) {
    case 0:
        FUN_800eeb5c(o, 10);
        *(unsigned char *)&o->da0 = 0;
        o->bbe = 0;
        o->b9e = 6;
        o->d8c = 0;
        o->wb6 = 0;
        *(short *)(D_8009C330 + 2) = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        *(unsigned char *)&o->waa = 1;
        o->state++;
    case 1:
        FUN_8001fec0(o);
        k = &D_8009D670;
        if ((*k & 0x50) == 0) {
            FUN_800eeb5c(o, 10);
        } else if (*k & 0x10) {
            FUN_800eeb5c(o, 0xb);
            if ((*(unsigned short *)0x1F8001F8 & 0xf) == 0) FUN_8001e560(0x1d, 0);
            o->y.raw -= 0x18000;
            FUN_800408d8(o, DIR(o, -4, 4), (short)(o->y.p.whole - 0x15));
        } else if (*k & 0x40) {
            FUN_800eeb5c(o, 0xc);
            if ((*(unsigned short *)0x1F8001F8 & 0xf) == 0) FUN_8001e560(0x1d, 0);
            o->y.raw += 0x28000;
            if (o->b69) {
                *(unsigned char *)&o->waa = 0;
                o->b9e = 0;
                if (o->bbe & 8) {
                    D_8009C330[8] = o->animFrame & 1;
                    if ((o->bbe & 1) != o->animFrame) {
                        FUN_800eeb5c(o, 8);
                        FUN_8001fe94(o, 2);
                        o->step = 0x1b;
                        o->state = 0;
                    } else {
                        FUN_800eeb5c(o, 0x11);
                        o->step = 0x1b;
                        o->state = 0;
                    }
                    break;
                }
                goto reset;
            }
            if ((short)FUN_80040278(o, DIR(o, 2, -2), (short)(o->y.p.whole + 0x10)) > 0) {
                FUN_8003fb90(o);
                *(unsigned char *)&o->waa = 0;
                o->b9e = 0;
                if (o->bbe & 8) {
                    D_8009C330[8] = o->animFrame & 1;
                    if ((o->bbe & 1) != o->animFrame) {
                        FUN_800eeb5c(o, 8);
                        FUN_8001fe94(o, 2);
                        o->step = 0x1b;
                        o->state = 0;
                    } else {
                        FUN_800eeb5c(o, 0x11);
                        o->step = 0x1b;
                        o->state = 0;
                    }
                    break;
                }
            reset:
                o->d8c = D_801152E8[o->wb0];
                o->step = 0;
                o->state = 0;
                break;
            }
        }
        o->h->p.whole += (o->animFrame & 1) ? -2 : 2;
        x = DIR(o, -8, 8);
        y = o->y.p.whole - 0x10;
        r = FUN_80041ca8(o, x, y);
        switch (r) {
        case 0:
            o->h->p.whole += (o->animFrame & 1) ? 4 : -4;
            *(unsigned char *)&o->waa = 0;
            *(unsigned char *)&o->wac = 1;
            o->velX = 0;
            o->velY = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            D_8009C330[8] = 1;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
            break;
        case 2:
            o->b9e = 5;
            o->step = 0x1d;
            o->state = 0;
            break;
        }
        break;
    }
    if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
        D_8009D2B0 = 0;
        o->b9c = 1;
        *(unsigned char *)&o->waa = 0;
        o->b9e = 0;
        o->h->p.whole += (o->animFrame & 1) ? 0x10 : -0x10;
        if (D_8009C984[0] & 0x40) {
            if (*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C4) {
                o->ba7 = 1;
            }
        }
        o->step = 2;
        o->state = 0;
    }
    FUN_800ef1ac(o);
}
