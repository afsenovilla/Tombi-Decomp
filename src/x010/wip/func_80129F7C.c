// FUNC 80129f7c 1768 X010
/* score 36: everything matches except the first test of near() in case 8: the game loads o->d into v1 and
   D_1F800172 into v0 (ours swaps them). Tried: operand orders, temps for d/global, nested/&& forms of near(),
   int/short return types for near()/land(), restructuring case 8. */
#include "TOBJ.H"
typedef struct B { unsigned char c[4]; } B;
extern B D_8012F3D4[];
extern void *D_80132310[];
extern unsigned char D_8012F494[];
extern unsigned short D_1F80016A, D_1F80016E, D_1F800172;
extern void playSFX(int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001f8e4(TObj *);
extern int Rand(void);
extern short TileCollideAt(TObj *, short, short);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    AnimLoadDuration(o);
}

#define SETANIM(k) \
    { unsigned char a0 = D_8012F3D4[k].c[0], a1 = D_8012F3D4[k].c[1], a2 = D_8012F3D4[k].c[2], a3 = D_8012F3D4[k].c[3]; \
    o->wac = k; \
    o->box0 = a0; \
    o->box1 = a1; \
    o->box2 = a2; \
    o->box3 = a3; \
    setAnim(o, D_80132310[k]); }

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

static __inline__ int near(TObj *o)
{
    int k;
    if ((unsigned short)(D_1F800172 - o->d->p.whole + 0x2d) >= 0x5b) return 0;
    k = 0xc0;
    if (k < (unsigned short)(D_1F80016A - o->h->p.whole + 0x60)) return 0;
    return !(k < (unsigned short)(D_1F80016E - o->y.p.whole + 0x60));
}

void func_80129F7C(TObj *o)
{
    FUN_8001f8e4(o);
    switch (o->state) {
    case 0:
        o->b9c = 0;
        o->state++;
        if (*(unsigned short *)&o->wb4) SETANIM(0xf) else SETANIM(0xb)
        break;
    case 1:
        if (AnimAdvance(o)) {
            if (o->visible) playSFX(0xa4);
            o->b9c = 1;
            o->velV = -0x900;
            o->state++;
            if (*(unsigned short *)&o->wb4) SETANIM(0x10) else SETANIM(0xc)
        }
        break;
    case 2:
        o->velV += 0x50;
        o->y.raw += o->velV << 8;
        if (o->velV >= -0xff) o->state++;
        break;
    case 3:
        o->velV += 0x10;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        break;
    case 4:
        o->velV += 0x10;
        if (o->velV > 0x100) o->state++;
        o->y.raw += o->velV << 8;
        if (land(o)) {
            o->b9c = 0;
            o->state = 6;
            if (*(unsigned short *)&o->wb4) SETANIM(0xf) else SETANIM(0xb)
        }
        break;
    case 5:
        o->velV += 0x50;
        if (o->velV > 0x900) o->velV = 0x900;
        o->y.raw += o->velV << 8;
        if (land(o)) {
            o->b9c = 0;
            o->state++;
            if (*(unsigned short *)&o->wb4) SETANIM(0xf) else SETANIM(0xb)
        }
        break;
    case 6:
        if (AnimAdvance(o)) {
            if (D_8012F494[Rand() & 7]) {
                o->velV = -0x900;
                o->state = 2;
            } else {
                o->velV = -0x280;
                o->state++;
            }
            if (o->visible) playSFX(0xa4);
            o->b9c = 1;
            if (*(unsigned short *)&o->wb4) SETANIM(0x10) else SETANIM(0xc)
        }
        break;
    case 7:
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            o->b69 = 0;
            o->state++;
        }
        break;
    case 8:
        o->velV += 0x40;
        if (o->velV > 0x500) o->velV = 0x500;
        o->y.raw += o->velV << 8;
        if (land(o)) {
            if (!near(o)) {
                o->b9c = 0;
                o->step = 0;
                o->state = 3;
            } else {
                o->b9c = 0;
                o->state = 1;
                if (*(unsigned short *)&o->wb4) SETANIM(0xf) else SETANIM(0xb)
            }
        }
        break;
    }
}
