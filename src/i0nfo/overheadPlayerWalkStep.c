// FUNC 800e9024 356 I0NFO
// MATCHING 800e9024 356
#include "TOBJ.H"
extern void func_8001FBAC(TObj *o, int a, int b);
extern void func_8001FCE4(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void overheadPlayerSetMoveAnim(TObj *o);
extern void overheadCollideMapTile(TObj *o, int x, int y, int z);
extern unsigned char GVARS_PLAYER_LANDED;
extern volatile unsigned short PAD_PRESSED[];

void overheadPlayerWalkStep(TObj *o)
{
    if (GVARS_PLAYER_LANDED != 0) {
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            o->state++;
        case 1:
            overheadPlayerSetMoveAnim(o);
            func_8001FEC0(o);
            if (o->animFrame & 8) {
                o->wb2 = 0;
                o->wb6 = 0;
            } else {
                o->wb6 = (o->animFrame & 7) << 5;
                if (PAD_PRESSED[0] & *(unsigned short *)0x1F8003C4) o->wb2 = 0x280;
                else o->wb2 = 0x180;
            }
            func_8001FBAC(o, o->wb6, o->wb2);
            func_8001FCE4(o);
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
        if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C4) {
            *(unsigned char *)&o->wac = 1;
            o->animFrame |= 8;
        }
    }
}
