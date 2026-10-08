// FUNC 800efc8c 656 X011
// MATCHING 800efc8c 656
#include "TOBJ.H"
typedef struct { TObj o; char p0[6]; unsigned char c6, c7; char p1[0xe3 - 0xc8]; unsigned char e3; } TObjX;
extern TObj *D_800A611C;
extern TObj *D_8009D2E8;
extern int D_8009C934;
extern unsigned char *D_8009C330;
extern TObj *D_8009F0EC;
extern char D_80010C0C[];
extern unsigned short D_8009C960;
extern void AnimLoadDuration(TObjX *o);
extern void SfxPlay2(int a, int b);
extern void SfxPlay3(int a, int b);

void func_800EFC8C(TObjX *x, short f)
{
    TObj *g;
    if (*(unsigned char *)&x->o.wac >= 2) {
        D_8009D2E8 = D_800A611C;
        D_8009D2E8->b04 = 2;
        D_8009D2E8->step = 2;
        D_8009D2E8->state = 0;
    }
    *(unsigned char *)&x->o.wac = 0;
    D_8009C934 = 0;
    x->c7 = 1;
    x->o.b9d = 0;
    x->c6 = 0;
    x->e3 = 0;
    D_8009C330[0] = 0;
    switch (x->o.b9e) {
    case 1:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 7; x->o.state = 0;
        break;
    case 2:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 6; x->o.state = 0;
        break;
    case 3:
        if ((unsigned short)(x->o.velX + 0x1c0) > 0x380) D_8009C330[5] = 1;
        x->o.anim = D_80010C0C;
        AnimLoadDuration(x);
        x->o.b0f = 0;
        g = D_8009F0EC;
        x->o.d30 = g->h->p.whole + x->o.wb8 + 4;
        x->o.d34 = g->y.p.whole + x->o.wba - 4;
        x->o.h->p.whole = x->o.d30;
        x->o.step = 5;
        x->o.state = 0;
        x->o.y.p.whole = x->o.d34 + 12;
        return;
    case 4:
        if ((D_8009F0EC->category & 0x7f) == 2) {
            SfxPlay2(0x1e, 0x1c);
            x->o.step = 8; x->o.state = 0;
        } else {
            if (D_8009F0EC->type == 2) D_8009F0EC->b69 = 1;
            SfxPlay2(0x1e, 0x1c);
            x->o.step = 8; x->o.state = 0;
        }
        break;
    case 5:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.w74 = x->o.h->p.whole;
        x->o.w76 = x->o.y.p.whole;
        if (f != 0) { x->o.step = 0xc; x->o.state = 0; }
        else { x->o.step = 0x24; x->o.state = 0; }
        break;
    case 6:
        x->o.velX = 0;
        x->o.velY = 0;
        SfxPlay3(4, 0x7f);
        if (f != 0) { x->o.step = 0xd; x->o.state = 0; }
        else { x->o.step = 0x25; x->o.state = 0; }
        break;
    case 7:
        if (D_8009C960 != 10) { x->o.step = 0x22; x->o.state = 0; }
        else { x->o.step = 0x46; x->o.state = 0; }
        break;
    case 8:
        x->o.step = 0x2e; x->o.state = 0;
        break;
    case 9:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 6; x->o.state = 0;
        break;
    case 10: case 11: case 12:
        x->o.step = 0x2f; x->o.state = 0;
        break;
    }
}
