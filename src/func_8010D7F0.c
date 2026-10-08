// FUNC 8010d7f0 152 X000
// MATCHING 8010d7f0 152
#include "TOBJ.H"
void func_8010D7F0(TObj *o)
{
    switch (o->w76 & 7) {
    case 0: o->w7a = 0; break;
    case 1: o->w7a = 0x20; break;
    case 2: o->w7a = 0x40; break;
    case 3: o->w7a = 0x60; break;
    case 4: o->w7a = 0x80; break;
    case 5: o->w7a = 0xa0; break;
    case 6: o->w7a = 0xc0; break;
    case 7: o->w7a = 0xe0; break;
    }
    if (o->w76 & 8) {
        o->w74 = 0;
    } else {
        o->w74 = 0x200;
    }
}
