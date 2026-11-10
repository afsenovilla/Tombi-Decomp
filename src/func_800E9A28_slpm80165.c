// VERSION SLPM_80165 x00 800E9A28 516
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of updateKomaPigDustPuff (src/updateKomaPigDustPuff.c, JP branch): area 0 or not instead of the
// per-area variant and anim tables.
#include "TOBJ.H"
extern int DAT_1f8002d0;
extern unsigned short GAME;
extern void *D_8012D210[], *D_8012EDE4[];
extern char MOVE_TABLE_0[];
extern void FUN_8001e4f0(int);
extern void FUN_8001fe6c(TObj *);
extern void FUN_800187e4(TObj *);
extern int FUN_8001fec0(TObj *);
extern void applyAnimVelocityX(TObj *, unsigned short);
extern int FUN_800202b4(TObj *);

void func_800E9A28(TObj *o)
{
    int v;
    switch (o->b04) {
    case 0:
        o->b04 = o->b04 + 1;
        v = DAT_1f8002d0;
        o->w1e = 1;
        *(signed char *)&o->b0f = -30;
        o->b0a = 0;
        o->movetab = MOVE_TABLE_0;
        o->d3c = v;
        if (o->subtype == 0) {
            if (GAME == 0) o->b0c = 0x41; else o->b0c = 0x10;
            o->timer = 0x5a;
            FUN_8001e4f0(0x10);
        } else {
            if (GAME == 0) o->b0c = 0x44; else o->b0c = 0x11;
            FUN_8001e4f0(0x27);
        }
        if (GAME == 0)
            o->anim = D_8012D210[o->b0c];
        else
            o->anim = D_8012EDE4[o->b0c];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (o->subtype == 0) {
            if (FUN_8001fec0(o) != 0)
                o->b04 = 3;
            applyAnimVelocityX(o, 1 - o->animFrame);
            o->timer = o->timer - 1;
            if (o->timer == -1)
                o->b04 = 3;
        } else {
            if (FUN_8001fec0(o) != 0)
                o->b04 = 3;
        }
        if (FUN_800202b4(o) == 0)
            o->b04 = 3;
        break;
    case 2:
        o->b04 = o->b04 + 1;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
#endif
