// FUNC 8010f254 212 X000
// wip: falta `move a1,a0` inicial, `move a3,zero` muerto antes del switch y `move a2,zero` + `addu at,at,a2`
// (indice en registro con valor 0) tras la union; parece cuerpo de un inline con parametros constantes.
#include "TOBJ.H"
extern unsigned char *DAT_8009c330;
extern unsigned short DAT_80115466[];
extern unsigned short DAT_80115468;

static __inline__ void clampv(TObj *o, int i)
{
    unsigned short lim = DAT_80115466[i];
    if ((short)lim < o->velY) { o->velY = lim; lim = DAT_80115466[i]; }
    if (o->velY < -(short)lim) o->velY = -lim;
}

void FUN_8010f254(TObj *o)
{
    int k;
    short a = o->wb2;
    if (a < 0) a = -a;
    k = 0;
    switch (*(unsigned short *)(DAT_8009c330 + 0x20)) {
    case 0 ... 12:
        o->velY = o->velY + DAT_80115468;
        break;
    default:
        o->velY = o->velY - 8 + (DAT_80115468 - (a >> 7));
        break;
    }
    clampv(o, k);
}
