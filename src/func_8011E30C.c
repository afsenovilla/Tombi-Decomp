// FUNC 8011e30c 1324 X000
// MATCHING 8011e30c 1324
#include "TOBJ.H"
extern short D_1F80016E;
extern unsigned short D_1F80016A;
extern unsigned short D_1F80016Eu;
extern unsigned char D_8009CE0B, D_8009CFED, D_8009D13F;
extern unsigned char D_8009C93A[];
extern unsigned short D_800A603C;
extern unsigned char D_800A603Cb, D_800A603D, D_800A603E, D_800A603F;
int func_800202B4(TObj *o);
void func_8011E1B0(TObj *o);
void func_80018838(TObj *o);

void func_8011E30C(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->ba4 = 0;
        break;
    case 1:
        if (func_800202B4(o)) {
            switch (o->subtype) {
            case 0:
                switch (o->state) {
                case 0:
                    if (D_1F80016E < -0x78) break;
                    if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x40) < 0x80) o->state++;
                    break;
                case 1:
                    o->d88 = (o->d88 - 0x10) & 0xfff;
                    if (o->d88 < 0xc01) o->state++;
                    break;
                case 2:
                    break;
                }
                break;
            case 1:
                if (D_8009CE0B) func_8011E1B0(o);
                break;
            case 2:
                switch (o->state) {
                case 0:
                    if ((o->state = ((TObj *)o->d90)->state) == 3) o->d88 = 0x600;
                    break;
                case 1:
                    o->d88 = (o->d88 + 0x20) & 0xfff;
                    if (o->d88 >= 0x600) o->state++;
                    break;
                case 2:
                    break;
                case 3:
                    o->d88 = (o->d88 - 0x20) & 0xfff;
                    if (o->d88 == 0) o->state = 0;
                    break;
                }
                break;
            case 3:
                if (D_8009CFED == 1) {
                    switch (o->state) {
                    case 0:
                        if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x20) < 0x41 &&
                            (unsigned short)(o->y.p.whole - D_1F80016Eu + 0x30) < 0x61) o->state++;
                        break;
                    case 1:
                        o->d88 = (o->d88 + 0x10) & 0xfff;
                        if (o->d88 >= 0x400) o->state++;
                        break;
                    case 2:
                        if (o->visible == 0) {
                            o->d88 = 0;
                            o->state = 0;
                        }
                        break;
                    }
                }
                break;
            case 4:
                switch (o->state) {
                case 0:
                    if (D_800A603C == 0x300) o->state++;
                    break;
                case 1:
                    o->d88 = (o->d88 + 0x20) & 0xfff;
                    if (o->d88 >= 0x600) o->state++;
                    break;
                case 2:
                    o->state++;
                    D_800A603Cb = 5;
                    D_800A603D = 2;
                    D_800A603E = 0;
                    D_800A603F = 0;
                    break;
                }
                break;
            case 5:
                switch (o->state) {
                case 0:
                    o->state = ((TObj *)o->d90)->state;
                    break;
                case 1:
                    o->d88 = (o->d88 - 0x20) & 0xfff;
                    if (o->d88 < 0xa01) o->state++;
                    break;
                case 2:
                    break;
                }
                break;
            case 6:
                if (D_8009D13F) {
                    switch (o->state) {
                    case 0:
                        if ((unsigned short)(o->h->p.whole - D_1F80016A + 0x20) < 0x41) {
                            o->d88 = 0;
                            o->state = 1;
                            if (D_8009C93A[0] == 0) {
                                o->state = 3;
                                o->d88 = 0xa00;
                            }
                        }
                        break;
                    case 1:
                        o->d88 = (o->d88 - 0x20) & 0xfff;
                        if (o->d88 < 0xa01) o->state++;
                        break;
                    case 2:
                        break;
                    case 3:
                        o->d88 = (o->d88 + 0x20) & 0xfff;
                        if (o->d88 == 0) o->state = 0;
                        break;
                    }
                }
                break;
            case 7:
                switch (o->state) {
                case 0:
                    if ((o->state = ((TObj *)o->d90)->state) == 3) o->d88 = 0x600;
                    break;
                case 1:
                    o->d88 = (o->d88 + 0x20) & 0xfff;
                    if (o->d88 >= 0x600) o->state++;
                    break;
                case 2:
                    break;
                case 3:
                    o->d88 = (o->d88 - 0x20) & 0xfff;
                    if (o->d88 == 0) o->state = 0;
                    break;
                }
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
