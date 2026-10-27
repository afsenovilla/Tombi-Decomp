// FUNC 801340c4 204 X000
// MATCHING 801340c4 204
#include "TOBJ.H"
extern TObj *allocObjectLayer2(void);
extern void FUN_8001fe6c(TObj *);
extern unsigned short PLAYER_ANIM_FRAME;
extern int DAT_1f8002d4[];
extern void *PTR_DAT_8013b184[];

void spawnFrog(int x, int y, int z)
{
    TObj *o = allocObjectLayer2();
    if (o) {
        unsigned short u;
        o->active = 1;
        o->type = 0x12;
        u = PLAYER_ANIM_FRAME;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->w1e = 0xb;
        o->b0d = 0;
        o->animFrame = u & 1;
        o->d3c = DAT_1f8002d4[0];
        o->anim = PTR_DAT_8013b184[0];
        o->b0a = 2;
        FUN_8001fe6c(o);
        o->b04 = 2;
        o->step = 3;
        o->state = 0;
    }
}
