// FUNC 801113bc 160 X011
// MATCHING 801113bc 160
#include "TOBJ.H"
extern unsigned short PAD_INFO;
extern unsigned char DAT_8009d619;
extern unsigned short PAD_PRESSED;

void adjustVelYFromPadDown(TObj *o)
{
    short v;
    v = 0x20;
    if (PAD_INFO == 7) { v = 0x20; v = v >> (3 - DAT_8009d619); }
    if (*(volatile unsigned short *)&PAD_PRESSED & 0x40) o->velY = o->velY + v;
    else o->velY = o->velY - v;
    if (o->velY > 0x300) o->velY = 0x300;
    if (o->velY < 0xc0) o->velY = 0xc0;
}
