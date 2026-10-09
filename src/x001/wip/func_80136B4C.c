// FUNC 80136b4c 1744 X001
/* score 242: structure complete. o26: case 3 rewritten in the style of the matched X009 func_8011B01C (separate
   player-field externs D_800A6039/6066/605C/6078/604E/607C/60C4/6047/609C, arrays as [0], angle as
   `u = ...; t = u; t &= 0xff;` per branch): 361 -> 242. Left: the game's angle tail is `andi v0; move s0,v0` (t and
   a in different regs; int a ties them -> one instruction short, short a adds sll/sra), the case 3 head order
   (D_800A6039 load first), store order in the four "grab" blocks and case 5. */
#include "TOBJ.H"

typedef struct { signed char a; unsigned char b; unsigned char c; signed char d; } R4;
extern TObj D_800A6038;
extern unsigned char D_800A6039;
extern unsigned short D_800A6066;
extern unsigned short *D_800A605C;
extern int D_800A60C4[];
extern Fix16 *D_800A6078[], *D_800A607C[];
extern unsigned short D_800A604E[];
extern unsigned char D_800A6047[];
extern int D_800A609C[];
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
    short a, dx, dy;
    unsigned short u;
    unsigned int t;

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
        o->visible = D_800A6039;
        o->category |= 0x80;
        *(signed char *)&o->b0f = -7;
        o->animFrame = D_800A6066 & 1;
        r = &D_80011E04[D_80011EB4[*D_800A605C]];
        o->anim = D_8013E568[r->a];
        if (r->a == 6) {
            o->animFrame ^= 1;
        }
        if (o->animFrame & 1) {
            u = D_800A60C4[0] + 0x80 - (signed char)r->c;
            t = u; t &= 0xff;
        } else {
            u = (signed char)r->c + D_800A60C4[0];
            t = u; t &= 0xff;
        }
        a = (short)t;
        dx = MulCos(a, r->d);
        dy = MulNegSinScaled(a, r->d);
        o->h->p.whole = D_800A6078[0]->p.whole + dx;
        o->y.p.whole = D_800A604E[0] + dy;
        o->d->p.whole = D_800A607C[0]->p.whole;
        o->d8c = D_800A60C4[0];
        o->b0f = D_800A6047[0] + r->b;
        if (D_800A609C[0]) {
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
        if (D_800A6078[0]->p.whole < 0x371) {
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
        if ((unsigned short)(D_800A6078[0]->p.whole - 0xb6f) < 0x50 && (short)D_800A604E[0] >= -0x118) {
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
        if (D_800A607C[0]->p.whole > 0) {
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
