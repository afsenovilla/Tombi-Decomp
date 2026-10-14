// FUNC 8012a124 328 X004
// MATCHING 8012a124 328
#include "TOBJ.H"
extern void func_80129FC8(TObj *);
extern void paintingSlotHintTalk(TObj *);
extern void func_80129E14(TObj *);
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void freeObjectLayer2(TObj *);

void updatePaintingSlotHintNpc(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_80129FC8(o);
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            switch (o->step) {
            case 0:
                if (o->b68) o->step++;
                break;
            case 1:
                paintingSlotHintTalk(o);
                break;
            case 3:
                AnimAdvance(o);
                break;
            }
            break;
        case 1:
            func_80129E14(o);
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
