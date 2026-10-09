// FUNC 80118e2c 440 X010
// MATCHING 80118e2c 440
#include "TOBJ.H"

extern unsigned char D_8009D2C3;
extern unsigned char D_8009C958;
extern void FUN_80020078(TObj *, int);
extern void playSFX(int);
extern void func_80118CD4(TObj *);
extern void ObjFreeDup(TObj *);

void func_80118E2C(TObj *o)
{
    unsigned char t = o->b04;
    unsigned char s;

    switch (t) {
    case 0:
        o->b04 = t + 1;
        o->d88 = 0x400;
        o->d84 = 0;
        o->d8c = 0;
        o->active = 2;
        break;
    case 1:
        FUN_80020078(o, 0x80);
        if (D_8009D2C3 & 0x20) break;
        if (o->b0c == 0) break;
        switch (o->subtype) {
        case 0:
            func_80118CD4(o);
            break;
        case 1:
            s = o->step;
            switch (s) {
            case 0:
                if (D_8009C958 == 1) {
                    o->step = s + 1;
                    o->timer = 0x10;
                }
                break;
            case 1:
                o->y.p.whole += 3;
                if (--o->timer == -1) o->step++;
                break;
            case 2:
                o->step = s + 1;
                playSFX(0xa3);
                break;
            }
            if (o->step == 0) func_80118CD4(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
