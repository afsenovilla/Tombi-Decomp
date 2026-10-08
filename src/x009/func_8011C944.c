// FUNC 8011c944 400 X009
// MATCHING 8011c944 400
#include "TOBJ.H"
extern unsigned char D_8009C93E[];
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942[];
extern unsigned char D_800A4553[];
extern int D_800A4568[];
extern unsigned char D_8009CF10;
extern int D_1F800190;
extern int D_1F80018C;

void func_8011C944(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C93F[0] = 1;
        D_8009C93E[0] = 1;
        D_8009C942[0] = 1;
        D_800A4553[0] = 3;
        D_800A4568[0] = 0;
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        o->timer = 60;
        o->state++;
        break;
    case 1:
        if (--o->timer == -1) {
            o->state++;
        }
        break;
    case 2:
        o->w74 += 0x80;
        if (o->w74 > 0x1000) {
            TObj *p = (TObj *)o->d94;
            o->w74 = 0x1000;
            o->timer = 30;
            o->active = 1;
            o->state++;
            p->b6a = 1;
            p = (TObj *)p->d94;
            p->b6a = 1;
        }
        o->w76 = o->w74;
        o->w78 = o->w74;
        break;
    case 3:
        if (--o->timer == -1) {
            o->state = 0;
            o->step++;
            D_800A4553[0] = 6;
            D_8009CF10 = 1;
        }
        break;
    }
}
