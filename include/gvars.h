/* GVARS (0x8009C930..0x8009C95F): the block of small global game variables right before GAME (0x8009C960): flags/counters
   (bytes at +0x08..+0x13, halfwords at +0x14/+0x16) and object pointers (+0x18..+0x24). Fields are named by their address
   (b3f = 0x8009C93F ...) until their meaning is known. Plain C types, self-contained. */
#ifndef _TOMBA_GVARS_H
#define _TOMBA_GVARS_H
typedef struct GVars {
    unsigned char b30;          /* 0x00 0x8009C930 */
    unsigned char _pad31[3];    /* 0x01 */
    int i34;                    /* 0x04 */
    unsigned char hurtState; /* 0x08 0x8009C938 GVARS_PLAYER_HURT_STATE: 1 stunned (playerStepHurtStun / Spin), 2 dead (playerStepDie) */
    unsigned char b39;          /* 0x09 */
    unsigned char playerLanded; /* 0x0A 0x8009C93A GVARS_PLAYER_LANDED: set 1 by the landing event steps, the camera controllers wait for it */
    unsigned char b3b;          /* 0x0B */
    unsigned char fadeEffect; /* 0x0C 0x8009C93C GVARS_FADE_EFFECT: scriptOpSetFadeEffect arg 0 */
    unsigned char b3d;          /* 0x0D */
    unsigned char b3e;          /* 0x0E */
    unsigned char b3f;          /* 0x0F */
    unsigned char useItemPending; /* 0x10 0x8009C940 GVARS_USE_ITEM_PENDING: an item use waits for playerStepUseItem */
    unsigned char useItemId; /* 0x11 0x8009C941 GVARS_USE_ITEM_ID: the item being used */
    unsigned char b42;          /* 0x12 */
    unsigned char b43;          /* 0x13 */
    short driftH;               /* 0x14 0x8009C944 GVARS_PLAYER_DRIFT_H: velocity << 8 added to the player h every tick */
    short driftY;               /* 0x16 0x8009C946 GVARS_PLAYER_DRIFT_Y: velocity << 8 added to the player y (-0x40 in water) */
    void *p48;                  /* 0x18 */
    void *p4c;                  /* 0x1C */
    void *p50;                  /* 0x20 */
    void *p54;                  /* 0x24 */
    unsigned char b58;          /* 0x28 */
    unsigned char b59;          /* 0x29 */
    unsigned char _pad5a[2];    /* 0x2A */
    int i5c;                    /* 0x2C */
} GVars;
extern GVars GVARS;
#endif
