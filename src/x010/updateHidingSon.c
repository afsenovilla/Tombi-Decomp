// FUNC 8012e010 260 X010
// MATCHING 8012e010 260
#include "gcd.h"
#include "TOBJ.H"

extern void func_8012DEA0(TObj *);
extern void hidingSonTalk(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void updateHidingSon(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012DEA0(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (GCD.bcdab == 1 && GCD.bcfcc == 0) {
                o->step = 1;
                o->state = 0;
            }
            break;
        case 1:
            hidingSonTalk(o);
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
