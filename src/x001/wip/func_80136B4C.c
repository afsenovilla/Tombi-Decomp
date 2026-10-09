// FUNC 80136b4c 1744 X001
/* score ~360: structure complete; differences are scheduling of the player-struct (D_800A6038) field loads in case 3 (game uses symbol+offset per access, gcc CSEs the base), (signed char) of r->c loaded with lbu+sll/sra in game, and store order in the four "grab" blocks. Tried separate field externs (worse), hill-climbs. */
#include "TOBJ.H"

typedef struct { signed char a; unsigned char b; unsigned char c; signed char d; } R4;
extern TObj D_800A6038;
extern R4 D_80011E04[];
extern signed char D_80011EB4[];
extern void *D_8013E568[];
extern char D_80077D3C[];
extern unsigned char D_8009CEAB;
extern unsigned char D_8009C942, D_8009C93F, D_8009C93E;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001faf4(TObj *);
extern int MulCos(int, int);
extern int MulNegSinScaled(int, int);
extern void func_8004D620(int, int);
extern short TileCollideAt(TObj *, int, int);
extern void FUN_80020490(TObj *);

void func_80136B4C(TObj *o)
{
    R4 *r;
    int ang;
    int dx, dy;

    switch (o->step) {
    case 0:
        o->step = 1;
        break;
    case 1:
        AnimAdvance(o);
        break;
    case 2:
        D_8009CEAB = 2;
        o->step = 5;
        o->state = 0;
        break;
    case 3:
        D_8009CEAB = 3;
        o->active = 2;
        o->visible = D_800A6038.visible;
        o->category |= 0x80;
        *(signed char *)&o->b0f = -7;
        o->animFrame = D_800A6038.animFrame & 1;
        r = &D_80011E04[D_80011EB4[*(unsigned short *)D_800A6038.anim]];
        o->anim = D_8013E568[r->a];
        if (r->a == 6) {
            o->animFrame ^= 1;
        }
        if (o->animFrame & 1) {
            ang = D_800A6038.d8c + 0x80 - (signed char)r->c;
        } else {
            ang = (signed char)r->c + D_800A6038.d8c;
        }
        ang &= 0xff;
        dx = MulCos(ang, r->d);
        dy = MulNegSinScaled(ang, r->d);
        o->h->p.whole = D_800A6038.h->p.whole + dx;
        o->y.p.whole = D_800A6038.y.p.whole + dy;
        o->d->p.whole = D_800A6038.d->p.whole;
        o->d8c = D_800A6038.d8c;
        o->b0f = D_800A6038.b0f + r->b;
        if (D_800A6038.d64) {
            D_8009C942 = 1;
            D_8009C93F = 1;
            D_8009C93E = 1;
            o->animFrame = 0;
            D_8009CEAB = 2;
            o->timer = 0x3c;
            o->b04 = 2;
            o->step = 5;
            o->state = 0;
            break;
        }
        if (D_800A6038.h->p.whole < 0x371) {
            D_8009C942 = 1;
            D_8009C93F = 1;
            D_8009C93E = 1;
            o->animFrame = 0;
            D_8009CEAB = 2;
            o->timer = 0x3c;
            o->b04 = 2;
            o->step = 5;
            o->state = 0;
        }
        if ((unsigned short)(D_800A6038.h->p.whole - 0xb6f) < 0x50 && D_800A6038.y.p.whole >= -0x118) {
            o->animFrame = 1;
            D_8009C942 = 1;
            D_8009C93F = 1;
            D_8009C93E = 1;
            D_8009CEAB = 2;
            o->timer = 0x3c;
            o->b04 = 2;
            o->step = 5;
            o->state = 0;
        }
        if (D_800A6038.d->p.whole > 0) {
            D_8009C942 = 1;
            D_8009C93F = 1;
            D_8009C93E = 1;
            D_8009CEAB = 2;
            o->animFrame = 1;
            o->b04 = 2;
            o->step = 5;
            o->state = 0;
        }
        break;
    case 5:
        switch (o->state) {
        case 0:
            o->active = 2;
            o->anim = D_8013E568[0];
            D_8009CEAB = 2;
            AnimLoadDuration(o);
            o->movetab = D_80077D3C;
            o->velY = -0x400;
            func_8004D620(0x11, 2);
            o->state++;
            break;
        case 1:
            FUN_8001faf4(o);
            AnimAdvance(o);
            o->y.raw += o->velY << 8;
            o->velY += 0x20;
            if (o->velY > 0) {
                o->state++;
            }
            break;
        case 2:
            FUN_8001faf4(o);
            AnimAdvance(o);
            o->y.raw += o->velY << 8;
            o->velY += 0x20;
            if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x20))) {
                o->anim = D_8013E568[0];
                AnimLoadDuration(o);
                o->state++;
            }
            break;
        case 3:
            FUN_8001faf4(o);
            AnimAdvance(o);
            o->y.raw += o->velY << 8;
            o->velY += 0x20;
            if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
                o->state++;
            }
            break;
        case 4:
            if (AnimAdvance(o)) {
                o->state++;
            }
            break;
        case 5:
            o->anim = D_8013E568[0];
            AnimLoadDuration(o);
            o->movetab = D_80077D3C;
            o->velY = -0x300;
            o->animFrame = !(D_800A6038.h->p.whole < o->h->p.whole);
            FUN_80020490(o);
            o->state++;
            break;
        case 6:
            FUN_8001faf4(o);
            AnimAdvance(o);
            o->y.raw += o->velY << 8;
            o->velY += 0x20;
            if (o->velY > 0) {
                o->state++;
            }
            break;
        case 7:
            FUN_8001faf4(o);
            AnimAdvance(o);
            o->y.raw += o->velY << 8;
            o->velY += 0x20;
            if (TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
                o->anim = D_8013E568[0];
                AnimLoadDuration(o);
                o->state = 5;
            }
            break;
        }
        if (o->visible == 0) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->h->p.whole = 0x9a8;
            o->y.p.whole = -0x200;
            o->d->p.whole = 0;
            o->active = 1;
            o->animFrame = 0;
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
        }
        break;
    }
}
