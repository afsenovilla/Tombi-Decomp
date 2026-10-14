// FUNC 8011698c 440 X005
// MATCHING 8011698c 440
#include "player.h"
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
extern unsigned char D_8009D2C3, D_8009CDDD, D_8009CEED;
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F;
extern unsigned char D_8009D2B0;
extern short D_1F8001C6;
extern int showMessageBoxDirect(int, int, V6 *);

void func_8011698C(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        if (D_8009D2C3 & 0x10) {
            o->d90 = showMessageBoxDirect(2, 0x59, &v);
        } else if (D_8009CDDD == 0xff) {
            o->d90 = showMessageBoxDirect(2, 0x52, &v);
        } else if (D_8009CEED < 2) {
            o->d90 = showMessageBoxDirect(2, 0x3d, &v);
        } else {
            o->d90 = showMessageBoxDirect(2, 0x43, &v);
        }
        D_8009C942[0] = 1;
        D_8009D2B0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        D_1F8001C6 = 0;
        D_8009C942[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        if (D_8009C93F == 0) {
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        }
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        o->animFrame = o->wbc;
        break;
    }
}
