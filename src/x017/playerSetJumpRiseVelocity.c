// FUNC 8010f328 216 X017
// MATCHING 8010f328 216
#include "TOBJ.H"
extern unsigned char DAT_8009d2b3;
extern unsigned char DAT_8009c990;
extern unsigned char DAT_8009cf06;
extern unsigned char DAT_8009d006;
extern TObj *PLAYER_EXT;
extern unsigned short DAT_80115468[][10];

void playerSetJumpRiseVelocity(TObj *o)
{
    int b = ((unsigned char *)o)[0xc1];
    int u = DAT_8009d2b3 + b * 4;
    if ((DAT_8009c990 & 3) != 0)
        u = (unsigned char)b << 2 | 3;
    if (DAT_8009cf06 != 0)
        u = 8;
    if (DAT_8009d006 != 0)
        u = 8;
    switch ((unsigned short)PLAYER_EXT->timer) {
    case 0 ... 12:
        o->velY = DAT_80115468[u][2];
        break;
    case 13:
        o->velY = DAT_80115468[u][1];
        break;
    }
}
