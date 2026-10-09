// FUNC 8011ccd8 2332 X010
// MATCHING 8011ccd8 2332
#include "TOBJ.H"
typedef struct {
    char pad0[2];
    short w02;
} G330;
extern G330 *D_8009C330;
typedef struct { char pad[0xe8]; short e8, ea; } PObj;
extern TObj *D_8009F0EC;
extern unsigned short D_8009D670;
extern unsigned short D_1F8001F8, D_1F8001FC, D_1F8003C4, D_1F8003C6;
extern unsigned char D_8009D2B0;
extern int D_8009C984[];
extern unsigned char D_801152E8[];
extern int AnimAdvance(TObj *);
extern void SfxPlay2(int, int);
extern void SfxPlay3(int, int);
extern int rcos(int);
extern int rsin(int);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void FUN_800ee428(TObj *);
extern short FUN_800408d8(TObj *, short, short);
extern short TileCollideAt(TObj *, short, short);
extern int func_8011E000(TObj *, TObj *, unsigned char, int);
extern int func_8011EF98(TObj *, TObj *);

#define HEIGHT(a) y = (unsigned)(rsin(a) * (D_8009F0EC->box0 - 0x24)) >> 12
#define HEIGHTN(a) y = (unsigned)(-(rsin(a) * (D_8009F0EC->box0 - 0x24))) >> 12

#define ROT(A, B)                                   \
    {                                               \
        int a1 = 0x1000 - ((A) & 0xfff);            \
        short c1 = rcos(a1) * 8 >> 12;              \
        short s1 = rsin(a1) * 8 >> 12;              \
        int a2 = 0x1000 - (B);                      \
        x = c1 + (-(rcos(a2) * 8) >> 12);           \
        y = s1 + (-(rsin(a2) * 8) >> 12);           \
    }

