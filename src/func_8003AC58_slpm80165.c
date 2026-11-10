// VERSION SLPM_80165 slpm80165 8003AC58 140
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only reward handler: raises D_8009A821 to at least 0x10 (with the message D_80011DD8 and sound 10), sets
// the object's flag bit unless +0x0C bit 7 is set, and advances the object.
#include "TOBJ.H"
extern unsigned char D_8009A821;
extern char D_80011DD8[];
extern char *D_800B0A9C;
extern void func_8001E3D8(int);
extern void func_800211B0(int);

void func_8003AC58(TObj *o)
{
    unsigned char *p = &D_8009A821;
    if (*p < 0x10) {
        *p = 0x10;
        D_800B0A9C = D_80011DD8;
        func_8001E3D8(10);
    }
    if (!(o->b0c & 0x80))
        func_800211B0(o->b6b);
    o->b04++;
}
#endif
