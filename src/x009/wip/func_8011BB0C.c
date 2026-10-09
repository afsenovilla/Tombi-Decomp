// FUNC 8011bb0c 1228 X009
/* score 6: only case 2 differs: game compares D_8009D2B0 (v1) against the dispatcher's constant-2 register (v0); here cse canonicalizes to the switch selector (v1) so D lands in v0. Tried selector casts/locals, compare forms, temps, -fno-cse-follow-jumps (reloads li 2). The case 1 sin add matches with the multi-block int i as temp (no do-while debt needed). */
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } P3;
extern TObj D_800A6038;
extern short D_800A604A;
extern short D_800A604Ax[];
extern unsigned char *D_8009C330;
extern unsigned char D_8009CDAC;
extern unsigned char D_8009C93F;
extern signed char D_8009D2B0;
extern short *D_800A6078[];
extern P3 D_8012B26C[];
extern void *D_8012DB94[];
extern int D_1F8002D4[];
extern short D_8007A3F0[];
extern void AnimLoadDuration(TObj *);
extern int FUN_800202b4(TObj *);
extern void ObjFreeDup(TObj *);

void func_8011BB0C(TObj *o)
{
    TObj *pl = &D_800A6038;
    int i;
    TObj *e;

    switch (o->b04) {
    case 0:
        if (D_8009CDAC == 0) {
            o->b04 = 3;
            break;
        }
        if (D_800A604A < 0x280) {
            o->b04 = 2;
            break;
        }
        o->animFrame = 1;
        if (D_800A6078[0][1] > 0x2d0) {
            i = 1;
            o->step = 0;
            o->b04++;
        } else {
            i = 0;
            o->step = 4;
            o->b04++;
        }
        o->a.raw = D_8012B26C[i].x << 16;
        o->y.raw = D_8012B26C[i].y << 16;
        o->b.raw = D_8012B26C[i].z << 16;
        o->ba7 = 0;
        if (o->subtype == 0) {
            o->box0 = 0x20;
            o->box1 = 0x40;
            o->box2 = 4;
            o->box3 = 0x10;
            *(signed char *)&o->b0f = -10;
        } else {
            o->active = 2;
            o->b0f = 10;
        }
        o->w1e = 1;
        o->b0d = 0;
        o->b69 = 0;
        o->anim = D_8012DB94[o->b0c];
        o->d3c = D_1F8002D4[0];
        AnimLoadDuration(o);
        break;
    case 1:
        FUN_800202b4(o);
        if (o->subtype) {
            e = (TObj *)o->d90;
            o->a.p.whole = e->a.p.whole;
            o->y.p.whole = e->y.p.whole;
            if (D_800A604Ax[0] < 0xee) o->b04++;
            break;
        }
        o->ba7 += 2;
        i = D_8007A3F0[o->ba7] * 2;
        o->y.raw += i;
        switch (o->step) {
        case 0:
            if (pl->a.p.whole < 0x3ac && pl->b69) {
                pl->b04 = 5;
                pl->step = 0;
                pl->state = 0;
                o->velX = -0x80;
                o->wb4 = pl->a.p.whole - o->a.p.whole;
                o->w22 = 0x3c;
                o->step++;
            }
            break;
        case 1:
            if (--o->w22 == 0) o->step++;
            break;
        case 2:
            pl->a.raw += o->velX << 8;
            o->a.p.whole = pl->a.p.whole - o->wb4;
            pl->y.p.whole = o->y.p.whole - 0x10;
            if (pl->a.p.whole < 0x2da) {
                o->timer = 0x1e;
                o->step++;
                o->y.p.whole += 8;
            }
            break;
        case 3:
            pl->a.raw += o->velX << 8;
            o->a.p.whole = pl->a.p.whole - o->wb4;
            pl->y.p.whole = o->y.p.whole - 0x10;
            if (o->a.p.whole < 0x1c2) {
                pl->animFrame = 1;
                D_8009C330[9] = 5;
                pl->b9c = 1;
                pl->velX = -0x240;
                pl->b04 = 6;
                pl->b69 = 0;
                pl->wb2 = 0;
                pl->velH = 0;
                pl->velV = 0;
                pl->velY = 0;
                pl->step = 4;
                pl->state = 0;
                o->step++;
            }
            break;
        case 4:
            if (pl->b69) {
                pl->wb2 = 0;
                pl->b04 = 1;
                pl->step = 0;
                pl->state = 0;
                D_8009C93F = 0;
                o->step++;
            }
            break;
        case 5:
            if (D_8009D2B0 != 3) {
                if (pl->a.p.whole > 0x17e) pl->a.p.whole = 0x17e;
            }
            if (pl->a.p.whole < 0xee) o->b04++;
            break;
        }
        break;
    case 2:
        if (D_8009D2B0 != 2) {
            if (D_800A604A > 0x17e) D_800A604A = 0x17e;
        }
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
