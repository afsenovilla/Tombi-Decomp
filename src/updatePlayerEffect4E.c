// FUNC 8002fe78 168 MAIN0
// MATCHING 8002fe78 168
#include "TOBJ.H"
extern unsigned char PLAYER;
extern void playerEffect4EInit(TObj *);
extern void playerEffect4EDispatchB(TObj *);
extern void ObjFree(TObj *);
void updatePlayerEffect4E(TObj *o)
{
    switch (o->b04) {
    case 0:
        playerEffect4EInit(o);
        break;
    case 1:
        if (PLAYER != 5)
            playerEffect4EDispatchB(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
