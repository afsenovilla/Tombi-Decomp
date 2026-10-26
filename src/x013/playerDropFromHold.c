// FUNC 800f6d80 168 X013
// MATCHING 800f6d80 168
#include "TOBJ.H"
extern unsigned char *PLAYER_EXT;
extern unsigned char *GRABBED_OBJECT;
extern unsigned char D_801152E8[];
void playerDropFromHold(TObj *o)
{
    int v;
    int y;
    int c;
    PLAYER_EXT[9] = 0;
    v = 0x10;
    *(unsigned char *)&o->wac = 1;
    o->b9e = 0;
    o->wb2 = 0;
    o->velH = 0;
    o->velV = 0;
    o->d30 = 0;
    o->d34 = 0;
    o->velX = 0;
    o->velY = 0;
    o->timer = 10;
    o->d84 = 0;
    if (o->animFrame & 1) {
        v = 0xf0;
    }
    o->d88 = v;
    *(volatile int *)&o->d8c = D_801152E8[o->wb0];
    c = o->animFrame & 1;
    /* the parameter register is reused for the hitbox pointer */
    o = (TObj *)((volatile TObj *)o)->h;
    y = ((Fix16 *)o)->p.whole;
    if (c) {
        y += 14;
    } else {
        y -= 14;
    }
    ((Fix16 *)o)->p.whole = y;
    GRABBED_OBJECT[0x69] = 0;
}
