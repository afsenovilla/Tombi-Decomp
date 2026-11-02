// FUNC 8002fbc8 208 MAIN0
// MATCHING 8002fbc8 208
// VERSION SLPS_01144 slps01144 80030CDC 156
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern unsigned short TICK_COUNT;
extern void func_80030440(TObj *);
extern void func_80030620(TObj *);
extern void func_80030A68_slps01144(TObj *);
extern void SfxPlay2(int, int);

void func_80030CDC_slps01144(TObj *o)
{
    switch (o->subtype) {
    case 0:
        func_80030440(o);
        break;
    case 1:
        func_80030620(o);
        break;
    case 2:
        func_80030A68_slps01144(o);
        break;
    }
    if ((TICK_COUNT & 3) == 0)
        SfxPlay2(3, 12);
}
#else
void playerEffect4EStepA(unsigned char *o);
void playerEffect4EStepC(unsigned char *o);
void playerEffect4EStepD(unsigned char *o);
void playerEffect4EStepB(unsigned char *o);
void SfxPlay2(int a, int b);
void playerEffect4EDispatchA(unsigned char *o)
{
    switch (o[3]) {
    case 0: playerEffect4EStepA(o); break;
    case 1: playerEffect4EStepC(o); break;
    case 2: playerEffect4EStepD(o); break;
    case 9: playerEffect4EStepB(o); break;
    }
    if (o[3] != 9 && (*(unsigned short *)0x1F8001F8 & 3) == 0) SfxPlay2(3, 0xc);
}
#endif
