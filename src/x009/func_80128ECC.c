// FUNC 80128ecc 552 X009
// MATCHING 80128ecc 552
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012B2E4[];
extern unsigned char D_8009CDCC;
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);

void func_80128ECC(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->anim = D_8012B2E4[o->subtype].anims[0x18];
        AnimLoadDuration(o);
        o->velX = -0x100;
        o->velY = 0;
        o->state++;
    case 1:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += 0x60000;
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10);
        o->b69 = 0;
        if (o->h->p.whole < 0x708) o->state = 2;
        break;
    case 2:
        o->animFrame = 0;
        o->anim = D_8012B2E4[o->subtype].anims[0x18];
        AnimLoadDuration(o);
        o->velX = 0x100;
        o->velY = 0;
        o->state++;
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += 0x60000;
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10);
        if (o->h->p.whole > 0x780) o->state = 0;
        break;
    }
    if (o->b68) {
        o->step = 1;
        o->state = 0;
    }
    if (D_8009CDCC == 0xff && !o->visible) o->b04 = 3;
}
