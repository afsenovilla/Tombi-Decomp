// FUNC 801222c4 332 X001
// MATCHING 801222c4 332
#include "gv2.h"
#include "spad.h"
#include "TOBJ.H"
typedef struct L { unsigned char active; char p[0x94 - 1]; struct L *next; } L;
extern TObj *PLAYER_EXT;
extern L *GRABBED_OBJECT;
extern unsigned int D_8009C984[];
extern volatile unsigned short PAD_PRESSED[];

void playerClingJumpRelease(TObj *o)
{
    L *q, *n;

    if (!(SPAD.s1fc & SPAD.s3c6)) return;
    GV2.bb0 = 0;
    ((unsigned char *)&o->wa8)[1] = 0;
    if (GV2.bb2 - 5 < 3) GRABBED_OBJECT->active = 1;
    PLAYER_EXT->b04 = 0;
    o->b9c = 1;
    o->timer = 10;
    o->velH = 0;
    o->velV = 0;
    o->velX = 0;
    o->velY = 0;
    n = GRABBED_OBJECT->next;
    if (n != 0) {
        q = n;
        if (q->next == 0) {
            q->active = 1;
        } else {
        loop:
            q->active = 3;
            q = q->next;
            if (q->next != 0) goto loop;
            q->active = 1;
        }
    }
    o->h->p.whole += (o->animFrame & 1) ? 0x10 : -0x10;
    if ((D_8009C984[0] & 0x40) && (PAD_PRESSED[0] & SPAD.s3c4)) o->ba7 = 1;
    o->step = 2;
    o->state = 0;
}
