// FUNC 8012eb78 296 X004
// MATCHING 8012eb78 296
#include "player.h"
#include "TOBJ.H"
typedef struct { unsigned short w0, w2, w4, w6, w8, wa, wc; } E;
extern E D_8013145C[];
extern unsigned short PLAYER_Y[];
extern TObj *allocObjectLayer2(void);

void spawnBigKeyPiece(short i, short flag)
{
    E *p = &D_8013145C[i];
    TObj *n = allocObjectLayer2();
    if (n == 0) return;
    n->active = 4;
    n->type = 0x32;
    n->animFrame = p->w0;
    n->subtype = p->w2;
    n->b0c = p->w4;
    n->b6b = p->wc;
    if (flag) {
        n->h->p.whole = p->w6;
        n->y.p.whole = p->w8;
        n->b.p.whole = p->wa;
        n->velV = 0;
        n->velH = 0;
    } else {
        n->velX = p->w6;
        n->velY = p->w8;
        n->h->p.whole = (*(unsigned short **)&PLAYER.obj.h)[1];
        n->y.p.whole = PLAYER_Y[0];
        n->b.p.whole = p->wa;
        n->velV = 3;
        n->velH = 3;
    }
}
