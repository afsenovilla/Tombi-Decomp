// FUNC 8011eb08 840 X014
// MATCHING 8011eb08 840
/* csv size 816 + the epilogue piece 8011EE38. Box pointer assigned in two statements keeps the lbu/addiu
   increments; D_800A6038 is the player TObj. Case 5 reads q before it is set (s1 garbage), as in the game. */
#include "TOBJ.H"

extern TObj D_800A6038;
extern void FUN_8001fe6c(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8004258c(void *, int);

#define ANIMS(o) (*(void ***)((char *)(o) + 0xa8))

#define setBox(o, i) { unsigned char *b = (unsigned char *)(o)->d90; b += (i) * 4; \
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
        q = (unsigned char *)&D_800A6038;
        o->state++;
        *q = 2;
        D_800A6038.animFrame = o->h->p.whole > D_800A6038.h->p.whole;
        D_800A6038.b04 = 2;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
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
