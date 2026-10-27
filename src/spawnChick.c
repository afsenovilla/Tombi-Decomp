// FUNC 8012e0b8 264 X000
// MATCHING 8012e0b8 264
#include "TOBJ.H"
extern TObj *allocObjectLayer2(void);
extern unsigned short FUN_8001f9e0(void);
extern void FUN_8001fe6c(TObj *o);
extern unsigned char PLAYER_DRAW_DEPTH[];
extern int D_1f8002d4;
extern void *CHICK_ANIM;

void spawnChick(int x, int y, int z)
{
    TObj *o = allocObjectLayer2();
    if (o != 0) {
        o->active = 2;
        o->box0 = 6;
        o->box1 = 12;
        o->box2 = 6;
        o->box3 = 12;
        o->type = 3;
        o->animFrame = FUN_8001f9e0() & 1;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->w1e = 11;
        o->b0c = 1;
        o->b0d = 0;
        o->b69 = 0;
        o->w7a = 1;
        o->b0f = PLAYER_DRAW_DEPTH[0] - 1;
        o->d3c = D_1f8002d4;
        o->anim = CHICK_ANIM;
        o->b0a = 2;
        FUN_8001fe6c(o);
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
    }
}
