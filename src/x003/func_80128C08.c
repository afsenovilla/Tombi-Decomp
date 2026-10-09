// FUNC 80128c08 496 X003
// MATCHING 80128c08 496
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80135D84[];
extern unsigned char D_8009CEEF;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *o, short x, short y);

void func_80128C08(TObj *o)
{
    unsigned char *c;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->anim = D_80135D84[o->subtype].anims[1];
        AnimLoadDuration(o);
        o->velX = -0x200;
        o->velY = 0x800;
        o->state++;
    case 1:
        AnimAdvance(o);
        if (D_8009CEEF) {
            o->anim = D_80135D84[o->subtype].anims[1];
            AnimLoadDuration(o);
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        if (o->h->p.whole < 0x218) o->h->p.whole = 0x218;
        o->y.raw += o->velY << 8;
        if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x10) && o->h->p.whole < 0x219) {
            c = &D_8009CEEF;
            *c = *c + 1;
            o->state++;
        }
        break;
    case 3:
        if (D_8009CEEF == 4) {
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}
