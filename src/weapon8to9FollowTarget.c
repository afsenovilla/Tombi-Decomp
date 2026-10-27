// FUNC 80036cb8 516 MAIN0
// MATCHING 80036cb8 516
#include "TOBJ.H"
typedef struct { char p0[0xc]; unsigned char bc; char p1; short we; char p2[0x20]; short w30; short w32; } G338;
extern G338 *WEAPON_STATE;
extern unsigned char *PLAYER_EXT;
extern int DAT_800a6068[];
extern int DAT_800a60c0;
extern unsigned char PLAYER_SUBSTATE, DAT_800a60d5, DAT_800a60fe;
extern int DAT_8009c934;

void weapon8to9FollowTarget(TObj *o)
{
    TObj *t = *(TObj **)((char *)o + 0x90);
    short dx, dy;
    unsigned short b;
    G338 *g;
    switch (o->state) {
    case 0:
        WEAPON_STATE->bc = 1;
        WEAPON_STATE->w30 = 0;
        WEAPON_STATE->w32 = 0;
        o->state++;
        if (t != 0) {
            o->velX = t->h->p.whole;
            if ((t->category & 0x7f) == 2)
                o->velY = t->y.p.whole + 8;
            else
                o->velY = t->y.p.whole;
        }
        break;
    case 1:
        if (t != 0) {
            dx = t->h->p.whole - o->velX;
            if ((t->category & 0x7f) == 2) {
                b = o->velY - 8;
                dy = t->y.p.whole - b;
            } else
                dy = t->y.p.whole - o->velY;
            o->h->p.whole += dx;
            o->y.p.whole += dy;
            DAT_800a6068[0] += dx;
            DAT_800a6068[1] += dy;
            o->velX = t->h->p.whole;
            if ((t->category & 0x7f) == 2)
                o->velY = t->y.p.whole + 8;
            else
                o->velY = t->y.p.whole;
        }
        o->d8c = DAT_800a60c0;
        break;
    }
    if (PLAYER_SUBSTATE != 0x32) {
        g = WEAPON_STATE;
        g->bc = 0;
        g->w32 = 0;
        g->we = 0;
        DAT_800a60d5 = 0;
        *PLAYER_EXT = 0;
        DAT_800a60fe = 0;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        DAT_8009c934 = 0;
    }
}
