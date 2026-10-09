// FUNC 8011ee50 968 X014
/* score 2: box copies fixed with `p_ = d90; p_ += off; box0 = *p_++; box1 = *p_++; box2 = *p_; box3 = p_[1];` (pointer
   assigned in two statements keeps the increments); func_80116C10 takes o; D_800A4553/D_800A4568 as [0] in case 0 but a
   scalar second name in case 4 (debt); the anim table read through an in-struct pointer field (TB) so it may move above
   the scratchpad store. Left: case 1 loads state between the 1st and 2nd call args (game: after the 3rd). Tried b6a/state
   raw vs struct and order, state = state + 1, local copy, inline with int/short params, callee prototypes. */
#include "TOBJ.H"
extern int D_1F80018C, D_1F800190;
extern unsigned char D_8009C93F[], D_8009C93E[], D_8009C942[];
extern unsigned char D_800A4553[];
extern unsigned char D_800A4553b;
extern int D_800A4568[];
extern char D_80077D6C[], D_80077D0C[];
extern void AnimLoadDuration(TObj *);
extern void FUN_8001faf4(TObj *);
extern int func_8011D7B0(TObj *);
extern int func_8011D6BC(TObj *);
extern void func_8011D604(TObj *);
extern void FUN_8001f96c(int, short, short, short);
extern void playSFX(int);
extern void FUN_8001fa88(TObj *, unsigned short);
extern void func_80116C10(TObj *);

#define setbox(o, q) { unsigned char *p_; p_ = (unsigned char *)o->d90; p_ += (q); (o)->box0 = *p_++; (o)->box1 = *p_++; (o)->box2 = *p_; (o)->box3 = p_[1]; }
static __inline__ void setbox_unused(TObj *o, unsigned char *p)
{
    o->box0 = *p++;
    o->box1 = *p++;
    o->box2 = *p++;
    o->box3 = *p++;
}

typedef struct { char pad[0xa8]; void **tbl; } TB;

void func_8011EE50(TObj *o)
{
    unsigned char *p;
    int r;

    switch (o->state) {
    case 0:
        D_8009C93F[0] = 1;
        D_8009C93E[0] = 1;
        D_8009C942[0] = 1;
        D_800A4553[0] = 3;
        D_800A4568[0] = 0;
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        o->b9c = 0;
        o->b6a = 1;
        o->state++;
        setbox(o, 0x3c);
        o->wac = 0xf;
        o->anim = ((TB *)o)->tbl[15];
        AnimLoadDuration(o);
        o->movetab = D_80077D6C;
        break;
    case 1:
        if (o->animFrame & 2) {
            o->y.raw += 0x48000;
            r = func_8011D7B0(o);
        } else {
            FUN_8001faf4(o);
            r = func_8011D6BC(o);
        }
        if (r) {
            o->b6a = 0;
            o->state++;
            FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            playSFX(6);
        }
        if (o->animFrame & 1) o->d8c = (unsigned char)(o->d8c + 0x14);
        else o->d8c = (unsigned char)(o->d8c - 0x14);
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        break;
    case 2:
        if (o->animFrame & 2) o->w7a = o->animFrame & 1;
        else o->w7a = ~o->animFrame & 1;
        playSFX(7);
        o->velV = -0x400;
        o->b9c = 1;
        o->b6a = 0;
        o->movetab = D_80077D0C;
        o->state++;
    case 3:
        if (o->w7a) o->d8c = (unsigned char)(o->d8c + 0x14);
        else o->d8c = (unsigned char)(o->d8c - 0x14);
        FUN_8001fa88(o, o->w7a);
        func_8011D604(o);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->velV > 0) {
            o->b9c = 2;
            if (func_8011D7B0(o)) {
                o->b9c = 0;
                o->d8c = 0;
                o->state++;
                o->animFrame &= 1;
            }
        }
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        break;
    case 4:
        setbox(o, 0x34);
        o->wac = 0xd;
        o->anim = ((TB *)o)->tbl[13];
        AnimLoadDuration(o);
        o->timer = 0x3c;
        o->state++;
        D_1F80018C = o->h->raw;
        D_1F800190 = o->y.raw;
        D_800A4553b = 6;
        func_80116C10(o);
        break;
    case 5:
        if (--o->timer == -1) {
            o->d8c = 0;
            o->step = 2;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
