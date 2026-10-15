// FUNC 8012c858 292 X003
// MATCHING 8012c858 292
#include "TOBJ.H"
extern void initTunnelDigger(TObj *);
extern void tunnelDiggerTalk(TObj *);
extern void ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void freeObjectLayer2(TObj *);

void updateTunnelDigger(TObj *o)
{
    switch (o->b04) {
    case 0:
        initTunnelDigger(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->b0c == 0x29) {
            switch (o->step) {
            case 0:
                if (o->b68) {
                    o->step++;
                }
                break;
            case 1:
                tunnelDiggerTalk(o);
                break;
            }
        }
        if (o->anim && o->active) {
            AnimAdvance(o);
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
