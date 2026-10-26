// FUNC 8011b330 1188 X010
// MATCHING 8011b330 1188
#include "gv2.h"
#include "spad.h"
#include "gv6.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

typedef struct {
    TObj o;
    unsigned char bc0, bc1, bc2, bc3;
    char pc4[0xcc - 0xc4];
    unsigned char bcc, bcd, bce;
    char pcf[0xe3 - 0xcf];
    signed char be3;
    TObj *e4;
} S;
typedef struct {
    unsigned char b0, b1, b2, b3, b4, b5, b6, b7, b8;
    char p9[0x20 - 9];
    short w20;
    char p22[0x2c - 0x22];
    unsigned short w2c;
    unsigned short w2e;
} P;

extern P *PLAYER_EXT;
extern TObj *BITTEN_OBJECT;
extern short D_8007A038[];
extern TObj *allocObjectLayer1(void);
extern void playerReleaseBittenAndClearFlags(S *);
extern void ObjSetAnimFromTable(S *);
extern void AnimLoadDuration(S *);
extern void SfxPlay2(int, int);
extern int AnimAdvance(S *);
extern void playerUpdateSpeedFromPad(S *);
extern void playerAirSpinToFall(S *);
extern int ObjCheckHeadCollision(S *);
extern short TileCollideAt(S *, short, short);
extern void spawnPlayerPowerEffect(int);

void playerStep3FArea10(S *s)
{
    TObj *n;
    P *p;
    short v, t;
    int w, k;
    TObj *e;
    unsigned short f;
    Fix16 *h;
    short x;

    switch (s->o.state) {
    case 0:
        e = s->e4;
        BITTEN_OBJECT = e;
        if (*(unsigned char *)&s->o.wac >= 2) {
            if (e->type == 0x3a) {
                n = allocObjectLayer1();
                if (n) {
                    n->active = 1;
                    n->type = 1;
                    n->subtype = 0xc;
                    BITTEN_OBJECT->b04 = 3;
                }
                f = s->o.animFrame & 1;
                n->animFrame = f;
                if (*(volatile unsigned short *)&GV6.s70 & 0x40) n->animFrame = f | 2;
                n->a.p.whole = s->o.a.p.whole;
                n->y.p.whole = s->o.y.p.whole;
                n->b.p.whole = s->o.b.p.whole;
                n->h->p.whole += (s->o.animFrame & 1) ? -0x10 : 0x10;
            } else {
                playerReleaseBittenAndClearFlags(s);
            }
        }
        s->o.b69 = 0;
        s->o.b9e = 0;
        s->bc3 = 0;
        s->o.b69 = 0;
        s->o.b9c = 1;
        *(unsigned char *)&s->o.da0 = 0;
        ((unsigned char *)&s->o.da0)[1] = 0;
        s->o.timer = 0xa;
        s->o.d84 = 0;
        s->o.d88 = 0;
        s->o.d8c = 0;
        s->o.wb0 = 0;
        s->o.wb6 = 0;
        PLAYER_EXT->b8 = 1;
        PLAYER_EXT->w2c = 4;
        PLAYER_EXT->w20 = 0;
        PLAYER_EXT->b5 = 0;
        *(unsigned char *)&s->o.wac = 0;
        ObjSetAnimFromTable(s);
        AnimLoadDuration(s);
        PLAYER_EXT->w2e = PLAYER_EXT->w2c;
        SfxPlay2(2, 4);
        s->o.state++;
    case 1:
        s->o.h->raw += ((short*)&GVARS.s44)[0] << 8;
        s->o.y.raw += ((short*)&GVARS.s46)[0] << 8;
        AnimAdvance(s);
        s->o.h->raw += s->o.velX << 8;
        s->o.y.raw += s->o.velY << 8;
        s->o.velY += 0x40;
        playerUpdateSpeedFromPad(s);
        if (s->o.velY >= -0x383) playerAirSpinToFall(s);
        if (s->o.velY > 0) {
            w = 0x10;
            s->o.d84 = 0;
            if (s->o.animFrame & 1) w = 0xf0;
            k = 1;
            s->o.d88 = w;
            *(unsigned char *)&s->o.wac = k;
            s->o.timer = 0xa;
            w = 2;
            s->o.b9c = w;
            s->o.step = w;
            s->o.state = 3;
        }
        if (ObjCheckHeadCollision(s)) {
            w = 0x10;
            s->o.d84 = 0;
            if (s->o.animFrame & 1) w = 0xf0;
            k = 1;
            *(unsigned char *)&s->o.wac = k;
            s->o.timer = 0xa;
            s->o.b9c = 2;
            s->o.step = 2;
            s->o.d88 = w;
            s->o.state = 3;
            s->o.velY = 0;
            s->o.velV = 0;
        }
        TileCollideAt(s, s->o.h->p.whole, s->o.y.p.whole + 0x10);
        s->o.b69 = 0;
        if ((SPAD.s1fc & SPAD.s3c8) == 0) break;
        switch (GAME.displayExpBar) {
        case 1:
            if (s->bcc == 1) spawnPlayerPowerEffect(0);
            return;
        case 2:
            if (s->bcc == 1) spawnPlayerPowerEffect(1);
            return;
        }
        s->bc3 = 0;
        if (s->be3 >= D_8007A038[GV2.bb2]) return;
        if (GV2.bb1 == 1 || GV2.bb1 == 2) return;
        p = PLAYER_EXT;
        if (p->b0 != 0) return;
        if (s->bc0 != 0) return;
        if (*(unsigned char *)&s->o.wac == 2) return;
        p->b8 = 1;
        PLAYER_EXT->b7 = s->o.animFrame;
        v = s->o.wb2;
        if (v < 0) {
            t = v;
            if (!(s->o.animFrame & 1))
                t = -v;
            s->o.wb2 = t;
        } else {
            if (s->o.animFrame & 1)
                v = -v;
            s->o.wb2 = v;
        }
        s->o.step = 4;
        s->o.state = 0;
        break;
    }
}
