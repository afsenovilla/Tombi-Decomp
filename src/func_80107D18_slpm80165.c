// VERSION SLPM_80165 x00 80107D18 188
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only player step: sets D_8009A7E8 to 1 (sound 0x2A the first time), waits 180 frames, then clears
// D_8009B161 and sets D_8009A7E8 to 2. The empty case 2 gives the compare tree of the retail switch.
#include "TOBJ.H"
extern unsigned char D_8009A7E8, D_8009B161;
extern void func_8001E3D8(int);

void func_80107D18(TObj *o)
{
    switch (o->state) {
    case 0:
        {
            unsigned char *p = &D_8009A7E8;
            if (*p != 1) {
                *p = 1;
                func_8001E3D8(0x2a);
            }
        }
        o->timer = 0xb4;
        *(signed char *)&o->b0f = -8;
        o->state++;
        break;
    case 1:
        if (--o->timer == 0) {
            D_8009B161 = 0;
            D_8009A7E8 = 2;
            o->state++;
        }
        break;
    case 2:
        break;
    }
}
#endif
