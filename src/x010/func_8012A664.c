// FUNC 8012a664 1152 X010
// MATCHING 8012a664 1152
#include "TOBJ.H"
typedef struct B { unsigned char c[4]; } B;
extern char D_80077CF4[];
extern B D_8012F3D4[];
extern void *D_80132310[];
extern void playSFX(int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void FUN_8001fb20(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

#define SETANIM(k) \
    { unsigned char a0 = D_8012F3D4[k].c[0], a1 = D_8012F3D4[k].c[1], a2 = D_8012F3D4[k].c[2], a3 = D_8012F3D4[k].c[3]; \
    o->wac = k; \
    o->box0 = a0; \
    o->box1 = a1; \
    o->box2 = a2; \
    o->box3 = a3; \
    o->anim = D_80132310[k]; \
    AnimLoadDuration(o); }

static __inline__ short wall(TObj *o)
{
    unsigned short f = o->w7a;
    short d;
    if (f & 1) d = -0x10;
    else d = 0x10;
    return func_8004065C(o, o->h->p.whole + d, o->y.p.whole, f);
}
#define WALL() wall(o)

static __inline__ short land(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x1a)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

void func_8012A664(TObj *o)
{
    switch (o->state) {
    case 0:
        if (o->subtype == 2) {
            o->subtype = 1;
            o->wb4 = 1;
            o->w98 = 0;
        }
        playSFX(0xa6);
        o->velV = -0x400;
        o->movetab = D_80077CF4;
        o->b68 = 0;
        o->d8c = 0;
        o->b9c = 1;
        o->state++;
        SETANIM(6);
        break;
    case 1:
        AnimAdvance(o);
        FUN_8001fa88(o, o->w7a);
        WALL();
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b69 = 0;
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_8001fa88(o, o->w7a);
        WALL();
        if (land(o)) {
            o->b9c = 0;
            o->velH = 0x180;
            o->state++;
        }
        break;
    case 3:
        FUN_8001fb20(o);
        if (o->b69 == 1 || TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x1a)) o->b69 = 0;
        if (o->w7a & 1) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        if (WALL()) o->velH = 0;
        o->velH -= 0x20;
        if (o->velH < 0) {
            o->velH = 0;
            o->timer = 0xf0;
            o->state++;
            SETANIM(7);
        }
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->state++;
            SETANIM(8);
        }
        break;
    case 5:
        if (AnimAdvance(o)) {
            o->b68 = 0;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
