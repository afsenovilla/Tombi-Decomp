// FUNC 8004fed4 496 MAIN0
#include "TOBJ.H"
extern unsigned short DAT_8009c960, DAT_8009c962;
extern unsigned short DAT_1f8001c8, DAT_1f8000f6, DAT_1f8000ee;
extern int DAT_1f8002b4;

unsigned char FUN_8004fed4(TObj *o, short d)
{
    switch (DAT_8009c960) {
    case 0:
        if (DAT_8009c962 == 3)
            return o->b0a;
        break;
    case 2:
        if (DAT_8009c962 != 0 && DAT_8009c962 != 3)
            return o->b0a;
        break;
    case 6:
        if ((unsigned)(DAT_8009c962 - 1) < 2)
            return o->b0a;
        break;
    case 5:
    case 8:
        if (DAT_8009c962 == 1 || DAT_8009c962 == 3)
            return o->b0a;
        break;
    case 9:
        if (DAT_8009c962 != 0 && DAT_8009c962 != 6)
            return o->b0a;
        break;
    case 10:
        if (DAT_8009c962 == 8)
            return o->b0a;
        break;
    case 13:
    case 19:
        if (DAT_8009c962 == 1)
            return o->b0a;
        break;
    case 18:
        if (DAT_8009c962 == 2)
            return o->b0a;
        break;
    case 11:
    case 16:
    case 17:
        return o->b0a;
    }
    if (o->b0a >= 4)
        return o->b0a;
    switch (DAT_1f8001c8 & 1) {
    case 0:
        d = DAT_1f8000f6 - o->b.p.whole;
        break;
    case 1:
        d = o->a.p.whole - DAT_1f8000ee;
        break;
    }
    if (d == 0)
        return o->b0a;
    if (o->b0a & 1)
        return o->b0a;
    if (DAT_8009c960 == 4)
        DAT_1f8002b4 = d * 5 + 0x1000;
    else
        DAT_1f8002b4 = d * 7 + 0x1000;
    return o->b0a | 1;
}
