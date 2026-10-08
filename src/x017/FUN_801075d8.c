// FUNC 801075d8 1576 X017
// MATCHING 801075d8 1576
#include "TOBJ.H"
extern unsigned char *D_8009C330;
extern char D_80077D0C[];
extern unsigned char D_8009D2B0[];
extern int D_8009C984[];
extern unsigned short D_8009D670;
extern unsigned short D_1F8001F8;
extern short D_8009C944[], D_8009C946[];
extern unsigned char D_801152E8[];
extern void FUN_801073c0(TObj *);
extern void FUN_801102b4(TObj *);
extern void FUN_800ee9cc(TObj *);
extern void FUN_800f00ac(TObj *);
extern void FUN_80025f40(int, int, int, int);
extern void FUN_800eae0c(int, int, int, int);
extern void FUN_8001faf4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern short FUN_8003fd78(TObj *, int, int);

void FUN_801075d8(TObj *o)
{
    unsigned char *g;
    unsigned char b;

    switch (o->state) {
    case 0:
        ((unsigned char *)o)[0xc7] = 1;
        o->b9d = 0;
        ((unsigned char *)o)[0xc6] = 0;
        ((unsigned char *)o)[0xe3] = 0;
        D_8009C330[0] = 0;
        o->movetab = D_80077D0C;
        *(unsigned char *)&o->wac = 0;
        o->b6b = 0;
        o->ba4 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        D_8009C330[8] = o->bbe & 1;
        if (D_8009C330[8] == 0) {
            if (o->wb2 < -0x144) o->wb2 = -0x144;
        } else {
            if (o->wb2 > 0x144) o->wb2 = 0x144;
        }
        o->state = 1;
        o->animFrame = o->bbe & 1;
    case 1:
        FUN_801073c0(o);
        FUN_801102b4(o);
        FUN_800ee9cc(o);
        if (o->bbe == 0) {
            g = D_8009C330;
            if (g[8] == o->animFrame) goto L;
            g[8] = 0;
            o->b9c = 0;
            o->d88 = 0;
            b = D_801152E8[o->wb0];
            o->ba4 = 0;
            o->step = 1;
            o->state = 0;
            o->d8c = b;
        } else if (o->ba6 >= 2) {
        L:
            o->b9c = 1;
            o->d88 = -0x100;
            o->ba4 = 0;
            if (o->ba6 & 6) {
                o->wb2 = 0;
                o->velH = 0;
                o->velV = 0;
                o->velX = 0;
                o->velY = 0;
            }
            o->step = 0x10;
            o->state = 2;
        }
        FUN_800f00ac(o);
        if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
            D_8009D2B0[0] = 0;
            if (o->wb2 > 0x200) o->wb2 = 0x200;
            if (o->wb2 < -0x200) o->wb2 = -0x200;
            o->ba4 = 0;
            o->b9c = 1;
            if (D_8009C984[0] & 0x40) {
                if (*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C4) {
                    o->ba7 = 1;
                }
            }
            o->step = 2;
            o->state = 0;
        }
        if (*(unsigned short *)(D_8009C330 + 0x2c) < 4) {
            o->d8c = (-o->wb0 << 2) & 0xff;
        }
        if (o->ba4) {
            if ((D_1F8001F8 & 3) == 0) FUN_80025f40(0, 0, 0xff, 2);
            if ((D_1F8001F8 & 0xf) == 0) FUN_800eae0c(o->h->p.whole, o->y.p.whole, o->d->p.whole, 0);
        }
        break;
    case 2:
        if (o->b6b + 0x20 < 0x100) o->b6b = o->b6b + 0x20; else o->b6b = 0xff;
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        FUN_8001faf4(o);
        FUN_8001fec0(o);
        o->d8c = (o->animFrame & 1) ? 0x100 - (o->b6b >> 2) : o->b6b >> 2;
        o->y.raw += o->d88 << 8;
        o->d88 += 0x20;
        if (o->d88 > 0) {
            o->timer = 4;
            o->state = 3;
        }
        break;
    case 3:
        if (--o->timer == 0) {
            o->b9c = 2;
            FUN_800eeb5c(o, 0x12);
            o->state++;
        }
        break;
    case 4:

        if (o->b6b - 0x20 < 0) o->b6b = 0; else o->b6b -= 0x20;
        FUN_8001faf4(o);
        o->d8c = (o->animFrame & 1) ? 0x100 - (o->b6b >> 2) : o->b6b >> 2;
        o->y.raw += o->d88 << 8;
        o->d88 += 0x20;
        if (FUN_8003fd78(o, 0, 0)) {
            if (FUN_8001fec0(o)) {
                o->ba7 = 0;
                D_8009C330[8] = 0;
                o->b9c = 0;
                o->d88 = 0;
                b = D_801152E8[o->wb0];
                o->ba4 = 0;
                o->d8c = b;
                if (((o->animFrame & 1) ? (*(volatile unsigned short *)&D_8009D670 & 0x80) : (*(volatile unsigned short *)&D_8009D670 & 0x20)) == 0) o->wb2 = 0;
                o->step = 0;
                o->state = 0;
            }
            if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
                D_8009D2B0[0] = 0;
                if (o->wb2 > 0x200) o->wb2 = 0x200;
                if (o->wb2 < -0x200) o->wb2 = -0x200;
                o->b9c = 1;
                o->ba4 = 0;
                if (D_8009C984[0] & 0x40) {
                    if (*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C4) {
                        o->ba7 = 1;
                    }
                }
                o->step = 2;
                o->state = 0;
            }
        }
        break;
    }
}
