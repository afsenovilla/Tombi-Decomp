// FUNC 80116e78 472 X005
// MATCHING 80116e78 472
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { char c[12]; } B12;

extern short D_1F8001C6;
extern TObj *showMessageBoxDirect(int, int, void *);
extern void func_801162B4(TObj *);
extern void func_801167F0(TObj *);
extern void func_8011698C(TObj *);
extern void func_80116CF8(TObj *);
extern void func_80116B44(TObj *);

void func_80116E78(TObj *o)
{
    B12 v;

    switch ((unsigned short)o->wba) {
    case 0:
        func_801162B4(o);
        break;
    case 1:
        func_801167F0(o);
        break;
    case 2:
        func_8011698C(o);
        break;
    case 3:
        switch (o->state) {
        case 0:
            if (o->b68) {
                v = *(B12 *)&o->a;
                o->d90 = (int)showMessageBoxDirect(2, 0x33, &v);
                ((unsigned char*)&GVARS.b42)[0] = 1;
                ((unsigned char*)&GV2.bb0)[0] = 2;
                ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
                ((unsigned char*)&PLAYER.obj.step)[0] = 0;
                ((unsigned char*)&PLAYER.obj.state)[0] = 0;
                o->wbc = o->animFrame;
                o->animFrame = o->b68 & 1;
                o->state++;
            }
            break;
        case 1:
            if (((TObj *)o->d90)->b04 == 2) {
                ((TObj *)o->d90)->b04 = 3;
                D_1F8001C6 = 0;
                ((unsigned char*)&GVARS.b42)[0] = 0;
                PLAYER.obj.wb2 = 0;
                if (!GVARS.b3f) {
                    ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
                    ((unsigned char*)&PLAYER.obj.step)[0] = 0;
                    ((unsigned char*)&PLAYER.obj.state)[0] = 0;
                }
                o->b68 = 0;
                o->step = 0;
                o->state = 0;
                o->animFrame = o->wbc;
            }
            break;
        }
        break;
    case 4:
        break;
    case 5:
        func_80116CF8(o);
        break;
    case 6:
        func_80116B44(o);
        break;
    }
}
