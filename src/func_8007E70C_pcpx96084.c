// VERSION PCPX_96084 tomba 8007E70C 520
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: SLPM_80165 C of src/func_80128D94_slpm80165.c (func_80128D94_slpm80165, twin score 0.851) [-O2 -G0] (tools/demo_sweep.py)
// Demo version: JP C of src/func_80131EF4_slps01144.c (func_80131EF4) (0.962 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"
extern unsigned short GAME_SELECTED_SECTION;
extern unsigned short PLAYER_ANIM_FRAME;
extern void *FROG_ANIMS;
extern void *DAT_8013b184[];
extern void *DAT_8013b188[];
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern short FUN_80040278(TObj *, int, int);

void func_80128D94_slpm80165(TObj *o)
{
    short f;
    switch (o->state) {
    case 0:
        o->box2 = 8;
        o->box3 = 0x10;
        o->active = 2;
        {
            short x = o->h->p.whole;
            o->velY = -0x400;
            o->velX = x;
        }
        o->anim = DAT_8013b184[0];
        FUN_8001fe6c(o);
        {
            unsigned short c = GAME_SELECTED_SECTION;
            unsigned char s = o->state;
            o->state = s + 1;
        }
    case 1:
        FUN_8001fec0(o);
        f = ((unsigned short *)o->anim)[2];
        o->animFrame = f;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (o->velY > 0)
            o->state = 2;
        break;
    case 2:
        FUN_8001fec0(o);
        f = ((unsigned short *)o->anim)[2];
        o->animFrame = f;
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x20))) {
            o->animFrame = PLAYER_ANIM_FRAME ^ 1;
            o->anim = DAT_8013b188[0];
            FUN_8001fe6c(o);
            o->state = 3;
        }
        break;
    case 3:
        FUN_8001fec0(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x20;
        if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) {
            o->active = 1;
            o->state = 4;
        }
        break;
    case 4:
        if (FUN_8001fec0(o)) {
            o->anim = FROG_ANIMS;
            FUN_8001fe6c(o);
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}
#endif
