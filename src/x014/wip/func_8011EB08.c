// FUNC 8011eb08 840 X014
/* score 217 (csv size 816 + the epilogue piece 8011EE38): logic and control flow match; the box table reads
   (u8 *)d90 + 4*i are folded by CSE into constant offsets (lbu 0x2c..0x2f(base)) while the game keeps the
   pointer and increments it (addiu b,0x2c; lbu 0(b); addiu b,1 ...). Tried macro/inline/int/index forms and
   function-scope pointer; a non-constant index does keep the increments. */
#include "TOBJ.H"

extern unsigned char D_800A6038[];
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern short D_800A6066;
extern Fix16 *D_800A6078;
extern void FUN_8001fe6c(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8004258c(void *, int);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

#define setBox(o, i) { unsigned char *b = (unsigned char *)(o)->d90 + (i) * 4; \
    (o)->box0 = *b++; (o)->box1 = *b++; (o)->box2 = b[0]; (o)->box3 = b[1]; (o)->wac = (i); }

void func_8011EB08(TObj *o)
{
    unsigned char *q;

    switch (o->state) {
    case 0:
        o->b9c = 0;
        o->timer = 8;
        o->state++;
        setBox(o, 11);
        o->anim = ANIMS(o)[11];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (--o->timer != -1) break;
        setBox(o, 12);
        o->anim = ANIMS(o)[12];
        FUN_8001fe6c(o);
        o->timer = 8;
        o->state++;
        break;
    case 3:
        setBox(o, 12);
        o->anim = ANIMS(o)[12];
        FUN_8001fe6c(o);
        o->timer = 0x78;
        o->state++;
        break;
    case 4:
        if (--o->timer != -1) break;
        setBox(o, 12);
        o->anim = ANIMS(o)[12];
        FUN_8001fe6c(o);
        o->timer = 0x3c;
        o->state++;
        break;
    case 5:
        AnimAdvance(o);
        if (*q & 2) break;
        if (--o->timer != -1) break;
        setBox(o, 14);
        o->anim = ANIMS(o)[14];
        FUN_8001fe6c(o);
        q = D_800A6038;
        o->state++;
        *q = 2;
        D_800A603C = 2;
        D_800A603D = 0;
        D_800A603E = 0;
        D_800A6066 = o->h->p.whole > D_800A6078->p.whole;
        FUN_8004258c(q, 1);
        break;
    case 2:
    case 7:
        break;
    case 6:
        if (AnimAdvance(o)) {
            o->active = 1;
            o->b68 = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
            o->d8c = 0;
        }
        break;
    }
}
