// FUNC 8011de64 512 X009
// MATCHING 8011de64 512
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    char p0[2]; short w2; char p4[4]; unsigned char b8, b9; char pa[2]; short wc, we;
    char p10[0x28 - 0x10]; unsigned short w28, w2a;
} G;
extern G *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern unsigned short D_1F8001FC;
extern int AnimAdvance(TObj *o);
extern void playerReleaseBittenAndClearFlags(TObj *o);
extern void SfxPlay3(int, int);
extern void PlayerSetAnimIfChanged(TObj *o, int);
extern void playerCheckPerchLanding(TObj *o);

void playerStep26Area9(TObj *o)
{
    G *g;

    o->h->p.whole += GRABBED_OBJECT->velX;
    o->y.p.whole += GRABBED_OBJECT->velY;
    switch (o->state) {
    case 0:
        g = PLAYER_EXT;
        o->wb6 = 0;
        g->b8 = 0;
        g->we = 0;
        g->w2 = 0;
        PLAYER_EXT->b9 = 0;
        PLAYER_EXT->wc = 0;
        PLAYER_EXT->w28 = 0xffff;
        PLAYER_EXT->w2a = 0xffff;
        o->velH = 0;
        o->velV = 0;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->h->p.whole = o->h->p.whole;
        o->y.p.whole += 4;
        playerReleaseBittenAndClearFlags(o);
        SfxPlay3(4, 0x7f);
        PlayerSetAnimIfChanged(o, 7);
        o->state++;
    case 1:
        AnimAdvance(o);
        if (D_1F8001FC & SPAD.s3c6) {
            PLAYER_EXT->b9 = 0;
            o->b04 = 1;
            o->step = 0x27;
            o->state = 0;
            o->y.p.whole += 0x10;
        }
        if (D_1F8001FC & 0x10) {
            PLAYER_EXT->b9 = 0;
            o->b04 = 1;
            o->step = 0x27;
            o->state = 0;
            o->y.p.whole += 0x10;
        }
        if (D_1F8001FC & 0x40) {
            PLAYER_EXT->b9 = 0;
            o->b9e = 6;
            o->step = 0x25;
            o->state = 0;
            o->y.p.whole += 9;
        }
        break;
    }
    playerCheckPerchLanding(o);
}
