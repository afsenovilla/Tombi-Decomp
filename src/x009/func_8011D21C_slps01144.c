// VERSION SLPS_01144 x09 8011D21C 1488
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern TObj *D_8009DEE0;
extern TObj *D_800A0C7C;
extern unsigned char D_801145D4[];
extern char D_80010924[];
extern char D_800109F0[];
extern int func_80020E38(TObj *);
extern void func_80020E0C(TObj *, int);
extern void func_80020DE4(TObj *);
extern short func_8012082C_slps01144(TObj *, short, short, unsigned char);
extern void func_8001E458(int, int);
extern void func_800EED18(TObj *, int);
extern void func_800EE71C(TObj *);

void func_8011D21C_slps01144(TObj *o)
{
    unsigned char b;
    unsigned char c;
    short v;
    short u;
    short t;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        func_800EED18(o, 9);
        o->wb6 = 0;
        *(short *)&D_8009DEE0->type = 0;
        o->wb2 = 0;
        {
            int hw = o->h->p.whole;
            o->d34 = o->y.p.whole;
            o->state++;
            o->d30 = hw;
        }
    case 1:
        func_80020E38(o);
        if (*(unsigned short *)o->anim == 0x87) {
            if (o->animFrame & 1)
                v = -0x1c0;
            else
                v = 0x1c0;
            o->velY = -0x2c0;
            o->d88 = 0x100;
            o->velX = v;
            o->state = 2;
        } else {
            o->h->p.whole = D_800A0C7C->velX + o->d30;
            o->y.p.whole = D_800A0C7C->velY + (((unsigned short *)o->anim)[2] + o->d34);
        }
        break;
    case 2:
        if (o->animFrame & 1) {
            o->d88 += 0x10;
            if (o->d88 > 0x170)
                o->d88 = 0x170;
        } else {
            o->d88 -= 0x10;
            if (o->d88 < 0xb0)
                o->d88 = 0xb0;
        }
        o->d8c = (*(unsigned char *)&o->d88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->velY > 0) {
            if (o->animFrame & 1)
                v = -0x100;
            else
                v = 0x100;
            o->b9c = 2;
            o->velX = v;
            o->b9e = 0;
            o->state = 3;
        }
        break;
    case 3:
        if (o->animFrame & 1) {
            o->d88 += 0x20;
            if (o->d88 <= 0x160) goto skip;
            goto turn;
        } else {
            o->d88 -= 0x20;
            if (o->d88 < 0xa0) {
            turn:
                o->anim = D_80010924;
                func_80020E0C(o, 3);
                o->d88 = 0x100;
                o->timer = 0xc;
                o->state++;
            }
        }
    skip:
        o->d8c = (*(unsigned char *)&o->d88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        u = o->h->p.whole - (D_800A0C7C->h->p.whole - D_800A0C7C->box0);
        t = o->y.p.whole - (D_800A0C7C->y.p.whole + (D_800A0C7C->box3 - D_800A0C7C->box2));
        if ((unsigned short)u < D_800A0C7C->box1) {
            if (o->b69 || func_8012082C_slps01144(o, u, t, D_800A0C7C->subtype))
                func_800EE71C(o);
        }
        break;
    case 4:
        if (o->animFrame & 1) {
            o->d88 += 0x10;
            if (o->d88 > 0x130)
                o->d88 = 0x130;
        } else {
            o->d88 -= 0x10;
            if (o->d88 < 0xd0)
                o->d88 = 0xd0;
        }
        o->d8c = (*(unsigned char *)&o->d88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->velY > 0x680)
            o->velY = 0x680;
        u = o->h->p.whole - (D_800A0C7C->h->p.whole - D_800A0C7C->box0);
        t = o->y.p.whole - (D_800A0C7C->y.p.whole + (D_800A0C7C->box3 - D_800A0C7C->box2));
        if ((unsigned short)u < D_800A0C7C->box1) {
            if (o->b69 || func_8012082C_slps01144(o, u, t, D_800A0C7C->subtype)) {
                func_8001E458(0x1c, 0x7f);
                o->b9c = 0;
                o->anim = D_80010924;
                func_80020E0C(o, 4);
                (*(unsigned char *)&o->waa) = 0;
                o->d88 = 0;
                b = D_801145D4[o->wb0];
                o->state++;
                o->d8c = b;
            } else if (--o->timer <= 0) {
                int d;
                (*(unsigned char *)&o->wac) = 1;
                o->d84 = 0;
                if (o->animFrame & 1)
                    d = 0xf0;
                else
                    d = 0x10;
                o->timer = 10;
                o->step = 2;
                o->d88 = d;
                o->d8c = 0;
                o->state = 3;
            }
        }
        break;
    case 5:
        o->h->p.whole += D_800A0C7C->velX;
        o->y.p.whole += D_800A0C7C->velY;
        if (func_80020E38(o)) {
            *(signed char *)&o->b0f = -8;
            *(short *)&D_8009DEE0->type = 0;
            o->b9e = 0;
            o->velX = 0;
            o->velY = 0;
            (*(unsigned char *)&o->waa) = 0;
            o->anim = D_800109F0;
            func_80020DE4(o);
            c = D_801145D4[o->wb0];
            o->step = 0;
            o->state = 0;
            o->d8c = c;
        }
        break;
    }
}
#endif
