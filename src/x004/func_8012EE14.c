// FUNC 8012ee14 640 X004
// MATCHING 8012ee14 640
#include "player.h"
#include "TOBJ.H"
typedef struct { unsigned short w0, w2, w4, w6, w8, wa, wc; } E;
extern E D_8013145C[];
extern unsigned char D_8009CF24[];
extern unsigned char D_8009C940, D_8009C941;
extern unsigned char D_8009CF29;
extern unsigned char D_8009C93E[], D_8009C93F[], D_8009C942[];
extern unsigned short D_800A604E[];
extern TObj *allocObjectLayer2(void);
extern void FUN_80026e0c(int, int);

void func_8012EE14(TObj *o)
{
    switch (o->state) {
    case 0: {
        int c;
        int i;
        E *p;
        TObj *n;

        if (D_8009C940 == 0) break;
        c = D_8009C941;
        i = c - 0x2e;
        if (D_8009CF24[i]) break;
        c &= 0xff;
        if (c >= 0x33) break;
        if (c < 0x2e) break;
        p = &D_8013145C[i];
        n = allocObjectLayer2();
        if (n) {
            n->active = 4;
            n->type = 0x32;
            n->animFrame = p->w0;
            n->subtype = p->w2;
            n->b0c = p->w4;
            n->b6b = p->wc;
            n->velX = p->w6;
            n->velY = p->w8;
            n->h->p.whole = (*(unsigned short **)&PLAYER.obj.h)[1];
            n->y.p.whole = D_800A604E[0];
            n->b.p.whole = p->wa;
            n->velV = 3;
            n->velH = 3;
        }
        D_8009CF24[i] = 1;
        FUN_80026e0c(i + 0x2e, 1);
        ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        D_8009CF29 = 1;
        break;
    }
    case 1:
        if (D_8009CF29 == 0) o->state++;
        break;
    case 2:
        D_8009C93E[0] = 1;
        o->w08 = 0x3c;
        o->state++;
        break;
    case 3:
        if (--o->w08 <= 0) o->state = 15;
        break;
    case 15:
        ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
        ((unsigned char*)&PLAYER.obj.step)[0] = 0;
        ((unsigned char*)&PLAYER.obj.state)[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        D_8009C93E[0] = 0;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        o->state = 0;
        o->step++;
        break;
    }
}
