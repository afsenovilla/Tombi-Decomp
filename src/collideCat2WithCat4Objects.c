// FUNC 800491f0 820 MAIN0
// MATCHING 800491f0 820
/* The outer list loop is a real do-while so the refs of the case-32 box test get loop depth 3 (goto loop = depth 2),
   which flips local-alloc's priority order (box/sum/andi chain before lim/slt and o-val/d) and gives the game's
   registers (box+d in v0, d tied to o-val in v1, e-ptr/e-val in a0). The dead `two = 2` in case 0 is debt: it makes
   `two` multi-set so loop.c does not combine the stores' constant 2 with the compare's and hoist it (flow deletes
   the dead store). Body of case 32 copied from the matched twin captureCat2ByCat4Box. */
#include "TOBJ.H"
#include "gamestate.h"

extern short D_1F80019E;
extern unsigned char D_8007B63C[];
void pushObjectOutOfObjectFacing(TObj *o);
void pushObjectOutOfCat4Type01(TObj *o);
void pushObjectOutOfCat4Type02(TObj *o);
void pushCat2ObjectOutOfCat4Type00(TObj *o);
void func_8012032C(TObj *o);
void func_80126D7C(TObj *o);
void pushCat2ObjectOutOfCat4Type0A(TObj *o);
void func_801280DC(TObj *o);
void func_80127FDC(TObj *o);
void func_801216A0(TObj *o);
void func_8011FAE8(TObj *o);
void func_8011FA74(TObj *o);
void func_80128054(TObj *o);
void func_801203D0(TObj *o);
void func_8012788C(TObj *o);
void func_801210B4(TObj *o);
void func_80121A40(TObj *o);
void func_80121994(TObj *o);
void func_8011D4FC(TObj *o);

void collideCat2WithCat4Objects(void)
{
    short n;
    TObj **list;
    TObj **l2;
    TObj *o;
    TObj *e;
    short d;
    unsigned char two;
    n = *(unsigned short *)0x1F800246;
    list = *(TObj ***)0x1F80021C;
    if (*(short *)0x1F80024C == 0) return;
    if (n == 0) return;
    do {
        o = *list++;
        n--;
        if (o->active != 2 && D_8007B63C[o->type] != 0) {
        l2 = *(TObj ***)0x1F800224;
        if ((D_1F80019E = *(unsigned short *)0x1F80024C) != 0)
        do {
            e = *l2++;
            D_1F80019E--;
            if (!(e->active & 1)) continue;
            switch (e->type) {
            case 0: two = 2; pushObjectOutOfObjectFacing(o); break;
            case 1: pushObjectOutOfCat4Type01(o); break;
            case 2: pushObjectOutOfCat4Type02(o); break;
            case 4:
                switch (GAME.selectedArea) {
                case 0: pushCat2ObjectOutOfCat4Type00(o); break;
                case 3: func_8012032C(o); break;
                }
                break;
            case 6: func_80126D7C(o); break;
            case 10: pushCat2ObjectOutOfCat4Type0A(o); break;
            case 15: func_801280DC(o); break;
            case 20: func_80127FDC(o); break;
            case 28: func_801216A0(o); break;
            case 32:
                if (o->type == 0x22 || o->type == 0x2c) {
                    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) < 0x5b) {
                        if ((unsigned short)(e->box0 + (o->h->p.whole - e->h->p.whole)) <= e->box1) {
                            if ((unsigned short)(e->box2 + (o->y.p.whole - e->y.p.whole)) <= e->box3) {
                                two = 2;
                                o->active = two;
                                o->b04 = two;
                                o->step = 0;
                                o->state = 0;
                                e->state++;
                            }
                        }
                    }
                }
                break;
            case 33: func_80128054(o); break;
            case 35: func_8012788C(o); break;
            case 40: func_801210B4(o); break;
            case 31: func_8011FA74(o); break;
            case 34: func_801203D0(o); break;
            case 66:
            case 67: func_8011D4FC(o); break;

            case 52: func_80121994(o); break;
            case 51: func_80121A40(o); break;
            case 30: func_8011FAE8(o); break;
            }
        } while (D_1F80019E != 0);
        }
    } while (n != 0);
}
