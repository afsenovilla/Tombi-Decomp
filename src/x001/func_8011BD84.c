// FUNC 8011bd84 980 X001
// MATCHING 8011bd84 980
#include "TOBJ.H"
extern unsigned char D_8009CDB5, D_8009CDA2;
extern unsigned short D_8009C962, D_8009CD96;
extern short D_1F80016E;
extern unsigned short D_1F80016A;
extern int ObjCullRegister(TObj *);
extern void func_8011BC6C(TObj *);
extern void ObjFreeDup(TObj *);

void func_8011BD84(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        if (o->subtype == 0) o->b6b = o->b0f;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->subtype) {
        case 0:
            switch (o->state) {
            case 0:
                if (D_8009CDB5 == 0) {
                    o->state = 3;
                    break;
                }
                if (D_1F80016E < -0x140) break;
                if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x40) < 0x80) o->state++;
                break;
            case 1:
                o->d88 = (o->d88 - 0x10) & 0xfff;
                if (o->d88 < 0xc01) o->state++;
                break;
            case 2:
                if (o->visible == 0) {
                    o->d88 = 0;
                    o->state = 0;
                }
                break;
            }
            o->b0f = o->b6b - 4;
            break;
        case 1:
            func_8011BC6C(o);
            break;
        case 2:
            switch (o->state) {
            case 0:
                switch (D_8009C962) {
                case 0:
                case 1:
                    o->state++;
                    break;
                case 2:
                    o->d88 = 0x800;
                    o->state = 4;
                    break;
                }
                break;
            case 1:
                o->state = ((TObj *)o->d90)->state;
                break;
            case 2:
                o->d88 = (o->d88 - 8) & 0xfff;
                if (o->d88 < 0xa01) o->state++;
                break;
            case 3:
                if (o->visible == 0 && ((TObj *)o->d90)->visible == 0) {
                    o->d88 = 0;
                    o->state = 1;
                }
                break;
            case 4:
                break;
            }
            break;
        case 3:
            switch (o->state) {
            case 0:
                if (D_8009CD96 != 2 && D_8009CDA2 != 0) o->state++;
                break;
            case 1:
                o->d88 = (o->d88 - 0x18) & 0xfff;
                if (o->d88 < 0xa01) o->state++;
                break;
            case 2:
                break;
            }
            break;
        case 4:
            switch (o->state) {
            case 0:
                o->state = ((TObj *)o->d90)->state;
                break;
            case 1:
                o->d88 = (o->d88 + 0x18) & 0xfff;
                if (o->d88 >= 0x600) o->state++;
                break;
            case 2:
                break;
            }
            break;
        case 5:
            switch (o->state) {
            case 0:
                if (D_8009CD96 != 4 && D_8009CDA2 != 0) o->state++;
                break;
            case 1:
                o->d88 = (o->d88 + 0x18) & 0xfff;
                if (o->d88 >= 0x600) o->state++;
                break;
            case 2:
                break;
            }
            break;
        case 6:
            switch (o->state) {
            case 0:
                o->state = ((TObj *)o->d90)->state;
                break;
            case 1:
                o->d88 = (o->d88 - 0x18) & 0xfff;
                if (o->d88 < 0xa01) o->state++;
                break;
            case 2:
                break;
            }
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
