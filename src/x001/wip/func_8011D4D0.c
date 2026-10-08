// FUNC 8011d4d0 620 X001
/* score 13: whole function (covers csv piece 8011D5F4); builds a 19-link chain sorted with insertionSortU32. Left:
   only the head-object init block after the sort: the game stores d3c (D_1F8002D4) right after b0c, then reloads
   b04 and loads anim table[0] (anim stored last); every statement order / [0] array tried keeps the anim load or
   the D_1F8002D4 load in the wrong slot. */
#include "TOBJ.H"
extern int D_1F8002D4;
extern void *D_8013E6C4[];
extern TObj *FUN_800184d8(void);
extern void insertionSortU32(int, TObj **);
extern int FUN_800202b4(TObj *);
extern void FUN_80018838(TObj *);

void func_8011D4D0(TObj *o)
{
    TObj *list[20];
    TObj *p, *q;
    int i;

    switch (o->b04) {
    case 0:
        if (o->subtype != 0) {
            o->b04++;
            break;
        }
        list[0] = o;
        for (i = 1; i < 0x13; i++) {
            q = FUN_800184d8();
            if (q == 0) break;
            list[i] = q;
        }
        insertionSortU32(0x13, list);
        i = 1;
        o = list[0];
        o->box0 = 4;
        o->box1 = 8;
        o->box2 = 0xa;
        o->box3 = 0x14;
        o->w1e = 0xa;
        o->b0d = 0;
        o->anim = D_8013E6C4[0];
        o->b0c = 0;
        o->d3c = D_1F8002D4;
        o->wac = 0;
        o->d8c = 0;
        o->b04++;
        q = o;
        do {
            p = list[i];
            p->active = 1;
            p->type = 0x10;
            p->animFrame = 1;
            p->subtype = 1;
            p->wac = i;
            p->b0c = i % 4;
            p->b0a = q->b0a;
            p->d3c = D_1F8002D4;
            p->anim = D_8013E6C4[o->b0c];
            p->w1e = q->w1e;
            p->box0 = 4;
            p->box1 = 8;
            p->box2 = 0xa;
            p->box3 = 0x14;
            p->b0d = 0;
            p->d8c = 0;
            p->d30 = 0;
            p->d34 = 0x100000;
            p->d38 = 0;
            p->h->raw = q->h->raw + p->d30;
            p->y.raw = q->y.raw + p->d34;
            p->d->raw = q->d->raw + p->d38;
            p->d90 = (int)q;
            q->d94 = (int)p;
            i++;
            q = p;
        } while (i < 0x13);
        p->d94 = 0;
        break;
    case 1:
        FUN_800202b4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
