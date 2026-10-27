// FUNC 8010ca18 912 X010
// MATCHING 8010ca18 912
#include "TOBJ.H"
typedef struct {
    char pad0[2];
    short w02;
    char pad4[3];
    unsigned char b7;
    unsigned char b8;
    unsigned char b9;
    char pada[2];
    short w0c;
    short w0e;
    char pad10[0x18];
    unsigned short w28;
    unsigned short w2a;
    char pad2c[2];
    unsigned short w2e;
} G330;
#define OB(o, n) (((unsigned char *)(o))[n])
extern G330 *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern unsigned short PAD_PRESSED;
extern int GAME;
extern unsigned char GAME_PLAYER_MODE;
extern unsigned short JOYPAD_STATE;
extern unsigned short BUTTON_MASK_B;
extern char DAT_80011148[];
extern void FUN_8001fe94(TObj *, int);
extern void playerClimbFollowObject(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001e560(int, int);
extern void playerClimbJumpOff(TObj *);

void playerStepClimbObject(TObj *o)
{
    volatile unsigned short *pad;
    int t;
    Fix16 *h;
    int v;
    PLAYER_EXT->b7 = o->animFrame;
    switch (o->state) {
    case 0:
        o->wb6 = -0x420;
        PLAYER_EXT->w0e = 0;
        PLAYER_EXT->w02 = 0x10;
        o->wb2 = GRABBED_OBJECT->box1;
        OB(o, 0xa9) = 1;
        o->animFrame &= 1;
        PLAYER_EXT->b8 = 0;
        PLAYER_EXT->b9 = 0;
        PLAYER_EXT->w0c = 0;
        PLAYER_EXT->w2e = 0xffff;
        PLAYER_EXT->w28 = 0xffff;
        PLAYER_EXT->w2a = 0xffff;
        o->velH = GRABBED_OBJECT->h->p.whole - o->h->p.whole;
        o->velV = GRABBED_OBJECT->y.p.whole - o->y.p.whole;
        o->wb0 = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velX = o->wb8;
        o->h->p.whole = GRABBED_OBJECT->h->p.whole + o->wb8;
        o->y.p.whole = GRABBED_OBJECT->y.p.whole + o->wba + 8;
        o->anim = DAT_80011148;
        FUN_8001fe94(o, 0);
        o->timer = 10;
        o->state++;
    case 1:
        playerClimbFollowObject(o);
        if (--o->timer == 0)
            o->state = 2;
        break;
    case 2:
        if (GRABBED_OBJECT->type == 0x35)
            goto up;
        pad = &PAD_PRESSED;
        if (*pad & 0x10) {
            FUN_800eeb5c(o, 0x26);
            FUN_8001fec0(o);
            if (o->b9e != 0xc)
                o->y.raw -= 0x18000;
        } else if (*pad & 0x40) {
        up:
            FUN_800eeb5c(o, 0x27);
            FUN_8001fec0(o);
            o->y.raw += 0x28000;
        }
        if (*(unsigned short *)o->anim == 0xda)
            *(signed char *)&o->b0f = 1;
        else
            *(signed char *)&o->b0f = -8;
        playerClimbFollowObject(o);
        if (GAME == 0x30000)
            break;
        if (o->animFrame & 1)
            t = JOYPAD_STATE & 0x80;
        else
            t = JOYPAD_STATE & 0x20;
        if (t) {
            FUN_8001e560(0x1e, 8);
            PLAYER_EXT->w2e = 0xff;
            FUN_800eeb5c(o, 0x38);
            o->state = 3;
        }
        break;
    case 3:
        if (FUN_8001fec0(o)) {
            h = o->h;
            v = h->p.whole;
            if (o->animFrame & 1)
                t = v - 0xc;
            else
                t = v + 0xc;
            h->p.whole = t;
            o->anim = DAT_80011148;
            o->animFrame ^= 1;
            FUN_8001fe94(o, 0);
            o->timer = 10;
            o->state = 1;
        }
        break;
    default:
        return;
    }
    if (JOYPAD_STATE & BUTTON_MASK_B) {
        GAME_PLAYER_MODE = 0;
        playerClimbJumpOff(o);
    }
}
