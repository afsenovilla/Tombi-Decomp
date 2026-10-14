// FUNC 8011b01c 908 X009
// MATCHING 8011b01c 908
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { unsigned char a, b, c; signed char d; } P4;
extern unsigned char D_8009CFF8;
extern signed char D_80011EB4[];
extern P4 D_80011E5C[];
extern void *D_8012E024;
extern Fix16 *D_800A6078[0], *D_800A607C[];
extern unsigned short D_800A604E[];
extern short FUN_8001fddc(short, int);
extern short FUN_8001fdac(short, int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001e4f0(int);
extern void FUN_80026e0c(int, int);
extern void FUN_80026c50(int, int, int);
extern void setEventComplete(int, int);

void func_8011B01C(TObj *o)
{
    P4 *p;
    short a, dx, dy;
    unsigned short u;
    unsigned int t;

    o->visible = PLAYER.obj.visible;
    switch (o->state) {
    case 0:
        t = 0;
        if (GVARS.b40 != 0) {
            switch (GVARS.b41) {
            case 5:
            case 7:
            case 0xe:
            case 0x7c:
            case 0x97:
            case 0x98:
                t++;
            }
        }
        if ((short)t) {
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        p = &D_80011E5C[D_80011EB4[*(*(unsigned short **)&PLAYER.obj.anim)]];
        if (D_8009CFF8 == 0) {
            o->b04 = 3;
            break;
        }
        o->anim = D_8012E024;
        if (o->animFrame & 1) {
            u = ((int*)&PLAYER.obj.d8c)[0] + 0x80 - (signed char)p->c;
            t = u; t &= 0xff;
        } else {
            u = (signed char)p->c + ((int*)&PLAYER.obj.d8c)[0];
            t = u; t &= 0xff;
        }
        a = (short)t;
        dx = FUN_8001fddc(a, p->d);
        dy = FUN_8001fdac(a, p->d);
        o->visible = PLAYER.obj.visible;
        o->animFrame = PLAYER.obj.animFrame & 1;
        o->h->p.whole = D_800A6078[0]->p.whole + dx;
        o->y.p.whole = D_800A604E[0] + dy;
        o->d->p.whole = D_800A607C[0]->p.whole;
        o->d8c = ((int*)&PLAYER.obj.d8c)[0];
        o->b0f = ((unsigned char*)&PLAYER.obj.b0f)[0] + p->b;
        o->category |= 0x80;
        FUN_8001fe6c(o);
        break;
    case 1:
        p = &D_80011E5C[D_80011EB4[*(*(unsigned short **)&PLAYER.obj.anim)]];
        if (o->animFrame & 1) {
            u = ((int*)&PLAYER.obj.d8c)[0] + 0x80 - (signed char)p->c;
            t = u; t &= 0xff;
        } else {
            u = (signed char)p->c + ((int*)&PLAYER.obj.d8c)[0];
            t = u; t &= 0xff;
        }
        a = (short)t;
        o->h->p.whole = D_800A6078[0]->p.whole + FUN_8001fddc(a, p->d);
        o->y.p.whole = D_800A604E[0] + FUN_8001fdac(a, p->d);
        o->d->p.whole = D_800A607C[0]->p.whole;
        o->d8c = ((int*)&PLAYER.obj.d8c)[0];
        o->b0f = ((unsigned char*)&PLAYER.obj.b0f)[0] + p->b;
        FUN_8001e4f0(9);
        o->b04 = 3;
        FUN_80026e0c(0xc, 1);
        FUN_80026c50(0xd, 1, 1);
        setEventComplete(0x28, 1);
        o->state = 3;
        break;
    }
}
