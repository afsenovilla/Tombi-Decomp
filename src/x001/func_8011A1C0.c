// FUNC 8011a1c0 872 X001
// MATCHING 8011a1c0 872
#include "TOBJ.H"
typedef struct { short x, y, z, pad; } E8;
extern int D_1F8002D4;
extern unsigned short D_1F8001C8;
extern unsigned char D_8009C942;
extern unsigned char D_8013C47C[];
extern void *D_8013E5AC, *D_8013E5BC;
extern int FUN_800201ac(TObj *, int);
extern void func_8011A528(TObj *);
extern void FUN_80018838(TObj *);

#define E(i) (((E8 *)&o->wb4)[i])

void func_8011A1C0(TObj *o)
{
    TObj *p;
    short dx, dy;

    switch (o->b04) {
    case 0:
        o->w1e = 8;
        o->b0d = 0;
        o->_pad0e[0] = o->b0f;
        o->d3c = D_1F8002D4;
        o->b04++;
        o->step = 0;
        o->state = 0;
        *(short *)((char *)o + 0x98) = 0;
        o->d30 = o->h->raw;
        o->d34 = o->y.raw;
        o->w74 = 0;
        o->w76 = D_8013C47C[o->b0c];
        o->w9a = 0;
        switch (o->subtype) {
        case 0:
            o->anim = D_8013E5AC;
            o->box0 = 0xc;
            o->box1 = 0x18;
            o->box2 = 0xe;
            o->b68 = 0;
            o->b69 = 0;
            o->b6a = 0;
            o->box3 = 0x1a;
            break;
        case 1:
            o->active = 2;
            o->box2 = 0;
            o->box0 = 4;
            o->box1 = 4;
            o->b0f = o->_pad0e[0] + 6;
            break;
        case 2:
            o->b0f = 2;
            o->active = 2;
            o->anim = D_8013E5BC;
            p = (TObj *)o->d90;
            o->box0 = 4;
            o->box1 = 4;
            o->box2 = 0;
            dy = p->y.p.whole - o->y.p.whole;
            dx = p->h->p.whole - o->h->p.whole;
            if (D_1F8001C8 & 1) {
                E(0).y = 0;
                E(0).x = 0;
                E(1).y = 0;
                E(1).x = 0;
                E(2).y = dy;
                E(2).x = 0;
                E(3).y = dy;
                E(3).x = 0;
                E(0).z = dx - o->box0;
                E(1).z = dx + o->box1;
                E(2).z = dx - o->box0;
                E(3).z = dx + o->box1;
            } else {
                E(0).y = 0;
                E(0).z = 0;
                E(1).y = 0;
                E(1).z = 0;
                E(2).y = dy;
                E(2).z = 0;
                E(3).y = dy;
                E(3).z = 0;
                E(0).x = dx - o->box0;
                E(1).x = dx + o->box1;
                E(2).x = dx - o->box0;
                E(3).x = dx + o->box1;
            }
            break;
        }
        break;
    case 1:
        if (D_8009C942 != 0 || o->subtype == 2) {
            FUN_800201ac(o, 0x40);
            break;
        }
        if (o->step == 0) {
            if (FUN_800201ac(o, 0x40) == 0) goto clear;
            if (o->b6a == 1) {
                o->step = 1;
                o->state = 0;
            } else if (o->b68 == 1) {
                if ((o->animFrame >> 1) != 2) o->step = 2;
                else o->step = 3;
                o->state = 0;
            } else if (o->b69 == 4) {
                o->step = 3;
                o->state = 0;
            } else if (o->b69 == 1) {
                o->step = 4;
                o->state = 0;
            }
        } else {
            FUN_800201ac(o, 0x40);
        }
        func_8011A528(o);
    clear:
        o->b68 = 0;
        o->b69 = 0;
        o->w9a = 0;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
