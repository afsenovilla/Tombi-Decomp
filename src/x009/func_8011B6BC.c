// FUNC 8011b6bc 548 X009
// MATCHING 8011b6bc 548
#include "TOBJ.H"
typedef struct { unsigned short b0, b1, b2, b3, w1e; short k; void **anims; } T16;
extern T16 D_8012B23C[];
extern int D_1F8002C8[];
extern unsigned char D_8009CFF8;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void func_8011B508(TObj *);
extern void ObjFreeDup(TObj *);

void func_8011B6BC(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->box0 = D_8012B23C[o->subtype].b0;
        o->box1 = D_8012B23C[o->subtype].b1;
        o->box2 = D_8012B23C[o->subtype].b2;
        o->box3 = D_8012B23C[o->subtype].b3;
        o->w1e = D_8012B23C[o->subtype].w1e;
        o->d3c = D_1F8002C8[D_8012B23C[o->subtype].k];
        o->anim = *D_8012B23C[o->subtype].anims;
        o->d8c = 0;
        o->b0d = 0;
        o->category |= 0x80;
        o->b0a = 2;
        AnimLoadDuration(o);
        o->b04 = 1;
        break;
    case 1:
        ObjCullRegister(o);
        func_8011B508(o);
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            o->step = 1;
            break;
        case 1:
            AnimAdvance(o);
            break;
        case 2:
            o->step = 5;
            o->state = 0;
            break;
        case 3:
            o->b04 = 3;
            break;
        }
        break;
    case 3:
        D_8009CFF8 = 0;
        ObjFreeDup(o);
        break;
    }
}
