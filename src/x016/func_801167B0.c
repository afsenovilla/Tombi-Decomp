// FUNC 801167b0 688 X016
// MATCHING 801167b0 688
#include "TOBJ.H"

typedef struct { char pad[0x4c]; short w4c; short w4e; } SC;
extern TObj D_800A6038;
extern unsigned char D_8009C93F[], D_8009C93E[], D_8009C942[], D_8009C93C[], D_8009C975[];
extern unsigned char D_800A4553[];
extern int D_800A4568[];
extern int D_1F800190, D_1F80018C;
extern unsigned char D_800A603C[], D_800A603D[];
extern short D_8009CD94[], D_8009CD96[], D_8009CDA0[];
extern unsigned char D_1F8003D1[];
extern SC *D_1F8001D4;
extern unsigned char D_8009E375[], D_8009E376[], D_8009E377[];
extern unsigned char D_8009F085[], D_8009F086[], D_8009F087[];
extern int func_8011650C(TObj *);

void func_801167B0(TObj *o)
{
    short t;

    switch (o->state) {
    case 0:
        if (o->visible == 0) break;
        if (func_8011650C(o) == 0) break;
        o->state++;
        D_800A6038.active = 2;
        D_8009C93F[0] = 1;
        D_8009C93E[0] = 1;
        D_8009C942[0] = 1;
        D_800A4553[0] = 3;
        D_800A4568[0] = 0;
        D_1F800190 = o->y.raw;
        D_1F80018C = o->h->raw;
        o->timer = 10;
        break;
    case 1:
        if (--o->timer == -1) {
            D_800A6038.active = 5;
            D_800A603C[0] = 5;
            D_8009C93E[0] = 0;
            D_800A6038.visible = 0;
            D_800A603D[0] = 0x40;
            o->timer = 0x14;
            o->state++;
            D_8009CD94[0] = 0xe;
            D_8009CD96[0] = 2;
            D_8009CDA0[0] = 0;
        }
        break;
    case 2:
        t = o->timer;
        if (t == 0) {
            D_8009C975[0] = 3;
            D_8009C93C[0] = 1;
            D_1F8003D1[0] = 1;
            o->state++;
        } else {
            o->timer = t - 1;
        }
        o->w74 += 0x80;
        if (o->w74 > 0x4000) {
            o->w74 = 0x4000;
            o->state++;
        }
        o->w76 = o->w74;
        o->w78 = o->w74;
        break;
    case 3:
        o->w74 += 0x80;
        if (o->w74 > 0x4000) {
            o->w74 = 0x4000;
        }
        o->w76 = o->w74;
        o->w78 = o->w74;
        if (D_8009C975[0] == 1) {
            D_1F8001D4->w4c = 7;
            D_1F8001D4->w4e = 0;
            D_8009E375[0] = 0xff;
            D_8009E376[0] = 0xff;
            D_8009E377[0] = 0xff;
            D_8009F085[0] = 0xff;
            D_8009F086[0] = 0xff;
            D_8009F087[0] = 0xff;
        }
        break;
    }
}
