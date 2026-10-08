// FUNC 8011f288 896 X000
// MATCHING 8011f288 896
/* Holds 3 functions merged by splat: func_8011F288, func_8011F500, func_8011F580. */
#include "TOBJ.H"
extern unsigned char D_800A6047[];
extern void *D_8013B2BC[];
extern int D_1F8002D4[];
extern unsigned char D_800A603C[];
extern unsigned short D_800A6066;
extern unsigned char D_800A603D, D_800A603E;
extern unsigned char D_8009CDAE;
extern short D_800A604E;
extern unsigned char D_800A60A1;
extern Fix16 *D_800A607C[];
extern void AnimLoadDuration(TObj *o);
extern int ObjCullRegister(TObj *o);
extern void AnimAdvance(TObj *o);
extern void ObjFreeDup(TObj *o);

void func_8011F288(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b0a = 0;
        o->b0f = D_800A6047[0] + 1;
        o->anim = D_8013B2BC[0];
        o->d3c = D_1F8002D4[0];
        o->b0d = 0;
        o->w22 = 0;
        o->b.raw = 0;
        o->w1e = 9;
        o->timer = 10;
        o->a.raw = 0xf00000;
        o->y.raw = 0xfe7c0000;
        o->animFrame = 1;
        o->b69 = 0;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 0x58;
        o->box3 = 0x88;
        AnimLoadDuration(o);
        o->step = 0;
        o->state = 0;
        o->b04++;
        break;
    case 1:
        switch (o->step) {
        case 1:
            switch (o->state) {
            case 0:
                o->timer = 0xf0;
                o->state++;
            case 1:
                o->y.p.whole++;
                if (--o->timer == 0) {
                    o->step = 2;
                    o->state = 0;
                    if (D_800A603C[0] != 1) {
                        D_800A6066 = 0;
                        D_800A603C[0] = 1;
                        D_800A603D = 0;
                        D_800A603E = 0;
                    }
                }
                break;
            }
            break;
        case 5:
            if (D_8009CDAE == 0xff && D_800A604E >= -0xc7 && D_800A60A1 != 0 && D_800A603C[0] != 1) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 6:
            o->y.raw = 0xff6c0000;
            break;
        case 0: case 2: case 3: case 4:
            break;
        }
        if (ObjCullRegister(o)) AnimAdvance(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}

void func_8011F500(TObj *o)
{
    if (o->subtype == 0) {
        o->box0 = 0x58;
        o->box1 = 0x60;
        o->box2 = 4;
        o->box3 = 8;
        o->b0a = 0x11;
        o->b0f = D_800A6047[0] + 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d->p.whole = D_800A607C[0]->p.whole;
        o->b04++;
    }
}

void func_8011F580(TObj *o)
{
    switch (o->step) {
    case 0:
        if (D_800A603D == 5) {
            o->timer = 0x80;
            o->step++;
        }
        break;
    case 1:
        o->d8c += 0x20;
        o->y.p.whole += 2;
        if (--o->timer == 0) {
            o->b04 = 2;
            o->step = 0;
        }
        break;
    }
}
