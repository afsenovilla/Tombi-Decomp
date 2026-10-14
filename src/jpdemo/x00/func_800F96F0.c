// JPDEMO x00 800F96F0 500
// Demo version of PAL x001 80122044 (func_80122044.c): player animation change inline (PAL calls
// PlayerSetAnimIfChanged). On landing: no tile response, +0x8c kept.
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { char p_[0x2c]; unsigned short animTimer, animFrame; } AnimP_;
extern AnimP_ *D_8009C330_anim;
extern void func_800EDA04(void *);
extern void func_80020554(void *);
#define PlayerSetAnimIfChanged(o, n) {                                                   \
    AnimP_ *p_ = D_8009C330_anim;                                     \
    p_->animTimer = (n);                                              \
    if (p_->animFrame != (n)) {                                       \
        func_800EDA04(o);                                             \
        func_80020554(o);                                             \
        D_8009C330_anim->animFrame = D_8009C330_anim->animTimer;      \
    }                                                                 \
}

typedef struct { char pad[2]; short w2; } PL;
extern PL *D_8009C330;
typedef struct { char pad[0xa2]; unsigned char ba2; char pad2[7]; unsigned char baa; } XB;
#define X(o) ((XB *)(o))
extern unsigned char D_801152E8[];
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern void applyCollisionTileResponse(TObj *);
extern void SfxPlay2(int, int);

void func_80122044(TObj *o)
{
    switch (o->state) {
    case 0:
        PlayerSetAnimIfChanged(o, 0x17);
        o->wb6 = 0;
        D_8009C330->w2 = 0;
        X(o)->ba2 = 3;
        o->wb0 = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d8c = 0;
        X(o)->baa = 1;
        o->timer = 0x20;
        o->y.p.whole += 8;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        o->state++;
    case 1:
        o->y.p.whole++;
        AnimAdvance(o);
        if (--o->timer <= 0) {
            o->state++;
        }
        break;
    case 2:
        o->y.p.whole++;
        AnimAdvance(o);
        if (TileCollideAt(o, o->h->p.whole + ((o->animFrame & 1) ? 9 : -9), o->y.p.whole + 0x10)) {
            ((unsigned char*)&GVARS.b42)[0] = 0;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            X(o)->baa = 0;
            o->b69 = 0;
            o->b9e = 0;
            o->step = 0;
            o->state = 0;
        }
        if ((SPAD.s1f8 & 0xf) == 0) {
            SfxPlay2(0x1d, 0);
        }
        break;
    }
}
