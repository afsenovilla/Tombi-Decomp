// FUNC 801194e0 360 X010
// MATCHING 801194e0 360
#include "TOBJ.H"

extern unsigned short D_8012F284[];
extern unsigned short D_8009C962[];
extern unsigned char D_8009D2C3;
extern void FUN_80020078(TObj *, int);
extern void func_80119374(TObj *);
extern void ObjFreeDup(TObj *);

void func_801194E0(TObj *o)
{
    unsigned short *p;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velH = 0;
        o->d38 = 0;
        o->velX = 0;
        p = &D_8012F284[o->subtype * 2];
        o->box0 = *p;
        o->box2 = *p;
        p++;
        o->box1 = *p;
        o->box3 = *p;
        if (D_8009C962[0] == 7) {
            D_8009D2C3 |= 0x40;
        }
        break;
    case 1:
        FUN_80020078(o, 0x88);
        if (D_8009D2C3 & 0x40) {
            func_80119374(o);
        } else if (o->subtype < 2) {
            o->d8c = (o->d8c - 2) & 0xfff;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
