// FUNC 80124d10 428 X000
// MATCHING 80124d10 428
#include "TOBJ.H"
extern int FUN_80124bbc(TObj *, unsigned char *);
extern int FUN_80124a84(TObj *, unsigned char *);

void FUN_80124d10(TObj *o, unsigned char *b)
{
    unsigned char c;
    if ((unsigned short)(o->d->p.whole - (*(Fix16 **)(b + 0x44))->p.whole + 0x2d) >= 0x5b)
        return;
    switch (b[0xc]) {
    case 0:
    case 1:
    case 2:
        FUN_80124bbc(o, b);
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        if (FUN_80124bbc(o, b) != 0) {
            if (o->step != 0x17) {
                o->step = 0x17;
                o->state = 0;
            }
            o->bbe = 5;
            c = b[0xc];
            o->b69 = 1;
            o->wb0 = c - 0xc;
            b[0x69] = 1;
        }
        break;
    case 8:
    case 9:
        if (FUN_80124a84(o, b) != 0) {
            if (o->b04 != 2 && o->step != 0x17) {
                o->step = 0x17;
                o->state = 0;
            }
            o->bbe = 5;
            o->wb0 = b[0xc] - 0xc;
            b[0x69] = 1;
        }
        break;
    case 10:
    case 11:
        if (FUN_80124a84(o, b) == 0)
            return;
        o->bbe = 5;
        o->wb0 = b[0xc] - 0xc;
        b[0x69] = 1;
        *((unsigned char *)o + 0xa1) = 1;
        break;
    case 12:
        if (FUN_80124a84(o, b) == 0)
            return;
        b[0x69] = 1;
        o->bbe = 5;
        o->wb0 = -1;
        *((unsigned char *)o + 0xa1) = 1;
        break;
    }
}
