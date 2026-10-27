// FUNC 801363e4 1704 X000
// MATCHING 801363e4 1704
// probably: variant-0 scene: message boxes 7 (DAT_8009cebf) with the part anims A00_VARIANT0_SCENE_PART_ANIMS..148
#include "TOBJ.H"
typedef struct { char p0[9]; unsigned char b9; } P801363E4;
typedef struct { short s0, s2; } S801363E4;

extern unsigned char DAT_8009c93f;
extern unsigned char PLAYER_STATE[], PLAYER_SUBSTATE, PLAYER_STEP;
extern S801363E4 *PLAYER_H;
extern unsigned char DAT_800a60a1[], DAT_800a60a2[], DAT_800a60a3[], DAT_800a60d4;
extern short PLAYER_ANIM_FRAME, DAT_800a60b4, PLAYER_VEL_Y, DAT_800a60b8, DAT_800a60ba, DAT_800a60ea;
extern P801363E4 *PLAYER_EXT;
extern TObj *PLAYER_MESSAGE_BOX;
extern unsigned char DAT_8009cebf[];
extern int GAME_AREA00_EVENT_CONTROL;
extern short PLAYER_X, PLAYER_Y, DAT_800a6052;
extern int DAT_800a60c4;
extern unsigned short *PLAYER_ANIM;
extern unsigned char DAT_8009d009;
extern void *D_8013B140[];
extern void *A00_VARIANT0_SCENE_PART_ANIMS, *D_8013B134, *D_8013B138, *D_8013B13C,  *D_8013B148;
extern char A00_VARIANT0_SCENE_ITEM_ENTRY[];

extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern TObj *showMessageBox(int, int, int, int);
extern void spawnFlashEffect(int, int, int, int);
extern void startPadVibration(int, int, int, int);
extern void applyItemEffect(char *, Fix16 *, int, int, int);
extern void FUN_8001e4f0(int);
extern void FUN_80026c50(int, int, int);

void FUN_801363e4(TObj *o)
{
    TObj *q = *(TObj **)&o->category;
    Fix16 v[3];

    switch (o->step) {
    case 0:
        DAT_8009c93f = 1;
        if (PLAYER_STATE[0] == 5) return;
        if (PLAYER_H->s2 > 0x130) PLAYER_H->s2 = 0x130;
        DAT_800a60a3[0] = 1;
        o->w08 = 100;
        PLAYER_ANIM_FRAME = 0;
        PLAYER_STATE[0] = 5;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->step++;
        break;
    case 1:
        if (DAT_800a60a1[0] == 0) return;
        if (--o->w08 != 0) return;
        PLAYER_ANIM_FRAME = 0;
        PLAYER_EXT->b9 = 5;
        DAT_800a60d4 = 1;
        DAT_800a60a1[0] = 0;
        DAT_800a60b4 = 0x220;
        PLAYER_STATE[0] = 6;
        DAT_800a60ea = 0;
        DAT_800a60b8 = 0;
        DAT_800a60ba = 0;
        PLAYER_VEL_Y = 0;
        PLAYER_SUBSTATE = 4;
        PLAYER_STEP = 0;
        o->step++;
        break;
    case 2:
        switch (o->state) {
        case 0:
            if (DAT_800a60a1[0] == 0) return;
            PLAYER_STATE[0] = 5;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            o->state++;
            break;
        case 1:
            if (PLAYER_H->s2 < 0x259) return;
            if (DAT_800a60a1[0] == 0) return;
            q->anim = D_8013B148;
            FUN_8001fe6c(q);
            PLAYER_MESSAGE_BOX = showMessageBox(7, DAT_8009cebf[0], 0xd0, 0x6c);
            q->anim = D_8013B134;
            FUN_8001fe6c(q);
            o->w08 = 0x30;
            o->state = 0;
            o->step++;
            break;
        }
        break;
    case 3:
        FUN_8001fec0(q);
        if (PLAYER_MESSAGE_BOX->b04 != 2) return;
        switch (DAT_8009cebf[0]) {
        case 0:
            switch (o->state) {
            case 0:
                o->w08 = 0x20;
                q->anim = D_8013B140[0];
                FUN_8001fe6c(q);
                o->state++;
                break;
            case 1:
                q->h->p.whole++;
                if (--o->w08 != 0) return;
                PLAYER_STATE[0] = 6;
                PLAYER_SUBSTATE = 5;
                PLAYER_STEP = 0;
                PLAYER_EXT->b9 = 10;
                DAT_800a60b4 = -0x128;
                DAT_800a60a1[0] = 0;
                o->state++;
                break;
            case 2:
                if (DAT_800a60a1[0] == 0) return;
                PLAYER_STATE[0] = 6;
                PLAYER_SUBSTATE = 3;
                PLAYER_STEP = 0;
                spawnFlashEffect(0, PLAYER_X, (short)(PLAYER_Y - 8), DAT_800a6052);
                spawnFlashEffect(2, PLAYER_X, (short)(PLAYER_Y - 8), DAT_800a6052);
                DAT_800a60c4 = 0;
                o->w08 = 400;
                o->state++;
                break;
            case 3:
                if (*PLAYER_ANIM == 0x3d) startPadVibration(0, 0, 0xff, 4);
                if (--o->w08 != 0) return;
                PLAYER_MESSAGE_BOX->b04 = 3;
                DAT_8009cebf[0]++;
                PLAYER_MESSAGE_BOX = showMessageBox(7, DAT_8009cebf[0], 0x120, 0x6c);
                q->anim = D_8013B138;
                FUN_8001fe6c(q);
                o->state = 0;
                break;
            }
            break;
        case 1:
            PLAYER_MESSAGE_BOX->b04 = 3;
            DAT_800a60a2[0] = 0;
            DAT_8009cebf[0]++;
            o->w08 = 0x78;
            v[0].p.whole = q->a.p.whole;
            v[1].p.whole = q->y.p.whole;
            v[2].p.whole = q->b.p.whole;
            q->anim = D_8013B13C;
            FUN_8001fe6c(q);
            applyItemEffect(A00_VARIANT0_SCENE_ITEM_ENTRY, v, 0, 0, 1);
            o->step = 5;
            break;
        case 2:
            GAME_AREA00_EVENT_CONTROL |= 0x28;
            PLAYER_MESSAGE_BOX->b04 = 3;
            o->w08 = 2;
            q->anim = A00_VARIANT0_SCENE_PART_ANIMS;
            FUN_8001fe6c(q);
            DAT_8009cebf[0] = 3;
            o->b04 = 3;
            break;
        }
        break;
    case 4:
        FUN_8001fec0(q);
        if (--o->w08 != 0) return;
        DAT_8009cebf[0]++;
        PLAYER_MESSAGE_BOX = showMessageBox(7, DAT_8009cebf[0], 0x120, 0x6c);
        q->anim = D_8013B138;
        FUN_8001fe6c(q);
        o->step = 3;
        break;
    case 5:
        FUN_8001fec0(q);
        if (o->w08 == 0x32) {
            FUN_8001e4f0(9);
            DAT_8009d009 = 1;
            FUN_80026c50(3, 1, 1);
        }
        if (--o->w08 != 0) return;
        PLAYER_MESSAGE_BOX = showMessageBox(7, DAT_8009cebf[0], 0x120, 0x6c);
        q->anim = D_8013B138;
        FUN_8001fe6c(q);
        o->step = 3;
        break;
    }
}
