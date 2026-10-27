// FUNC 800e9998 532 I0NFO3
// MATCHING 800e9998 532
#include "TOBJ.H"
extern void func_8001FBAC(TObj *o, int a, int b);
extern void func_8001FCE4(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void func_8005A9A4(int a, int b);
extern void tunnelPlayerSetMoveAnim(TObj *o);
extern void tunnelCollideMapTile(TObj *o, int x, int y, int z);
extern unsigned char GVARS_PLAYER_LANDED;
extern unsigned char D_8009CE34;
extern unsigned char D_8009CE35;
extern volatile unsigned short PAD_PRESSED[];

void tunnelPlayerWalkStep(TObj *o)
{
    if (GVARS_PLAYER_LANDED != 0) {
        *(unsigned char *)&o->wac = 0;
        switch (o->state) {
        case 0:
            o->state++;
        case 1:
            tunnelPlayerSetMoveAnim(o);
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
            if (D_8009CE34 != 0 && D_8009CE35 != 0xFF) {
                if (o->h->p.whole >= 0x546 && o->h->p.whole < 0x5AA && o->y.p.whole >= -0x2FF && o->y.p.whole < -0x2DF) {
                    func_8005A9A4(0x91, 1);
                }
                if (o->h->p.whole >= 0x334 && o->h->p.whole < 0x37A && o->y.p.whole >= -0x190 && o->y.p.whole < -0x170) {
                    func_8005A9A4(0x91, 1);
                }
            }
            tunnelCollideMapTile(o, o->h->p.whole, o->y.p.whole, (short)o->animFrame);
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
