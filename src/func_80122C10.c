// FUNC 80122c10 772 X000
// MATCHING 80122c10 772
// probably: player step 0x35: thrown off after a bite (anim 0x37, sfx 0x23/0x1F, playerReleaseBittenAirborne), velX +-0x200 velY -0x300, lands and skids to a stop
#include "TOBJ.H"
typedef struct { char pad[8]; unsigned char b8; char pad2[0x2e - 9]; short w2e; } P;
extern P *PLAYER_EXT;
extern unsigned char D_801152E8[];
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void SfxPlay2(int, int);
extern void playSFX(int);
extern void playerReleaseBittenAirborne(TObj *);
extern int AnimAdvance(TObj *);
extern short ObjTileCollide(TObj *, int, int);
extern void playerSetAnimIfChangedAt(TObj *, int, int);

void func_80122C10(TObj *o)
{
    int v;
    switch (o->state) {
    case 0:
        if (o->animFrame & 1) {
            v = -0x200;
        } else {
            v = 0x200;
        }
        o->timer = 0x14;
        o->velX = v;
        o->velY = -0x300;
        o->state++;
    case 1:
        PlayerSetAnimIfChanged(o, 0x37);
        SfxPlay2(0x23, 0x24);
        playSFX(0x1f);
        playerReleaseBittenAirborne(o);
        break;
    case 2:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0) {
            o->b9c = 2;
            o->state++;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0x680) {
            o->velY = 0x680;
        }
        if (o->b69 == 0 && !ObjTileCollide(o, 0, 0)) {
            break;
        }
        o->state++;
        break;
    case 4:
        AnimAdvance(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY = 0x680;
        if (o->animFrame & 1) {
            o->velX += 0x10;
            if (o->velX <= 0) goto skip;
        } else {
            o->velX -= 0x10;
            if (o->velX >= 0) goto skip;
        }
        PLAYER_EXT->w2e = 0xff;
        playerSetAnimIfChangedAt(o, 0x37, 2);
        o->state = 5;
    skip:
        if (o->b69) {
            o->velY = 0;
            o->y.p.whole += 2;
        }
        if (ObjTileCollide(o, 0, 0)) {
            o->velY = 0;
        }
        break;
    case 5:
        if (AnimAdvance(o)) {
            o->b9c = 0;
            PLAYER_EXT->b8 = 0;
            o->active = 1;
            o->wb2 = 0;
            o->wb6 = 0;
            o->velX = 0;
            o->velY = 0;
            o->velH = 0;
            o->velV = 0;
            PLAYER_EXT->w2e = 0xff;
            o->d8c = D_801152E8[o->wb0];
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
