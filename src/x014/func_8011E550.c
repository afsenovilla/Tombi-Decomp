// FUNC 8011e550 540 X014
// MATCHING 8011e550 540
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern signed char D_8009D2B0;
extern unsigned char D_80125CA8[];
extern unsigned char D_80125CB8[];
extern unsigned char D_80125CC8[];
extern void (*D_80125CD8[])(TObj *);
extern void func_8011E1F0(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern int angleFromDelta(int, int);

void func_8011E550(TObj *o)
{
    unsigned char *t;
    int x;

    if (GVARS.b38 != 0 || D_8009D2B0 == 3) {
        o->state = 0;
        o->step++;
    } else if (((unsigned short*)&GAME.selectedSection)[0] == 7) {
        func_8011E1F0(o);
    } else switch (o->state) {
    case 0:
        ObjSetFacingToPlayer(o);
        if ((unsigned short)o->wb4 < 0x708) {
            t = D_80125CC8;
        } else {
            t = D_80125CA8;
            if ((unsigned short)o->wb4 < 0xe10) {
                t = D_80125CB8;
            }
        }
        if (t[Rand() & 0xf] == 0) {
            o->state = 0;
            o->step++;
            break;
        }
        o->d38 = angleFromDelta((short)((*(unsigned short*)((char*)&SPAD.i168 + 2)) - o->h->p.whole), (short)((*(unsigned short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole)) & 0xff;
        o->wb6 = ((unsigned short*)&GAME.selectedSection)[0];
        x = ((o->d38 + 8) & 0xff) >> 4;
        o->wb8 = x;
        o->animFrame = ~((x - 4) >> 3) & 1;
        o->state++;
    case 1:
        if (D_8009D2B0 != 3) {
            D_80125CD8[((unsigned short*)&GAME.selectedSection)[0]](o);
        }
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer == -1) {
            o->step = 0;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
