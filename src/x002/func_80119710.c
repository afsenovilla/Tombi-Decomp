// FUNC 80119710 120 X002
// MATCHING 80119710 120
#include "TOBJ.H"

extern unsigned int D_8009C96C, D_8009C98C;
extern unsigned char D_8009CDCB, D_8009CFE4;
extern void ObjSpawnType0(short, short, void *);

void func_80119710(void)
{
    Fix16 v[3];

    if (D_8009C96C >= D_8009C98C && D_8009CDCB != 0 && D_8009CFE4 == 0) {
        v[0].p.whole = 0xb0;
        v[1].p.whole = -0x38;
        v[2].p.whole = 0;
        ObjSpawnType0(0x5f, 0, v);
    }
}
