// FUNC 800ee654 244 I0NFO2
// MATCHING 800ee654 244
#include "TOBJ.H"
extern void func_8001FE6C(TObj *o);
extern void func_800202B4(TObj *o);
extern void topdownItemSpotCheckStep(TObj *o);
extern unsigned char D_800A60A3;
extern short TOPDOWN_ITEM_SPOT_KINDS[];
extern void *TOPDOWN_ITEM_SPOT_TAKEN_ANIMS[][8];

void topdownItemSpotUpdate(TObj *o)
{
    int i;
    func_800202B4(o);
    i = o->b0c * 2;
    switch (o->step) {
    case 0:
        topdownItemSpotCheckStep(o);
        break;
    case 2:
        if (o->state == 0) {
            o->anim = TOPDOWN_ITEM_SPOT_TAKEN_ANIMS[*(short *)((char *)TOPDOWN_ITEM_SPOT_KINDS + i)][0];
            func_8001FE6C(o);
            o->state++;
        }
        break;
    }
    if (D_800A60A3 != 0) {
        if (D_800A60A3 == *(unsigned char *)&o->wb6) o->b6b = 0x80;
        else o->b6b = 0;
    } else {
        if ((unsigned short)o->wb6 != 0) o->b6b = 0;
        else o->b6b = 0x80;
    }
}
