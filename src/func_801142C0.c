// FUNC 801142c0 924 X000
// MATCHING 801142c0 924
#include "TOBJ.H"
#include "gamestate.h"
#include "player.h"

typedef struct { short s[6]; } P6;
extern int D_8009C960_w;
extern unsigned char D_8009CDB1;
extern unsigned char *D_8009C330;
extern void *SCRIPT_OBJECTS[];
int showMessageBoxDirectTimed(int a, int b, P6 *p, int c);
void TileCollideAt(TObj *o, short x, short y);
void AnimAdvance(TObj *o);
void printInfoMessage(int a, int b);

void func_801142C0(TObj *o)
{
    P6 pos;
    unsigned short *p;
    switch (o->state) {
    case 0:
        o->timer = 60;
        o->state++;
    case 1:
        if (--o->timer > 0) break;
        o->timer = 0;
        pos = *(P6 *)&o->a;
        p = &GAME.selectedArea;
        if (*p == 2) pos.s[1] -= 0x50;
        pos.s[3] -= 0x20;
        o->b68 = 0;
        o->b6a = 1;
        if (*p == 2) {
            switch (D_8009CDB1) {
            case 1: o->d90 = showMessageBoxDirectTimed(2, 0, &pos, 0x78); break;
            case 2: o->d90 = showMessageBoxDirectTimed(2, 1, &pos, 0x78); break;
            case 3: o->d90 = showMessageBoxDirectTimed(2, 1, &pos, 0x78); break;
            case 4: o->d90 = showMessageBoxDirectTimed(2, 2, &pos, 0x78); break;
            }
        } else {
            switch (D_8009CDB1) {
            case 1: o->d90 = showMessageBoxDirectTimed(2, 0, &pos, 0x78); break;
            case 2: o->d90 = showMessageBoxDirectTimed(2, 1, &pos, 0x78); break;
            case 3: o->d90 = showMessageBoxDirectTimed(2, 1, &pos, 0x78); break;
            case 4: o->d90 = showMessageBoxDirectTimed(2, 2, &pos, 0x78); break;
            }
        }
        ((unsigned char*)&PLAYER.obj.b04)[0] = 6;
        ((unsigned char*)&PLAYER.obj.b04)[1] = 5;
        ((unsigned char*)&PLAYER.obj.b04)[2] = 0;
        D_8009C330[9] = 10;
        if (D_8009C960_w == 0x10002) {
            (*(short*)&PLAYER.obj.animFrame) = 1;
            PLAYER.obj.velX = 0xc0;
        } else {
            (*(short*)&PLAYER.obj.animFrame) = 0;
            PLAYER.obj.velX = -0xc0;
        }
        PLAYER.obj.b69 = 0;
        o->state++;
        break;
    case 2:
        if (GAME.selectedArea == 1) o->h->p.whole += 4;
        else if (GAME.selectedArea == 2) o->h->p.whole -= 4;
        o->y.raw += 0x80000;
        TileCollideAt(o, o->h->p.whole, o->y.p.whole + o->box3 - o->box2);
        AnimAdvance(o);
        if (PLAYER.obj.b69) {
            if (GAME.selectedArea == 2) {
                if (D_8009CDB1 == 4) printInfoMessage(8, 3);
            } else if (D_8009CDB1 != 0xff) {
                printInfoMessage(D_8009CDB1 + 7, 2);
            }
            o->b6a = 0;
            SCRIPT_OBJECTS[o->b0c] = 0;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.b04)[1] = 0;
            ((unsigned char*)&PLAYER.obj.b04)[2] = 0;
            o->b04 = 3;
        }
        break;
    }
}
