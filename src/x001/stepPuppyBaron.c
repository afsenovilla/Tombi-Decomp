// FUNC 80136b4c 1744 X001
// MATCHING 80136b4c 1744
#include "gvars.h"
/* case 3: D_800A6039 read into a $3-pinned local before the stores; asm("") before r keeps the
   D_800A605C load from being scheduled up into the head. */
#include "TOBJ.H"

typedef struct { signed char a; unsigned char b; unsigned char c; signed char d; } R4;
extern TObj PLAYER;
extern unsigned char D_800A6039[];
extern unsigned short D_800A6066[];
extern unsigned short *D_800A605C;
extern int D_800A60C4[];
extern Fix16 *PLAYER_H[], *PLAYER_D[];
extern unsigned short PLAYER_Y[];
extern unsigned char D_800A6047[];
extern int D_800A609C[];
extern R4 D_80011E04[];
extern signed char D_80011EB4[];
extern void *D_8013E568[];
extern char D_80077D3C[];
extern unsigned char D_8009CEAB[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void applyFrameVelocityX(TObj *);
extern int MulCos(int, int);
extern int MulNegSinScaled(int, int);
extern void printInfoMessage(int, int);
extern short TileCollideAt(TObj *, int, int);
extern void despawnObjectOffscreen(TObj *);

void stepPuppyBaron(TObj *o)
{
    R4 *r;
    register int a asm("$16"); short dx, dy;
    unsigned short u;
    register unsigned int t asm("$2");

    switch (o->step) {
    case 0:
        o->step = 1;
        break;
    case 1:
        AnimAdvance(o);
        break;
    case 2:
        D_8009CEAB[0] = 2;
        o->step = 5;
        o->state = 0;
        break;
    case 3:
        {register unsigned char v asm("$3") = D_800A6039[0];
        D_8009CEAB[0] = 3;
        o->active = 2;
        o->visible = v;
        o->animFrame = D_800A6066[0] & 1;
        o->category |= 0x80;
        *(signed char *)&o->b0f = -7;}
        asm("");
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
        a = t; asm("");
        dx = MulCos(a, r->d);
        dy = MulNegSinScaled(a, r->d);
        o->h->p.whole = PLAYER_H[0]->p.whole + dx;
        o->y.p.whole = PLAYER_Y[0] + dy;
        o->d->p.whole = PLAYER_D[0]->p.whole;
        o->d8c = D_800A60C4[0];
        o->b0f = D_800A6047[0] + r->b;
        if (D_800A609C[0]) {
            GVARS.b42 = 1;
            GVARS.b3f = 1;
            GVARS.b3e = 1;
            o->animFrame = 0;
            D_8009CEAB[0] = 2;
            o->timer = 0x3c;
            o->b04 = 2;
            o->step = 5;
            o->state = 0;
            break;
        }
        if (PLAYER_H[0]->p.whole < 0x371) {
            GVARS.b42 = 1;
            GVARS.b3f = 1;
            GVARS.b3e = 1;
            o->animFrame = 0;
            D_8009CEAB[0] = 2;
            o->timer = 0x3c;
            o->b04 = 2;
            o->step = 5;
            o->state = 0;
        }
        if ((unsigned short)(PLAYER_H[0]->p.whole - 0xb6f) < 0x50 && (short)PLAYER_Y[0] >= -0x118) {
            o->animFrame = 1;
            GVARS.b42 = 1;
            GVARS.b3f = 1;
            GVARS.b3e = 1;
            D_8009CEAB[0] = 2;
            o->timer = 0x3c;
            o->b04 = 2;
            o->step = 5;
            o->state = 0;
        }
        if (PLAYER_D[0]->p.whole > 0) {
            GVARS.b42 = 1;
            GVARS.b3f = 1;
            GVARS.b3e = 1;
            D_8009CEAB[0] = 2;
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
            D_8009CEAB[0] = 2;
            o->anim = D_8013E568[0];
            AnimLoadDuration(o);
            o->movetab = D_80077D3C;
            o->velY = -0x400;
            printInfoMessage(0x11, 2);
            o->state++;
        case 1:
            applyFrameVelocityX(o);
            AnimAdvance(o);
            o->y.raw += o->velY << 8;
            o->velY += 0x20;
            if (o->velY > 0) {
                o->state++;
            }
            break;
        case 2:
            applyFrameVelocityX(o);
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
            applyFrameVelocityX(o);
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
            o->animFrame = !(PLAYER.h->p.whole < o->h->p.whole);
            despawnObjectOffscreen(o);
            o->state++;
            break;
        case 6:
            applyFrameVelocityX(o);
            AnimAdvance(o);
            o->y.raw += o->velY << 8;
            o->velY += 0x20;
            if (o->velY > 0) {
                o->state++;
            }
            break;
        case 7:
            applyFrameVelocityX(o);
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
            GVARS.b42 = 0;
            GVARS.b3f = 0;
            GVARS.b3e = 0;
        }
        break;
    }
}
