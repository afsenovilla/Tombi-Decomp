// FUNC 800ea67c 1936 X010
// MATCHING 800ea67c 1936
#include "TOBJ.H"
extern unsigned short DAT_8009c960;
extern unsigned short D_1f8001f8;
extern int D_1f800198;
extern int DAT_1f8002d4[];
extern void *DAT_8013b1a4[];
extern void *DAT_8013e584[];
extern void *DAT_8012e02c[];
extern short DAT_80114a98[];
extern short DAT_80114aa8[];
extern TObj *FUN_80018448(void);
extern unsigned FUN_8001f9e0(void);
extern int FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void FUN_800ea67c(TObj *o)
{
    TObj *c;
    short v;
    int t;

    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            if (--o->w22 == -1) {
                o->w22 = FUN_8001f9e0() & 0xf;
                c = FUN_80018448();
                if (c) {
                    c->active = 1;
                    c->type = 0xc;
                    c->subtype = 1;
                    c->wac = (D_1f8001f8 + D_1f800198) & 1;
                    c->b0c = FUN_8001f9e0() & 7;
                    *c->h = *o->h;
                    c->h->p.whole += o->timer * 2 - 8;
                    c->y = o->y;
                    *c->d = *o->d;
                }
                if (--o->timer == 0) {
                    o->subtype = 1;
                    o->wac = 0;
                    o->b0c = FUN_8001f9e0() & 7;
                }
            }
            break;
        case 1:
            o->b04++;
            if ((D_1f8001f8 + D_1f800198) & 1) o->timer = 0x32; else o->timer = 0x1e;
            o->b0d = 0;
            *(signed char *)&o->b0f = -12;
            if (DAT_8009c960 == 0) {
                o->w1e = 9;
                o->anim = DAT_8013b1a4[o->wac];
            } else if (DAT_8009c960 == 1 || DAT_8009c960 == 7) {
                o->w1e = 8;
                o->anim = DAT_8013e584[o->wac];
            } else if (DAT_8009c960 == 9) {
                o->w1e = 9;
                o->anim = DAT_8012e02c[o->wac];
            }
            o->d3c = DAT_1f8002d4[0];
            switch (o->b0c) {
            case 0:
            case 1:
                o->velV = -0x180;
                o->b0a = 0;
                o->velY = 0x10;
                break;
            case 2:
            case 7:
                o->b0a = 2;
                o->velV = -0x180;
                o->velY = 0xc;
                o->d8c = -0x10;
                o->d84 = 4;
                break;
            case 3:
                o->b0a = 2;
                o->velV = -0x200;
                o->velY = 0x10;
                o->d8c = -0x18;
                o->d84 = 6;
                break;
            case 4:
                o->b0a = 2;
                o->velV = -0x80;
                o->d8c = -0x10;
                o->d84 = 4;
                break;
            case 5:
                o->b0a = 2;
                o->velV = -0x80;
                o->velH = -0x80;
                o->d8c = 0;
                break;
            case 6:
                o->b0a = 2;
                o->velV = -0x80;
                o->velH = 0x80;
                o->d8c = 0;
                break;
            }
            break;
        case 2:
            c = FUN_80018448();
            if (c) {
                c->active = 1;
                c->type = 0xc;
                c->subtype = 3;
                c->wac = (D_1f8001f8 + D_1f800198) & 1;
                c->b0c = o->timer + 7;
                *c->h = *o->h;
                c->y = o->y;
                *c->d = *o->d;
            }
            goto dec;
        case 3:
            o->b04++;
            o->timer = 0x3c;
            *(signed char *)&o->b0f = -12;
            o->b0d = 0;
            o->w1e = 8;
            o->anim = DAT_8013e584[o->wac];
            o->d3c = DAT_1f8002d4[0];
            o->b0a = 2;
            o->d8c = (FUN_8001f9e0() & 3) << 4;
            if (!((o->b0c - 8) & 1))
                o->animFrame = 1;
            else
                o->animFrame = 0;
            o->velH = DAT_80114a98[FUN_8001f9e0() & 7];
            v = DAT_80114aa8[FUN_8001f9e0() & 7];
            o->velY = 0x10;
            o->velV = v;
            break;
        }
        break;
    case 1:
        if (FUN_800202b4(o) == 0)
            goto set3;
        switch (o->b0c) {
        case 0:
            o->velV += o->velY;
            o->y.raw += o->velV << 8;
            o->d->p.whole -= 2;
            break;
        case 1:
            o->velV += o->velY;
            o->y.raw += o->velV << 8;
            o->d->p.whole += 2;
            break;
        case 2:
        case 7:
            o->velV += o->velY;
            o->y.raw += o->velV << 8;
            o->d8c += o->d84;
            if (!((unsigned)(o->d8c + 0xf) < 0x1f))
                o->d84 = -o->d84;
            break;
        case 3:
            o->velV += o->velY;
            o->y.raw += o->velV << 8;
            o->d8c += o->d84;
            if (!((unsigned)(o->d8c + 0xf) < 0x1f))
                o->d84 = -o->d84;
            break;
        case 4:
            o->y.raw += o->velV << 8;
            t = o->d8c + o->d84;
            o->d8c = t;
            if ((unsigned)(t + 0xf) >= 0x1f)
                o->d84 = -o->d84;
            break;
        case 5:
            o->y.raw += o->velV << 8;
            o->h->raw += o->velH << 8;
            o->d8c += 4;
            break;
        case 6:
            o->y.raw += o->velV << 8;
            o->h->raw += o->velH << 8;
            o->d8c -= 4;
            break;
        default:
            if ((o->velV += o->velY) == 0)
                o->velY /= 2;
            o->y.raw += o->velV << 8;
            if (o->velV > 0) {
                o->h->raw += o->velH << 8;
                o->d8c = (o->d8c + 4) & 0xff;
            } else {
                o->h->raw += o->velH << 7;
                o->d8c = (o->d8c + 2) & 0xff;
            }
            break;
        }
    dec:
        if (--o->timer == 0) {
        set3:
            o->b04 = 3;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
