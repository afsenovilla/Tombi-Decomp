// FUNC 800efc8c 656 X000
// MATCHING 800efc8c 656
// VERSION SLPM_80165 x00 800EDA7C 612
// VERSION SLPS_01144 x00 800EFDF8 656
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 800EFC8C (playerEnterGrabStep.c): Demo: no GVARS.i34 reset, flag at +0xcf (PAL +0xe3),
// player states up to 10, no area-10 special case.
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { TObj o; char p0[6]; unsigned char c6, c7; char p1[0xe3 - 0xc8]; unsigned char e3; } TObjX;
extern TObj *BITTEN_OBJECT;
extern unsigned char *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern char D_80010C0C[];
extern void AnimLoadDuration(TObjX *o);
extern void SfxPlay2(int a, int b);
extern void SfxPlay3(int a, int b);

void playerEnterGrabStep(TObjX *x, short f)
{
    TObj *g;
    if (*(unsigned char *)&x->o.wac >= 2) {
        BITTEN_OBJECT = (*(TObj **)&PLX.p611c);
        BITTEN_OBJECT->b04 = 2;
        BITTEN_OBJECT->step = 2;
        BITTEN_OBJECT->state = 0;
    }
    *(unsigned char *)&x->o.wac = 0;
    x->c7 = 1;
    x->o.b9d = 0;
    x->c6 = 0;
    ((unsigned char *)x)[0xcf] = 0;
    PLAYER_EXT[0] = 0;
    switch (x->o.b9e) {
    case 1:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 7; x->o.state = 0;
        break;
    case 2:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 6; x->o.state = 0;
        break;
    case 3:
        if ((unsigned short)(x->o.velX + 0x1c0) > 0x380) PLAYER_EXT[5] = 1;
        x->o.anim = D_80010C0C;
        AnimLoadDuration(x);
        x->o.b0f = 0;
        g = GRABBED_OBJECT;
        x->o.d30 = g->h->p.whole + x->o.wb8 + 4;
        x->o.d34 = g->y.p.whole + x->o.wba - 4;
        x->o.h->p.whole = x->o.d30;
        x->o.step = 5;
        x->o.state = 0;
        x->o.y.p.whole = x->o.d34 + 12;
        return;
    case 4:
        if ((GRABBED_OBJECT->category & 0x7f) == 2) {
            SfxPlay2(0x1e, 0x1c);
            x->o.step = 8; x->o.state = 0;
        } else {
            if (GRABBED_OBJECT->type == 2) GRABBED_OBJECT->b69 = 1;
            SfxPlay2(0x1e, 0x1c);
            x->o.step = 8; x->o.state = 0;
        }
        break;
    case 5:
        x->o.velX = 0;
        x->o.velY = 0;
        if (f != 0) { x->o.step = 0xc; x->o.state = 0; }
        else { x->o.step = 0x24; x->o.state = 0; }
        break;
    case 6:
        x->o.velX = 0;
        x->o.velY = 0;
        SfxPlay3(4, 0x7f);
        if (f != 0) { x->o.step = 0xd; x->o.state = 0; }
        else { x->o.step = 0x25; x->o.state = 0; }
        break;
    case 7:
        x->o.step = 0x22; x->o.state = 0;
        break;
    case 8:
        x->o.step = 0x2e; x->o.state = 0;
        break;
    case 9:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 6; x->o.state = 0;
        break;
    case 10:
        x->o.step = 0x2f; x->o.state = 0;
        break;
    }
}
#elif defined(VERSION_SLPS_01144)
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { TObj o; char p0[6]; unsigned char c6, c7; char p1[0xe3 - 0xc8]; unsigned char e3; } TObjX;
extern TObj *BITTEN_OBJECT;
extern unsigned char *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern char D_80010C0C[];
extern void AnimLoadDuration(TObjX *o);
extern void SfxPlay2(int a, int b);
extern void SfxPlay3(int a, int b);

