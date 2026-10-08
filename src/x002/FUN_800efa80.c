// FUNC 800efa80 388 X002
// MATCHING 800efa80 388
#include "TOBJ.H"
typedef struct { char pad[0x2c]; unsigned short cur; unsigned short prev; } Cam2c;
extern Cam2c *DAT_8009c330;
extern int DAT_8009c984;
extern unsigned short DAT_8009d670;
extern unsigned short DAT_1f8003c4;
extern void FUN_800ef760(TObj *o, int v);

void FUN_800efa80(TObj *o)
{
    short v;
    if (o->wb2 == 0) {
        DAT_8009c330->cur = 0;
    } else {
        if ((DAT_8009c984 & 0x40) && (*(volatile unsigned short *)&DAT_8009d670 & DAT_1f8003c4) && o->b04 == 1) {
            DAT_8009c330->cur = 0x21;
        } else {
            v = o->wb2;
            if (v < -0x143)
                DAT_8009c330->cur = 3;
            else if (v < -0x84)
                DAT_8009c330->cur = 2;
            else if (v < 0)
                DAT_8009c330->cur = 1;
            else if (v >= 0x144)
                DAT_8009c330->cur = 3;
            else if (v >= 0x85)
                DAT_8009c330->cur = 2;
            else if (v > 0)
                DAT_8009c330->cur = 1;
        }
        if (o->subtype)
            DAT_8009c330->cur = 0x21;
    }
    if (DAT_8009c330->prev != DAT_8009c330->cur) {
        FUN_800ef760(o, DAT_8009c330->cur);
        DAT_8009c330->prev = DAT_8009c330->cur;
    }
}
