// VERSION SLPS_01144 info2 800EBCAC 364
// JP version of PAL i0nfo2 800ea538 (topdownPlayerWalkStep.c)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern void func_80020B24(TObj *o, int a, int b);
extern void func_80020C5C(TObj *o);
extern void func_80020E38(TObj *o);
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void overheadCollideMapTile(TObj *o, int x, int y, int z);
extern unsigned char D_8009E4E2;
extern volatile unsigned short D_8009F200[];

void func_800EBCAC(TObj *o)
{
    if (D_8009E4E2 != 0) {
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            o->state++;
        case 1:
            overheadPlayerSetMoveAnim(o);
            func_80020E38(o);
            if (o->animFrame & 8) {
                o->wb2 = 0;
                o->wb6 = 0;
            } else {
                o->wb6 = (o->animFrame & 7) << 5;
                if (D_8009F200[0] & *(unsigned short *)0x1F8003C4) o->wb2 = 0x280;
                else o->wb2 = 0x180;
            }
            func_80020B24(o, o->wb6, o->wb2);
            func_80020C5C(o);
            overheadCollideMapTile(o, o->h->p.whole, o->y.p.whole, (short)o->animFrame);
            if (o->animFrame & 8) {
                o->step = 0;
                o->state = 0;
            }
            break;
        }
        if (*(unsigned char *)&o->wac >= 2) {
            *(unsigned char *)&o->wac = 0;
            o->step = 2;
            o->state = 0;
            return;
        }
        if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C8) {
            *(unsigned char *)&o->wac = 1;
            o->animFrame |= 8;
            return;
        }
        *(unsigned char *)&o->wac = 0;
    }
}
#endif
