// FUNC 8011c5cc 612 X009
// MATCHING 8011c5cc 612
#include "TOBJ.H"
typedef struct { char p[0xb4]; TObj *c[6]; } TX;

extern unsigned char D_8009D136;
extern char D_8012B27C[];
extern int ObjCullRegister(TObj *);
extern void func_8011BFD8(TObj *);
extern void func_8011C30C(TObj *);
extern void func_8011C494(TObj *);
extern void FUN_8003ecb0(char *d, Fix16 *pos, int a, int b, TObj *o);
extern void ObjFreeDup(TObj *);

void func_8011C5CC(TObj *o)
{
    Fix16 v[3];

    switch (o->b04) {
    case 0:
        if (D_8009D136) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->b0a = 0x13;
        o->w74 = 0xc00;
        o->w76 = 0xc00;
        o->w78 = 0xc00;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b69 = 0;
        o->timer = 0x20;
        if (o->b0c == 0) {
            o->box0 = 0x12;
            o->box1 = 0x24;
            o->box2 = 0x24;
            o->box3 = 0x24;
        } else {
            o->active = 2;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (o->b0c == 0) {
            func_8011BFD8(o);
            func_8011C30C(o);
            o->b69 = 0;
        } else {
            o->b04 = ((TObj *)o->d90)->b04;
            o->step = ((TObj *)o->d90)->step;
            o->state = ((TObj *)o->d90)->state;
        }
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (o->b0c == 0) {
                v[0].p.whole = o->h->p.whole;
                v[1].p.whole = o->y.p.whole - 0x20;
                v[2].p.whole = o->d->p.whole;
                FUN_8003ecb0(D_8012B27C, v, 0, -0x400, o);
                ((TX *)o)->c[0]->b04 = 2;
                ((TX *)o)->c[1]->b04 = 2;
                ((TX *)o)->c[2]->b04 = 2;
                ((TX *)o)->c[3]->b04 = 2;
                ((TX *)o)->c[4]->b04 = 2;
                ((TX *)o)->c[5]->b04 = 2;
            }
            o->step++;
            break;
        case 1:
            func_8011C494(o);
            break;
        case 2:
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
