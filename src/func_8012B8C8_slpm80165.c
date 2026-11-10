// VERSION SLPM_80165 x00 8012B8C8 1780
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of updateCharlesTalkScene, from the JP branch (func_80135DC0 of src/updateCharlesTalkScene.c): message
// bank 28 (JP 4), item effect entry 5, six talk lines in place of the JP 16 (no button-config offset).
#include "TOBJ.H"
typedef struct { TObj o; unsigned char bc0; } BigObj;
extern BigObj PLAYER;
#define X PLAYER.o
extern unsigned char DAT_8009c93e, DAT_8009c93f, GVARS_USE_ITEM_PENDING, GVARS_USE_ITEM_ID, DAT_8009c942;
extern unsigned char DAT_8009cebd, DAT_8009d0a7, GAME_PLAYER_MODE, DAT_8009ce4a, DAT_8009d00d, BUTTON_CONFIG_INDEX;
extern int GAME_AREA00_EVENT_CONTROL;
extern short D_800A7B8A[], PLAYER_Y[], PLAYER_Z[];
extern unsigned char DAT_8009d00dA[];
extern unsigned char *PLAYER_EXT;
extern void *NPC_CHARLES_ANIMS, *PTR_8013ac18, *PTR_8013ac20, *PTR_8013ac24, *PTR_8013ac2c;
extern char PLAYER_IDLE_ANIM[];
extern int showMessageBox(int, int, int, int);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void applyItemEffect(void *, void *, int, int, int);
extern void FUN_80026e0c(int, int);
extern void printInfoMessage(int, int);
extern void FUN_8001e4f0(int);

static __inline__ short chk(void)
{
    short r = 0;
    if ((GAME_AREA00_EVENT_CONTROL & 0x20) && GVARS_USE_ITEM_PENDING != 0)
        r = GVARS_USE_ITEM_ID == 3;
    return r;
}
static __inline__ int chk2(void)
{
    int r = 0;
    if (DAT_8009d0a7 != 0) {
        r = 0;
        if (GVARS_USE_ITEM_PENDING != 0)
            r = GVARS_USE_ITEM_ID == 3;
    }
    return r;
}

