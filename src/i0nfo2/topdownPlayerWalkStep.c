// FUNC 800ea538 356 I0NFO2
// MATCHING 800ea538 356
#include "TOBJ.H"
extern void func_8001FBAC(TObj *o, int a, int b);
extern void func_8001FCE4(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void topdownPlayerSetMoveAnim(TObj *o);
extern void topdownCollideMapTile(TObj *o, int x, int y, int z);
extern unsigned char D_8009C93A;
extern volatile unsigned short D_8009D670[];

void topdownPlayerWalkStep(TObj *o)
{
    if (D_8009C93A != 0) {
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            o->state++;
        case 1:
            topdownPlayerSetMoveAnim(o);
            func_8001FEC0(o);
            if (o->animFrame & 8) {
                o->wb2 = 0;
                o->wb6 = 0;
            } else {
                o->wb6 = (o->animFrame & 7) << 5;
                if (D_8009D670[0] & *(unsigned short *)0x1F8003C4) o->wb2 = 0x280;
                else o->wb2 = 0x180;
            }
            func_8001FBAC(o, o->wb6, o->wb2);
            func_8001FCE4(o);
            topdownCollideMapTile(o, o->h->p.whole, o->y.p.whole, (short)o->animFrame);
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
        if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C4) {
            *(unsigned char *)&o->wac = 1;
            o->animFrame |= 8;
        }
    }
}
