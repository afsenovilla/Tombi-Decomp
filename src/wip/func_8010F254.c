// FUNC 8010f254 212 X000
#include "TOBJ.H"
extern unsigned char *D_8009C330;
extern unsigned short D_80115466[];
extern unsigned short D_80115468;
extern int abs(int);

static __inline__ void clampv(TObj *o, int i)
{
    short lim = *(short *)((char *)D_80115466 + i);
    if (lim < o->velY) { o->velY = lim; lim = *(short *)((char *)D_80115466 + i); }
    if (o->velY < -lim) o->velY = -lim;
}


/* score 17: (short)(velY-8) + pad[16] fixed most; left: game has dead a3=0 before switch and a separate a2=0 index set after the merge (ours reuses k); tried j copies, short/char params */
void func_8010F254(TObj *o)
{
    char pad[16];
    int k;
    short a = abs(o->wb2);
    k = 0;
    switch (*(unsigned short *)(D_8009C330 + 0x20)) {
    case 0 ... 12:
        o->velY = o->velY + D_80115468;
        break;
    default:
        o->velY = (short)(o->velY - 8) + (D_80115468 - (a >> 7));
        break;
    }
    clampv(o, k);
}
