// VERSION PCPX_96084 tomba 80024BA0 308
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Background strip piece: subtype 0 is culled normally; subtype 1 is drawn (list 0x1F80022C) while its column +0x0C
// is within -1..+6 of the camera column ((camera +0x4E + +0x42) / 160).
#include "TOBJ.H"
typedef struct { char p[0x42]; short w42; char p2[0x4e - 0x44]; short w4e; } CAM;
extern CAM *D_800DE544;
extern void func_800183FC(TObj *, int);
extern void ObjListPush_1F80022C(TObj *);
extern void func_8001A914_pcpx96084(TObj *);

void func_80024BA0_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->box0 = 0;
        o->box1 = 0;
        o->box2 = 0;
        o->box3 = 0;
        o->b04++;
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            func_800183FC(o, 0xf0);
            break;
        case 1:
            {
                short v = (D_800DE544->w4e + D_800DE544->w42) / 160;
                if (o->b0c >= v - 1 && o->b0c <= v + 6) {
                    o->visible = 1;
                    ObjListPush_1F80022C(o);
                }
            }
            break;
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_8001A914_pcpx96084(o);
        break;
    }
}

#endif
