// FUNC 8011c540 456 X004
// MATCHING 8011c540 456
#include "TOBJ.H"

extern short D_800A60D0[];
extern short D_800A60D2;
extern unsigned char D_8009C971, D_8009C970;
extern int ObjCullRegister(TObj *);
extern void playSFX(int);
extern void ObjFreeDup(TObj *);

void func_8011C540(TObj *o)
{
    short *p;

    switch (o->b04) {
    case 0:
        o->b04++;
        if (o->subtype == 0) {
            o->box0 = 0x30;
            o->box1 = 0x5c;
            o->box2 = 0x20;
            o->box3 = 0x34;
        } else {
            o->box0 = 8;
            o->box1 = 0x10;
            o->box2 = 0x20;
            o->box3 = 0x40;
            o->da0 = 0;
        }
        o->b69 = 0;
        break;
    case 1:
        ObjCullRegister(o);
        if (o->subtype) break;
        switch (o->step) {
        case 0:
            if (o->b69) {
                o->step++;
                o->b69 = 0;
                o->timer = 0;
            }
            break;
        case 1:
            if (!o->b69) o->step = 0;
            if (++o->timer >= 0x3c) {
                o->timer = 0;
                p = D_800A60D0;
                if (*p < D_8009C971) {
                    playSFX(9);
                    D_8009C970 = D_800A60D2 = ++*p;
                }
            }
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
