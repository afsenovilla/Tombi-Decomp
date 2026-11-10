// VERSION SLPM_80165 slpm80165 8003A9F8 172
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only reward handler (vitality max up): D_8009A821 + 1 up to 0x10, message func_8002DC34(0x15, 3), sound 10,
// and the new value into D_800A5850 / D_800A5852 / D_8009A820; then the object's flag bit (unless +0x0C bit 7) and
// the next object step.
#include "TOBJ.H"
extern unsigned char D_8009A821, D_8009A820;
extern short D_800A5850, D_800A5852;
extern void func_8002DC34(int, int);
extern void func_8001E3D8(int);
extern void func_800211B0(int);

void func_8003A9F8(TObj *o)
{
    unsigned char *p = &D_8009A821;
    if (*p < 0x10) {
        (*p)++;
        func_8002DC34(0x15, 3);
        func_8001E3D8(10);
        D_8009A820 = D_800A5852 = D_800A5850 = *p;
    }
    if (!(o->b0c & 0x80))
        func_800211B0(o->b6b);
    o->b04++;
}
#endif
