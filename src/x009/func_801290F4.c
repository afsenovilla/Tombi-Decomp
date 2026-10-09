// FUNC 801290f4 592 X009
// MATCHING 801290f4 592
/* debt: register asm("$6") for the player pointer in pushR (without it gcc swaps a2/a3 with the box1 copy and the shared sh 2(a2) tail is not cross-jumped; score 75). */
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012B2E4[];
extern Fix16 *D_800A6078;
extern unsigned short D_800A604E;
extern unsigned char D_8009C93A;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void func_80128ECC(TObj *);
extern void func_80128C44(TObj *);

static __inline__ void pushL(short x, short w)
{
    if (x - w < D_800A6078->p.whole) D_800A6078->p.whole = x - w;
}

static __inline__ void pushR(short w1, short w0, short x)
{
    register Fix16 *p asm("$6") = D_800A6078;
    if (p->p.whole < x + (w1 - w0)) p->p.whole = x + (w1 - w0);
}

void func_801290F4(TObj *o)
{
    if (o->b0c == 0) {
        if ((o->active & 8) && D_8009C93A && (unsigned short)(o->y.p.whole - D_800A604E + 0x80) < 0x100) {
            if (o->animFrame & 1)
                pushL(o->h->p.whole, o->box0);
            else
                pushR(o->box1, o->box0, o->h->p.whole);
        }
        switch (o->step) {
        case 0:
            AnimAdvance(o);
            break;
        case 1:
            switch (o->state) {
            case 0:
                o->animFrame = 1;
                o->anim = D_8012B2E4[o->subtype].anims[24];
                AnimLoadDuration(o);
                o->velX = -0x100;
                o->velY = 0;
                o->state++;
            case 1:
                AnimAdvance(o);
                o->h->raw += o->velX << 8;
                o->y.raw += o->velY << 8;
                if (o->visible == 0) o->state = 2;
                break;
            case 2:
                o->b04 = 2;
                break;
            }
            break;
        }
    } else {
        switch (o->step) {
        case 0:
            func_80128ECC(o);
            break;
        case 1:
            func_80128C44(o);
            break;
        }
    }
}
