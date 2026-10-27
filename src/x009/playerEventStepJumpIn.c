// FUNC 80108338 976 X009
// MATCHING 80108338 976
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"

extern unsigned short GAME[];
extern unsigned short GAME_SELECTED_SECTION;
extern char *PLAYER_EXT;
void playerAirSpinToFall(TObj *o);
void AnimAdvance(TObj *o);
void PlayerSetAnimIfChanged(TObj *o, int a);
void SfxPlay2(int a, int b);
short ObjTileCollide(TObj *o, int a, int b);
void TileCollideAt(TObj *o, int x, int y);
int ObjCheckHeadCollision(TObj *o);

void playerEventStepJumpIn(TObj *o)
{
    switch (o->state) {
    case 0:
        o->active = 4;
        o->velH = 0;
        o->velV = 0;
        if ((GAME[0] == 2 || GAME[0] == 0x13) && GAME_SELECTED_SECTION == 1) {
            short v = 0x100;
            if (o->animFrame & 1)
                v = -0x100;
            o->velX = v;
            o->velY = -0xa00;
        } else {
            if (GAME[0] != 4) {
                o->active = 1;
                o->velX = 0;
                o->velY = 0;
                PLAYER_EXT[8] = 0;
                o->ba5 = 0;
                o->b9c = 0;
                *(unsigned char *)&o->wac = 0;
                *(signed char *)&o->b0f = -8;
                o->wb2 = 0;
                *(short *)(PLAYER_EXT + 0x20) = 0;
                o->step = 2;
                o->b04 = 1;
                o->state = 3;
                o->substep = 0;
                o->timer = 0;
                GV2.bb0 = 0;
                GVARS.b3a = 1;
                return;
            }
            o->velX = 0;
            o->velY = 0;
        }
        o->b69 = 0;
        o->b9e = 0;
        o->b9c = 1;
        o->wb2 = 0;
        o->d8c = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        PLAYER_EXT[8] = 0;
        *(short *)(PLAYER_EXT + 0x20) = 0;
        *(unsigned char *)&o->wac = 0;
        PlayerSetAnimIfChanged(o, 4);
        SfxPlay2(2, 4);
        o->state = 1;
    case 1:
        playerAirSpinToFall(o);
        AnimAdvance(o);
        o->h->raw += ((short*)&GVARS.s44)[0] << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0) {
            o->active = 3;
            o->b9c = 2;
            o->state++;
        }
        TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        if (ObjCheckHeadCollision(o)) {
            o->active = 3;
            o->b9c = 2;
            o->velY = 0;
            o->velV = 0;
            o->state = 2;
        }
        o->b69 = 0;
        break;
    case 2:
        playerAirSpinToFall(o);
        AnimAdvance(o);
        o->b9e = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        o->h->raw += ((short*)&GVARS.s44)[0] << 8;
        o->y.raw += ((short*)&GVARS.s46)[0] << 8;
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0x680)
            o->velY = 0x680;
        if (o->b69 == 0 && ObjTileCollide(o, 0, 0) == 0)
            return;
        o->active = 1;
        PLAYER_EXT[8] = 0;
        o->ba5 = 0;
        o->b9c = 0;
        *(unsigned char *)&o->wac = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        *(short *)(PLAYER_EXT + 0x20) = 0;
        *(signed char *)&o->b0f = -8;
        *(short *)(PLAYER_EXT + 0x20) = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
        o->timer = 0;
        GVARS.b3a = 1;
        break;
    }
}