void func_8012B8C8(TObj *o)
{
    TObj *p = *(TObj **)&o->category;
    short buf[6];
    int f;

    switch (o->step) {
    case 0:
        DAT_8009c93f = 1;
        if (*(unsigned short *)&o->anim != 0) {
            PLAYER.bc0 = 1;
            DAT_8009cebd = 1;
            o->step = 3;
            break;
        }
        if (chk()) {
            X.animFrame = 1;
            DAT_8009cebd = 2;
            DAT_8009c93e = 1;
            X.b04 = 1;
            X.step = 0;
            X.state = 0;
            GAME_PLAYER_MODE = 0;
            X.d90 = showMessageBox(28, 2, 0x80, 0x6c);
            p->anim = PTR_8013ac20;
            AnimLoadDuration(p);
            p->b04 = 2;
            p->step = 4;
            p->state = 0;
        } else {
            DAT_8009cebd = 0;
            X.d90 = showMessageBox(28, 0, 0x80, 0x6c);
        }
        o->step++;
        break;
    case 1:
        AnimAdvance(p);
        if (((TObj *)X.d90)->b04 != 2)
            break;
        switch (DAT_8009cebd) {
        case 0:
            ((TObj *)X.d90)->b04 = 3;
            p->b04 = 2;
            p->step = 1;
            p->state = 0;
            X.animFrame = 1;
            X.b04 = 6;
            X.step = 5;
            X.state = 0;
            PLAYER_EXT[9] = 10;
            X.wb2 = 0;
            X.velH = 0;
            X.velV = 0;
            X.velX = 0xc0;
            X.b69 = 0;
            DAT_8009cebd = 1;
            o->step = 3;
            break;
        case 1:
            ((TObj *)X.d90)->b04 = 3;
            p->anim = PTR_8013ac18;
            AnimLoadDuration(p);
            p->active = 1;
            p->b04 = 1;
            p->step = 0;
            p->state = 0;
            if (DAT_8009ce4a == 0) {
                setEventStarted(0xa6, 0, 0);
                o->w08 = 300;
                o->step = 6;
                o->state = 0;
                o->substep = 0;
                break;
            }
            DAT_8009cebd = 0;
            X.animFrame = 1;
            X.b04 = 1;
            X.step = 0;
            X.state = 0;
            X.velX = 0;
            X.velY = 0;
            X.b69 = 1;
            PLAYER_EXT[9] = 0;
            o->step = 0;
            o->state = 0;
            goto clear;
        case 2:
            ((TObj *)X.d90)->b04 = 3;
            buf[1] = D_800A7B8A[0] - 32;
            buf[3] = PLAYER_Y[0];
            buf[5] = PLAYER_Z[0];
            applyItemEffect((void *)5, buf, 0, 0, 1);
            X.b04 = 5;
            X.step = 0;
            X.state = 0;
            p->active = 1;
            DAT_8009cebd = 3;
            FUN_80026e0c(3, 1);
            o->w08 = 0x50;
            o->step = 4;
            break;
        case 3:
            ((TObj *)X.d90)->b04 = 3;
            p->anim = PTR_8013ac2c;
            DAT_8009cebd = 4;
            AnimLoadDuration(p);
            o->step = 4;
            break;
        case 4:
            ((TObj *)X.d90)->b04 = 3;
            p->anim = NPC_CHARLES_ANIMS;
            AnimLoadDuration(p);
            DAT_8009cebd = 5;
            o->step = 4;
            break;
        case 5:
            ((TObj *)X.d90)->b04 = 3;
            setEventComplete(0xa6, 0);
            o->step = 5;
            printInfoMessage(0xc, 3);
            p->b04 = 2;
            p->step = 3;
            p->state = 0;
            o->w08 = 0x50;
            break;
        }
        break;
    case 3:
        AnimAdvance(p);
        if (X.b69 != 0) {
            p->active = 1;
            p->anim = PTR_8013ac24;
            AnimLoadDuration(p);
            p->b04 = 2;
            p->step = 4;
            p->state = 0;
            X.animFrame = 1;
            X.b04 = 5;
            X.step = 0;
            X.state = 0;
            X.b69 = 4;
            X.anim = PLAYER_IDLE_ANIM;
            AnimLoadDuration(&X);
            DAT_8009cebd = 1;
            X.d90 = showMessageBox(28, 1, 0x80, 0x6c);
            o->step = 1;
        }
        break;
    case 4:
        if (o->w08 == 10)
            FUN_8001e4f0(9);
        if (--o->w08 > 0)
            break;
        X.d90 = showMessageBox(28, DAT_8009cebd, 0x80, 0x6c);
        o->step = 1;
        break;
    case 5:
        if (--o->w08 > 0)
            break;
        X.b04 = 1;
        X.step = 0;
        X.state = 0;
        DAT_8009c942 = 0;
        PLAYER.bc0 = 0;
        DAT_8009c93f = 0;
        { unsigned char *c = &DAT_8009cebd; *c = *c + 1; }
        o->b04 = 2;
        DAT_8009c93e = 0;
        GVARS_USE_ITEM_PENDING = 0;
        GAME_AREA00_EVENT_CONTROL |= 0x40;
        break;
    case 6:
        if (--o->w08 > 0)
            break;
        X.animFrame = 1;
        X.b04 = 1;
        X.step = 0;
        X.state = 0;
        X.velX = 0;
        X.velY = 0;
        X.b69 = 1;
        o->step = 0;
        o->state = 0;
        DAT_8009cebd = 0;
    clear:
        *(short *)&o->anim = 0;
        PLAYER.bc0 = 0;
        DAT_8009c93f = 0;
        DAT_8009c942 = 0;
        DAT_8009c93e = 0;
        break;
    }
}
#endif