void func_8011CCD8(TObj *o)
{
    TObj *p;
    short x, y, t;
    int ang;
    int hv;
    int an2;
    volatile unsigned short *pad;

    switch (o->state) {
    case 0:
        PlayerSetAnimIfChanged(o, 10);
        o->d8c = 0;
        o->wb6 = 0;
        D_8009C330->w02 = 0;
        *(unsigned char *)&o->waa = 1;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        {
            int v = D_8009F0EC->h->p.whole + o->wb8;
            o->h->p.whole = (o->animFrame & 1) ? v + 8 : v - 8;
        }
        o->y.p.whole = D_8009F0EC->y.p.whole + o->wba + 8;
        SfxPlay3(4, 0x7f);
        o->state++;
    case 1:
        AnimAdvance(o);
        switch (D_8009F0EC->w7a) {
        case 0:
            if ((short)D_8009F0EC->d30 < 0x800) HEIGHT(D_8009F0EC->d30 & 0xfff);
            else HEIGHTN(D_8009F0EC->d30 & 0xfff);
            break;
        case 1:
            if ((short)D_8009F0EC->d30 < 0x800) HEIGHT(D_8009F0EC->d30 & 0xfff);
            else HEIGHTN(D_8009F0EC->d30 & 0xfff);
            break;
        case 2:
            if (D_8009F0EC->subtype == 3) ang = D_8009F0EC->d30 & 0xfff;
            else ang = (D_8009F0EC->d30 + 0x400) & 0xfff;
            if (ang < 0x800) HEIGHT(ang);
            else HEIGHTN(ang);
            break;
        }
        if (D_8009C330->w02 != 0) o->y.raw += 0x10000;
        D_8009C330->w02 = 1;
        pad = &D_8009D670;
        if (!(*pad & 0x50)) {
            PlayerSetAnimIfChanged(o, 0xc);
        } else if (*pad & 0x10) {
            PlayerSetAnimIfChanged(o, 0x2d);
            o->y.raw += -0x18000;
            {
                int v = o->h->p.whole;
                FUN_800408d8(o, (o->animFrame & 1) ? v - 4 : v + 4, o->y.p.whole - 0x15);
            }
            if ((D_1F8001F8 & 0xf) == 0) SfxPlay2(0x1d, 0);
        } else if (*pad & 0x40) {
            PlayerSetAnimIfChanged(o, 0xc);
            if ((D_1F8001F8 & 0xf) == 0) SfxPlay2(0x1d, 0);
            o->y.raw += 0x18000;
        }
        p = D_8009F0EC;
        if (p->subtype != 3) {
            switch (p->w7a) {
            case 1:
                func_8011E000(o, p, 2, (p->d30 + 0x400) & 0xfff);
                break;
            case 2:
                func_8011E000(o, p, 1, p->d30);
                break;
            }
        }
        if (o->b69 || (hv = o->h->p.whole, TileCollideAt(o, (o->animFrame & 1) ? hv + 9 : hv - 9, o->y.p.whole + 0x10))) {
            o->d8c = D_801152E8[o->wb0];
            D_8009C330->w02 = 0;
            *(unsigned char *)&o->waa = 0;
            o->b9e = 0;
            o->step = 0;
            o->state = 0;
            break;
        }
        t = D_8009F0EC->y.p.whole - y;
        if (t >= o->y.p.whole) o->y.p.whole = t;
        switch (D_8009F0EC->w7a) {
        case 0:
        case 1:
            ang = D_8009F0EC->d30;
            if (o->animFrame & 1) {
                o->h->p.whole -= 4;
                x = ang;
                if (x >= 0x800) {
                    o->d8c = ((x + 0x400) >> 4) & 0xff;
                    ROT(x + 0xc00, ang & 0xfff);
                } else {
                    o->d8c = ((x + 0xc00) >> 4) & 0xff;
                    ROT(x + 0x400, (x + 0x800) & 0xfff);
                }
            } else {
                o->h->p.whole += 4;
                x = ang;
                if (x > 0x800) {
                    o->d8c = ((x + 0x400) >> 4) & 0xff;
                    ROT(x + 0x400, ang & 0xfff);
                } else {
                    o->d8c = ((x + 0xc00) >> 4) & 0xff;
                    ROT(x + 0xc00, (x + 0x800) & 0xfff);
                }
            }
            break;
        case 2:
            if (D_8009F0EC->subtype == 3) an2 = D_8009F0EC->d30 & 0xfff;
            else an2 = (D_8009F0EC->d30 + 0x400) & 0xfff;
            x = an2;
            if (o->animFrame & 1) {
                o->h->p.whole -= 4;
                if (x >= 0x800) {
                    o->d8c = ((x + 0x400) >> 4) & 0xff;
                    ROT(x + 0xc00, an2);
                } else {
                    o->d8c = ((x + 0xc00) >> 4) & 0xff;
                    ROT(x + 0x400, (x + 0x800) & 0xfff);
                }
            } else {
                o->h->p.whole += 4;
                if (x <= 0x800) {
                    o->d8c = ((x + 0xc00) >> 4) & 0xff;
                    ROT(x + 0xc00, (x + 0x800) & 0xfff);
                } else {
                    o->d8c = ((x + 0x400) >> 4) & 0xff;
                    ROT(x + 0x400, an2);
                }
            }
            break;
        }
        ((PObj *)o)->e8 = o->h->p.whole + x;
        ((PObj *)o)->ea = o->y.p.whole + y;
        p = D_8009F0EC;
        if (func_8011EF98(o, p) == 0) {
            {
                Fix16 *h = o->h; int v = h->p.whole; h->p.whole = (o->animFrame & 1) ? v + 4 : v - 4;
            }
            o->b9e = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb2 = 0;
            o->velH = 0;
            o->velV = 0;
            D_8009C330->w02 = 0;
            *(unsigned char *)&o->waa = 0;
            *(unsigned char *)&o->wac = 1;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
            break;
        }
        o->h->p.whole = D_8009F0EC->h->p.whole + o->wb8 - x;
        o->y.p.whole = D_8009F0EC->y.p.whole + o->wba - y;
        break;
    }
    if (D_1F8001FC & D_1F8003C6) {
        D_8009D2B0 = 0;
        *(unsigned char *)&o->waa = 0;
        o->b9c = 1;
        o->b9e = 0;
        D_8009C330->w02 = 0;
        {
            Fix16 *h = o->h; int v = h->p.whole; h->p.whole = (o->animFrame & 1) ? v + 0x10 : v - 0x10;
        }
        if (D_8009C984[0] & 0x40) {
            if (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4) o->ba7 = 1;
        }
        o->step = 2;
        o->state = 0;
    }
}
