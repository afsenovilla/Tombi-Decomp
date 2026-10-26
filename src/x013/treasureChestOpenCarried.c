// FUNC 80112204 796 X013
// MATCHING 80112204 796
#include "player.h"
#include "TOBJ.H"
extern short *D_800A6078;
static char D_80115AFA[314] = {
    3, 1, -0x7f, 0x21, 0, 0, 3, 0x1f, 0, 0, 0, 0, 3, 0x1f, 0, 0,
    0, 0, 3, 0x2b, 0, 0, 0, 0, 3, 0x16, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, -1, 2, 0, 0, 0, 0, -1, 3, 0, 0, 0, 0,
    3, 0x1e, 0, 6, 0, 0, 3, 0x1f, 0, 0, 0, 0, -2, 1, 0, 0,
    0, 0, -2, 2, 0, 0, 0, 0, -1, 0xb, 0, 0, 0, 0, -1, 6,
    0, 3, 0, 0, 3, 0x26, 0, 0, 0, 0, -1, 4, 0, 0x20, 0, 0,
    3, 0x1e, 0, 0x21, 0, 0, 3, 0x10, 0, 0x22, 0, 0, -1, 4, 0, 0x23,
    0, 0, 3, 0x10, 0, 0x24, 0, 0, 3, 0x10, 0, 0x25, 0, 0, -1, 5,
    0, 0x26, 0, 0, 3, 0x4d, 0, 0x27, 0, 0, 3, 0x1f, 0, 0x28, 0, 0,
    -1, 5, 0, 0x29, 0, 0, 3, 0x10, 0, 0x2a, 0, 0, 3, 0x7d, 0, 0x20,
    0, 0, 3, 0x1d, 0, 0x21, 0, 0, 3, 0x4d, 0, 0x22, 0, 0, 3, 0x1f,
    0, 0x23, 0, 0, 3, 2, 7, 0x20, 0, 0, 3, 0x2a, 0, 0x21, 0, 0,
    3, 2, 6, 0x20, 0, 0, 3, 0x4d, 0, 0x21, 0, 0, 3, 7, 0, 0x22,
    0, 0, 3, 7, 0, 0x23, 0, 0, 3, 0x1f, 0, 0x24, 0, 0, -1, 5,
    0, 0x25, 0, 0, 3, 0x27, 0, 0x26, 0, 0, 3, -0x6c, 0, 0x27, 0, 0,
    3, 0x28, 0, 0, 0, 0, 3, 0x1d, 0, 0x23, 0, 0, 3, -0x6f, 0, 0x20,
    0, 0, 3, 7, 0, 0x24, 0, 0, 3, 0x10, 0, 0x25, 0, 0, 3, 7,
    0, 0x26, 0, 0, -1, 5, 0, 0x27, 0, 0, 3, 7, 0, 0x28, 0, 0,
    3, 0x10, 0, 0x29, 0, 0, 3, 0x10, 0, 0x2a, 0, 0, 3, 7, 0, 0x20,
    0, 0, 3, 0x4d, 0, 0x21,
};
extern int GAME[];
extern unsigned short D_8009C960h;
extern int *D_80115A08[];
extern int *D_80115948[];
extern int ObjCullRegister(TObj *o);
extern void setSectionFlagBit(int n);
extern void spawnItemFromEntry(char *a, Fix16 *v, int b, int c, TObj *o);
extern void AnimLoadDuration(TObj *o);
extern void despawnObjectOffscreen(TObj *o);
extern void treasureChestTossMove(TObj *o);
extern void spawnBreakPieces(TObj *o, int x, int y, int z);

void treasureChestOpenCarried(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    short dx, dy;
    Fix16 v[3];
    dx = p->h->p.whole - o->wb4;
    dy = p->y.p.whole - o->wb6;
    o->h->p.whole += dx;
    o->y.p.whole += dy;
    o->wb4 = p->h->p.whole;
    o->wb6 = p->y.p.whole;
    ObjCullRegister(o);
    switch (o->step) {
    case 0:
        D_800A6078[1] += dx;
        ((unsigned short*)&PLAYER.obj.y.p.whole)[0] += dy;
        break;
    case 1:
        setSectionFlagBit(o->b6b);
        v[0].p.whole = o->h->p.whole;
        v[1].p.whole = o->y.p.whole;
        v[2].p.whole = o->d->p.whole;
        spawnItemFromEntry(D_80115AFA, v, -0x40, -0x400, o);
        spawnItemFromEntry(D_80115AFA, v, 0x40, -0x400, o);
        o->step = 3;
        o->timer = 10;
        o->wac = 1;
        if (GAME[0] == 0x30009)
            o->anim = (void *)D_80115A08[o->b0c & 0x7f][1];
        else
            o->anim = (void *)D_80115948[D_8009C960h * 4 + (o->b0c & 0x7f)][1];
        AnimLoadDuration(o);
        o->b69 = 0;
        o->velX = 0x80;
        o->velY = 0x100;
        o->w78 = ((o->d38 + 0x800) & 0xfff) >> 4;
        if (o->w78 > 0x80) o->w78 = 0x80;
        o->d8c = o->w78 + 0x80;
        break;
    case 2:
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        break;
    case 3:
        if (--o->timer == -1) {
            o->active = 3;
            o->step++;
        }
    case 4:
        if (o->visible == 0) {
            o->b04 = 3;
            break;
        }
        despawnObjectOffscreen(o);
        treasureChestTossMove(o);
        if (o->y.p.whole < -0xae) break;
        o->timer = 10;
        o->active = 2;
        o->step = 7;
        break;
    case 5:
        spawnBreakPieces(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        setSectionFlagBit(o->b6b);
        o->b04++;
        break;
    case 7:
        spawnBreakPieces(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        o->b04++;
        break;
    }
}
