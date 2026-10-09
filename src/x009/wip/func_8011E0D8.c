/* score 11 (whole 1456 B incl. csv pieces 8011E1A4/8011E358): only case 3 tail block: game does lbu d88/sw d8c first and still reloads o->h early (before sw y); with d8c first in source gcc reloads o->h after sh velY. Tried all orders of d8c/h/y/velY/p lines, raw/int forms of d8c, local h pointers. */
// FUNC 8011e0d8 1456 X009
#include "TOBJ.H"
#include "raw7.h"

extern TObj *DAT_8009c330;
extern TObj *DAT_8009f0ec;
extern unsigned char DAT_801152e8[];
extern char D_80010C50[];
extern char D_80010748[];
extern void PlayerSetAnimIfChanged(TObj *, int);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void AnimJump(TObj *, int);
extern void SfxPlay3(int, int);
extern void FUN_800ee428(TObj *);
extern void FUN_800ee560(TObj *);
extern short func_801215C0(TObj *, short, short, int);

void func_8011E0D8(TObj *o)
{
    TObj *p;
    unsigned short dx, dy;

    switch (o->state) {
    case 0:
        o->d8c = 0;
        PlayerSetAnimIfChanged(o, 9);
        o->wb6 = 0;
        U16(DAT_8009c330, 2) = 0;
        o->wb2 = 0;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->state++;
    case 1:
        AnimAdvance(o);
        if (*(unsigned short *)o->anim == 0x61) {
            short v;
            if (o->animFrame & 1)
                v = -0x1c0;
            else
                v = 0x1c0;
            o->velY = -0x2c0;
            o->d88 = 0x100;
            o->velX = v;
            o->state = 2;
        } else {
            o->h->p.whole = DAT_8009f0ec->velX + o->d30;
            o->y.p.whole = DAT_8009f0ec->velY + (((unsigned short *)o->anim)[2] + o->d34);
        }
        break;
    case 2:
        if (o->animFrame & 1) {
            o->d88 += 0x10;
            if (o->d88 > 0x170) o->d88 = 0x170;
        } else {
            o->d88 -= 0x10;
            if (o->d88 < 0xb0) o->d88 = 0xb0;
        }
        o->d8c = U8(o, 0x88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->velY > 0) {
            short v;
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
        } else {
            o->d88 -= 0x20;
            if (o->d88 >= 0xa0) goto skip;
        }
        o->anim = D_80010C50;
        AnimJump(o, 3);
        o->d88 = 0x100;
        o->timer = 0xc;
        o->state++;
    skip:
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        o->d8c = U8(o, 0x88);
        p = DAT_8009f0ec;
        dx = o->h->p.whole - (p->h->p.whole - p->box0);
        dy = o->y.p.whole - (p->y.p.whole + (p->box3 - p->box2));
        if (dx < p->box1) {
            if (o->b69 || func_801215C0(o, dx, dy, p->subtype))
                FUN_800ee560(o);
        }
        break;
    case 4:
        if (o->animFrame & 1) {
            o->d88 += 0x10;
            if (o->d88 > 0x130) o->d88 = 0x130;
        } else {
            o->d88 -= 0x10;
            if (o->d88 < 0xd0) o->d88 = 0xd0;
        }
        o->d8c = U8(o, 0x88);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x80;
        if (o->velY > 0x680) o->velY = 0x680;
        p = DAT_8009f0ec;
        dx = o->h->p.whole - (p->h->p.whole - p->box0);
        dy = o->y.p.whole - (p->y.p.whole + (p->box3 - p->box2));
        if (dx < p->box1) {
            if (o->b69 || func_801215C0(o, dx, dy, p->subtype)) {
                SfxPlay3(0x1c, 0x7f);
                o->b9c = 0;
                o->anim = D_80010C50;
                AnimJump(o, 4);
                U8(o, 0xaa) = 0;
                o->d88 = 0;
                o->d8c = DAT_801152e8[o->wb0];
                o->state++;
            } else if (--o->timer <= 0) {
                U8(o, 0xac) = 1;
                FUN_800ee428(o);
                o->step = 2;
                o->state = 3;
            }
        }
        break;
    case 5:
        o->h->p.whole += DAT_8009f0ec->velX;
        o->y.p.whole += DAT_8009f0ec->velY;
        if (AnimAdvance(o)) {
            *(signed char *)&o->b0f = -8;
            U16(DAT_8009c330, 2) = 0;
            o->b9e = 0;
            o->velX = 0;
            o->velY = 0;
            U8(o, 0xaa) = 0;
            o->anim = D_80010748;
            AnimLoadDuration(o);
            o->d8c = DAT_801152e8[o->wb0];
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
