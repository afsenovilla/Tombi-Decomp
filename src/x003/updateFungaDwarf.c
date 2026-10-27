// FUNC 8012b454 392 X003
// MATCHING 8012b454 392
#include "gvars.h"
#include "TOBJ.H"

extern unsigned char GVARS_USE_ITEM_ID;
extern unsigned char D_8009D003;
extern void initFungaDwarf(TObj *);
extern void fungaDwarfTalk(TObj *);
extern int FUN_800202b4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void freeObjectLayer2(TObj *);

static __inline__ int check(void)
{
    return GVARS.useItemPending == 1 && GVARS_USE_ITEM_ID == 0x7d;
}

void updateFungaDwarf(TObj *o)
{
    switch (o->b04) {
    case 0:
        initFungaDwarf(o);
        break;
    case 1:
        FUN_800202b4(o);
        if (o->b0c == 0x28) {
            switch (o->step) {
            case 0:
                if (o->b68) {
                    o->step++;
                } else if (check()) {
                    GVARS.useItemPending = 0;
                    if (D_8009D003 == 0) D_8009D003 = 1;
                    o->step++;
                }
                break;
            case 1:
                fungaDwarfTalk(o);
                break;
            }
        }
        if (o->anim && o->active) FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
