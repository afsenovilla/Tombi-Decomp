// FUNC 80128c44 648 X009
// MATCHING 80128c44 648
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;

extern unsigned char D_8009CDCC[];
extern short D_1F8001C6;
extern AT3 D_8012B2E4[];
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern void setEventStarted(int, int, int);

void func_80128C44(TObj *o)
{
    B12 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(B12 *)&o->a;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        GV2.bb0 = 2;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->anim = D_8012B2E4[o->subtype].p[32];
        FUN_8001fe94(o, 0);
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        if (D_8009CDCC[0] == 0)
            o->d90 = (int)showMessageBoxDirect(2, 6, &v);
        else
            o->d90 = (int)showMessageBoxDirect(2, 7, &v);
        o->state++;
        break;
    case 1:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_8012B2E4[o->subtype].p[24];
        FUN_8001fe6c(o);
        o->timer = 4;
        if (D_8009CDCC[0] == 0) {
            setEventStarted(0x28, 0, 0);
            o->timer = 200;
        }
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        o->state = 2;
        break;
    case 2:
        FUN_8001fec0(o);
        if (--o->timer != 0)
            o->state = 9;
        break;
    case 9:
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