void playerEnterGrabStep(TObjX *x, short f)
{
    TObj *g;
    if (*(unsigned char *)&x->o.wac >= 2) {
        BITTEN_OBJECT = (*(TObj **)&PLX.p611c);
        BITTEN_OBJECT->b04 = 2;
        BITTEN_OBJECT->step = 2;
        BITTEN_OBJECT->state = 0;
    }
    GVARS.i34 = 0;
    *(unsigned char *)&x->o.wac = 0;
    x->c7 = 1;
    x->o.b9d = 0;
    x->c6 = 0;
    ((unsigned char *)x)[0xd3] = 0;
    PLAYER_EXT[0] = 0;
    switch (x->o.b9e) {
    case 1:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 7; x->o.state = 0;
        break;
    case 2:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 6; x->o.state = 0;
        break;
    case 3:
        if ((unsigned short)(x->o.velX + 0x1c0) > 0x380) PLAYER_EXT[5] = 1;
        x->o.anim = D_80010C0C;
        AnimLoadDuration(x);
        x->o.b0f = 0;
        g = GRABBED_OBJECT;
        x->o.d30 = g->h->p.whole + x->o.wb8 + 4;
        x->o.d34 = g->y.p.whole + x->o.wba - 4;
        x->o.h->p.whole = x->o.d30;
        x->o.step = 5;
        x->o.state = 0;
        x->o.y.p.whole = x->o.d34 + 12;
        return;
    case 4:
        if ((GRABBED_OBJECT->category & 0x7f) == 2) {
            SfxPlay2(0x1e, 0x1c);
            x->o.step = 8; x->o.state = 0;
        } else {
            if (GRABBED_OBJECT->type == 2) GRABBED_OBJECT->b69 = 1;
            SfxPlay2(0x1e, 0x1c);
            x->o.step = 8; x->o.state = 0;
        }
        break;
    case 5:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.w74 = x->o.h->p.whole;
        x->o.w76 = x->o.y.p.whole;
        if (f != 0) { x->o.step = 0xc; x->o.state = 0; }
        else { x->o.step = 0x24; x->o.state = 0; }
        break;
    case 6:
        x->o.velX = 0;
        x->o.velY = 0;
        SfxPlay3(4, 0x7f);
        if (f != 0) { x->o.step = 0xd; x->o.state = 0; }
        else { x->o.step = 0x25; x->o.state = 0; }
        break;
    case 7:
        if (GAME.selectedArea != 10) { x->o.step = 0x22; x->o.state = 0; }
        else { x->o.step = 0x46; x->o.state = 0; }
        break;
    case 8:
        x->o.step = 0x2e; x->o.state = 0;
        break;
    case 9:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 6; x->o.state = 0;
        break;
    case 10: case 11: case 12:
        x->o.step = 0x2f; x->o.state = 0;
        break;
    }
}
#else
#include "plx.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { TObj o; char p0[6]; unsigned char c6, c7; char p1[0xe3 - 0xc8]; unsigned char e3; } TObjX;
extern TObj *BITTEN_OBJECT;
extern unsigned char *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern char D_80010C0C[];
extern void AnimLoadDuration(TObjX *o);
extern void SfxPlay2(int a, int b);
extern void SfxPlay3(int a, int b);

void playerEnterGrabStep(TObjX *x, short f)
{
    TObj *g;
    if (*(unsigned char *)&x->o.wac >= 2) {
        BITTEN_OBJECT = (*(TObj **)&PLX.p611c);
        BITTEN_OBJECT->b04 = 2;
        BITTEN_OBJECT->step = 2;
        BITTEN_OBJECT->state = 0;
    }
    *(unsigned char *)&x->o.wac = 0;
    GVARS.i34 = 0;
    x->c7 = 1;
    x->o.b9d = 0;
    x->c6 = 0;
    x->e3 = 0;
    PLAYER_EXT[0] = 0;
    switch (x->o.b9e) {
    case 1:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 7; x->o.state = 0;
        break;
    case 2:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 6; x->o.state = 0;
        break;
    case 3:
        if ((unsigned short)(x->o.velX + 0x1c0) > 0x380) PLAYER_EXT[5] = 1;
        x->o.anim = D_80010C0C;
        AnimLoadDuration(x);
        x->o.b0f = 0;
        g = GRABBED_OBJECT;
        x->o.d30 = g->h->p.whole + x->o.wb8 + 4;
        x->o.d34 = g->y.p.whole + x->o.wba - 4;
        x->o.h->p.whole = x->o.d30;
        x->o.step = 5;
        x->o.state = 0;
        x->o.y.p.whole = x->o.d34 + 12;
        return;
    case 4:
        if ((GRABBED_OBJECT->category & 0x7f) == 2) {
            SfxPlay2(0x1e, 0x1c);
            x->o.step = 8; x->o.state = 0;
        } else {
            if (GRABBED_OBJECT->type == 2) GRABBED_OBJECT->b69 = 1;
            SfxPlay2(0x1e, 0x1c);
            x->o.step = 8; x->o.state = 0;
        }
        break;
    case 5:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.w74 = x->o.h->p.whole;
        x->o.w76 = x->o.y.p.whole;
        if (f != 0) { x->o.step = 0xc; x->o.state = 0; }
        else { x->o.step = 0x24; x->o.state = 0; }
        break;
    case 6:
        x->o.velX = 0;
        x->o.velY = 0;
        SfxPlay3(4, 0x7f);
        if (f != 0) { x->o.step = 0xd; x->o.state = 0; }
        else { x->o.step = 0x25; x->o.state = 0; }
        break;
    case 7:
        if (GAME.selectedArea != 10) { x->o.step = 0x22; x->o.state = 0; }
        else { x->o.step = 0x46; x->o.state = 0; }
        break;
    case 8:
        x->o.step = 0x2e; x->o.state = 0;
        break;
    case 9:
        x->o.velX = 0;
        x->o.velY = 0;
        x->o.step = 6; x->o.state = 0;
        break;
    case 10: case 11: case 12:
        x->o.step = 0x2f; x->o.state = 0;
        break;
    }
}
#endif
