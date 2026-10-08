// FUNC 8010beb8 184 X009
// MATCHING 8010beb8 184
#include "TOBJ.H"
extern unsigned char D_8009D2B0;
extern int D_8009C984;
extern unsigned short D_8009D670;
extern void func_8010BD08();

short func_8010BEB8(TObj *o)
{
    short r = 0;

    if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
        D_8009D2B0 = 0;
        func_8010BD08();
        if (D_8009C984 & 0x40) {
            if (*(volatile unsigned short *)&D_8009D670 & *(unsigned short *)0x1F8003C4) {
                o->ba7 = 1;
            }
        }
        r++;
        o->b9c = 1;
        o->step = 2;
        o->state = 0;
    }
    return r;
}
