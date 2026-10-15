// FUNC 800e8da4 280 I0NFO3
// MATCHING 800e8da4 280
#include "TOBJ.H"
extern void func_80018838(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800E8A14(TObj *o);
extern void func_800E8B58(TObj *o);
extern void func_800E8C60(TObj *o);
extern unsigned char D_800A60A3;

void func_800E8DA4(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = o->animFrame << 9;
        break;
    case 1:
        if (D_800A60A3 == 0) {
            func_800202B4(o);
            switch (o->subtype) {
            case 2:
                func_800E8A14(o);
                break;
            case 5:
                func_800E8B58(o);
                break;
            case 6:
                func_800E8C60(o);
                break;
            }
        }
        break;
    case 2:
        break;
    case 3:
        func_80018838(o);
        break;
    }
}
