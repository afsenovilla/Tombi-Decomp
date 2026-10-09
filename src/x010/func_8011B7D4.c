// FUNC 8011b7d4 716 X010
// MATCHING 8011b7d4 716
#include "TOBJ.H"
#include "raw7.h"

typedef struct { unsigned char b0; char p1[6]; unsigned char b7, b8; char p9[0x20 - 9]; short w20; } G;

extern G *D_8009C330;
extern short D_8009C944, D_8009C946[];
extern int D_8009D2E8;
extern void SfxPlay2(int, int);
extern int AnimAdvance(TObj *o);
extern void FUN_8010f400(TObj *o);
extern short TileCollideAt(TObj *o, short x, short y);
extern int ObjCheckHeadCollision(TObj *o);
extern void PlayerSetAnimIfChanged(TObj *o, int);

void func_8011B7D4(TObj *o)
{
    G *g;
    int v;

    switch (o->state) {
    case 0:
        o->b9e = 0;
        o->b69 = 0;
        o->b9c = 1;
        U8(o, 0xa0) = 0;
        U8(o, 0xc8) = 0;
        U8(o, 0xc3) = 0;
        U8(o, 0xac) = 0;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        D_8009C330->w20 = 0;
        SfxPlay2(2, 4);
        o->state++;
    case 1:
        o->state++;
    case 2:
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        FUN_8010f400(o);
        if (U8(o, 0xac) == 2) {
            D_8009D2E8 = S32(o, 0xe4);
            D_8009C330->b8 = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10);
        if (ObjCheckHeadCollision(o)) {
            D_8009C330->b7 = o->animFrame;
            U8(o, 0xac) = 1;
            o->b9c = 2;
            o->timer = 10;
            o->step = 4;
            o->velY = 0;
            o->state = 3;
        }
        if (o->velY > 0) {
            D_8009C330->b7 = o->animFrame;
            o->b9c = 2;
            U8(o, 0xac) = 1;
            o->timer = 10;
            o->step = 4;
            o->state = 3;
            break;
        }
        g = D_8009C330;
        if (g->b0 == 0) {
            if (U8(o, 0xc6)) break;
            o->animFrame = (signed char)g->b7;
            PlayerSetAnimIfChanged(o, 4);
            o->b9d = 0;
            o->timer = 0;
            o->d84 = 0;
            v = 0x10;
            if (o->animFrame & 1) v = 0xf0;
            o->d88 = v;
            o->d8c = 0;
            o->step = 2;
        } else if (U8(o, 0xc8)) {
            g->b8 = 0;
            U8(o, 0xac) = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->step = 0x32;
            o->state = 0;
        }
        break;
    }
}
