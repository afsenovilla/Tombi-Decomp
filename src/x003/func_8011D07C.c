// FUNC 8011d07c 636 X003
// MATCHING 8011d07c 636
#include "TOBJ.H"

extern int DAT_1f8002e0[];
extern unsigned char D_80135A88[];
extern void *D_80139880[];
extern short D_8007A1F0[], D_8007A5F0[];
extern void FUN_8001fe6c(TObj *);
extern void FUN_80018ca4(TObj *);
extern void FUN_800187e4(TObj *);

void func_8011D07C(TObj *o)
{
    TObj *p;
    int a, v, x;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 0xe;
        o->d3c = DAT_1f8002e0[0];
        *(signed char *)&o->b0f = -9;
        o->b0d = 1;
        o->w08 = 0x7c90;
        o->velH = 4;
        o->animFrame = 0;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->wac >= 5)
            break;
        if ((o->visible = p->visible) == 0)
            break;
        o->state = 0;
        o->wac = D_80135A88[(p->d38 & 0xff) >> 3];
        o->anim = D_80139880[o->wac];
        FUN_8001fe6c(o);
        switch (o->step) {
        case 0:
            o->velH = 0x40;
            o->step++;
            break;
        case 1:
            o->velH += 4;
            if (o->velH == 0x80)
                o->step++;
            break;
        case 2:
            o->velH -= 4;
            if (o->velH == 0)
                o->step--;
            break;
        }
        a = (4 - (o->velH >> 4)) & 0xff;
        o->d8c = a;
        if (p->wac != 2) {
            v = D_8007A1F0[(a + 0x40) & 0xff];
            o->velY = v << 2;
            x = D_8007A5F0[(o->d8c + 0x40) & 0xff];
            x <<= 2;
            o->velX = x;
            p->h->raw = o->d30 + ((short)x << 8);
            p->y.raw = o->d34 + (o->velY << 8);
        }
        FUN_80018ca4(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
