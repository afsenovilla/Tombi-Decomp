// FUNC 80101938 1084 X003
// MATCHING 80101938 1084
#include "plx.h"
#include "gv2.h"
#include "gvars.h"
typedef struct P { unsigned char b0; char p0[7]; unsigned char b8; char p1[0x20-9]; short h20; char p2[0x28-0x22]; unsigned short h28; unsigned short h2a; unsigned short h2c; unsigned short h2e; } P;
typedef struct O {
    unsigned char active; unsigned char visible; unsigned char type; char p00[3]; unsigned char state; char p07[0x16-7];
    short y; char p18[0x22-0x18]; short w22; void *anim; char p2[0x68-0x28];
    unsigned char b68; unsigned char b69; unsigned char b6a; char p6b[0x7c-0x6b]; short vx; short vy;
    char p3[0x8c-0x80]; int d8c; char p4[0x9c-0x90]; unsigned char b9c; unsigned char b9d; unsigned char b9e; char p9f[0xa4-0x9f];
    unsigned char ba4; unsigned char ba5; char pa6; unsigned char ba7;
    char p6[0xac-0xa8]; unsigned char bac; char p7[0xb0-0xad]; short hb0; short hb2; char pb4[2]; short hb6;
    char pb8[0xc6-0xb8]; unsigned char bc6; unsigned char bc7; char pc8[0xe0-0xc8]; short he0; char pe2; unsigned char be3;
} O;
typedef struct E { char p0[4]; unsigned char b4, b5, b6; } E;
extern P *PLAYER_EXT;
extern unsigned char SLOPE_TILT_TABLE[];
extern unsigned char D_8009D2AFa[]; /* same global; array read keeps it after the stores */
extern unsigned char TALK_POSE;
extern char D_80010748[];
extern E *BITTEN_OBJECT;
extern O *GRABBED_OBJECT;
extern void AnimJump(O *, int);
extern int AnimAdvance(O *);
extern short ObjTileCollide(O *, int, int);
extern void SfxPlay3(int, int);
extern void ObjGravityStep(O *);
extern void ObjAddVelY7E(O *);
extern void ObjSetAnimFromTable(O *);
extern void playerAirSpinToFall(O *);
extern void playerAnimateTalkPose(O *);

static __inline__ void land(O *o)
{
    PLAYER_EXT->b8 = 0;
    o->ba4 = 0;
    o->ba5 = 0;
    o->b9c = 0;
    o->bac = 0;
    o->hb2 = 0;
    o->vx = 0;
    o->vy = 0;
    o->d8c = SLOPE_TILT_TABLE[o->hb0];
    PLAYER_EXT->h20 = 0;
    o->anim = D_80010748;
    AnimJump(o, 0);
    PLAYER_EXT->h2e = 0xffff;
    PLAYER_EXT->h28 = 0xffff;
    PLAYER_EXT->h2a = 0xffff;
}

void playerEventStepLandIdle(O *o)
{
    P *p;
    O *q;

    switch (o->state) {
    case 0:
        o->ba4 = 0;
        o->ba5 = 0;
        o->b9c = 2;
        o->ba7 = 0;
        p = PLAYER_EXT;
        o->hb2 = 0;
        o->vx = 0;
        o->vy = 0;
        p->h20 = 0;
        o->visible = 1;
        if (o->active == 2)
            o->he0 = 0x8c;
        o->w22 = 0;
        if (o->bac >= 2) {
            BITTEN_OBJECT = (*(E **)&PLX.p611c);
            BITTEN_OBJECT->b4 = 2;
            BITTEN_OBJECT->b5 = 2;
            BITTEN_OBJECT->b6 = 0;
        }
        o->bac = 0;
        GVARS.i34 = 0;
        o->bc7 = 1;
        o->b9d = 0;
        o->bc6 = 0;
        o->be3 = 0;
        PLAYER_EXT->b0 = 0;
        q = GRABBED_OBJECT;
        if (q != 0 && q != (O *)1) {
            switch (q->type) {
            case 5:
                q->b6a = 0;
                break;
            case 0x21:
                q->b68 = 0;
                break;
            }
        }
        o->state++;
        if (o->b69 == 1 || ObjTileCollide(o, 4, 0) != 0) {
            land(o);
            if (D_8009D2AFa[0] == 0) {
                GV2.baf = 1;
                GVARS.b3a = 1;
            }
            o->state = 2;
        }
        break;
    case 1:
        playerAirSpinToFall(o);
        AnimAdvance(o);
        PLAYER_EXT->b8 = 1;
        o->b9e = 0;
        o->hb0 = 0;
        o->hb6 = 0;
        ObjGravityStep(o);
        ObjAddVelY7E(o);
        AnimAdvance(o);
        PLAYER_EXT->h2c = 4;
        if (PLAYER_EXT->h2e != 4) {
            PLAYER_EXT->h2c = 4;
            ObjSetAnimFromTable(o);
            AnimJump(o, 0);
            PLAYER_EXT->h2e = PLAYER_EXT->h2c;
        }
        if (o->b69 == 1 || ObjTileCollide(o, 4, 0) != 0) {
            SfxPlay3(0x1c, 0x7f);
            land(o);
            if (D_8009D2AFa[0] == 0) {
                GV2.baf = 1;
                GVARS.b3a = 1;
            }
            o->state++;
        }
        break;
    case 2:
        if (TALK_POSE != 0) {
            playerAnimateTalkPose(o);
        } else {
            PLAYER_EXT->h2c = 0;
            o->anim = D_80010748;
            AnimJump(o, 0);
        }
        if (o->b69 != 0)
            o->y += 2;
        ObjTileCollide(o, 0, 0);
        break;
    }
}
