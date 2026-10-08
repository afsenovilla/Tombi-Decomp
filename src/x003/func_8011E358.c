// FUNC 8011e358 856 X003
// MATCHING 8011e358 856
#include "TOBJ.H"

#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
extern unsigned char *D_8009C330;
extern short D_8009C944[];
extern short D_8009C946[];
extern int D_8009F0EC;
int func_8004BBC0(TObj *o, int a);
void func_800EFC8C(TObj *o, int a);
void PlayerSetAnimIfChanged(TObj *o, int a);
void FUN_800ee428(TObj *o);
void FUN_800fc02c(TObj *o);
void SfxPlay2(int a, int b);
int ObjCheckHeadCollision(TObj *o);
int AnimAdvance(TObj *o);

void func_8011E358(TObj *o)
{
    short v;

    switch (o->state) {
    case 0:
        if (o->animFrame & 1) {
            v = 0x200;
        } else {
            v = -0x200;
        }
        o->velY = -0x600;
        o->velX = v;
        o->timer = 0x14;
        o->state++;
    case 1:
        if (*(short *)((char *)o + 0xe0) != 0) {
            o->active = 3;
        } else {
            o->active = 1;
        }
        PlayerSetAnimIfChanged(o, 0x39);
        SfxPlay2(0x23, 0x24);
        FUN_800fc02c(o);
        break;
    case 2:
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (ObjCheckHeadCollision(o)) {
            o->b9c = 2;
            B(o, 0xac) = 1;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
        }
        if (o->velY > 0) {
            o->b9c = 2;
            B(o, 0xac) = 1;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
        }
        if (AnimAdvance(o)) {
            PlayerSetAnimIfChanged(o, 0x10);
            o->state = 3;
        }
        break;
    case 3:
        o->h->raw += D_8009C944[0] << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->d8c = (o->d8c + ((o->animFrame & 1) ? -0x10 : 0x10)) & 0xff;
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->velY += 0x40;
        o->y.raw += o->velY << 8;
        if (ObjCheckHeadCollision(o)) {
            o->b9c = 2;
            B(o, 0xac) = 1;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
        }
        if (o->velY > 0) {
            o->b9c = 2;
            B(o, 0xac) = 1;
            FUN_800ee428(o);
            o->step = 2;
            o->state = 3;
        }
        break;
    }
    if (--o->timer <= 0) {
        o->timer = 0;
        D_8009F0EC = func_8004BBC0(o, 0);
        if (o->b9e != 0) {
            D_8009C330[8] = 0;
            *(short *)(D_8009C330 + 0x20) = 0;
            B(o, 0xac) = 0;
            o->b9c = 0;
            o->velX = 0;
            o->velY = 0;
            o->wb2 = 0;
            func_800EFC8C(o, D_8009F0EC == 1);
        }
    }
}
