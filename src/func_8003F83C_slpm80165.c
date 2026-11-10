// VERSION SLPM_80165 slpm80165 8003F83C 176
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only: for an object with +0x0A below 4, its distance to the camera along the scroll axis (0x1F8001C8 bit 0:
// x against 0x1F8000EE, else depth against 0x1F8000F6); a non-zero distance on an even +0x0A sets 0x1F8002B4 to
// 7 * d + 0x1000 and returns +0x0A with bit 0 set. Returns +0x0A.
#include "TOBJ.H"
extern unsigned short D_1F8001C8, D_1F8000EE, D_1F8000F6;
extern int D_1F8002B4;

unsigned char func_8003F83C(TObj *o)
{
    short d;
    if (o->b0a < 4) {
        switch (D_1F8001C8 & 1) {
        case 0:
            d = D_1F8000F6 - o->b.p.whole;
            break;
        case 1:
            d = o->a.p.whole - D_1F8000EE;
            break;
        }
        if (d != 0 && !(o->b0a & 1)) {
            D_1F8002B4 = d * 7 + 0x1000;
            return o->b0a | 1;
        }
    }
    return o->b0a;
}
#endif
