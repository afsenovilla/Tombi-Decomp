// FUNC 800f3c28 348 I0NFO2
// MATCHING 800f3c28 348
#include "TOBJ.H"
typedef struct {
    unsigned char box[4];
    unsigned short w1e;
    short spad;
    void **anims;
} ObjInit;
extern ObjInit TOPDOWN_A16_ACTOR_DEFS[];
extern int SPR_DATA[];
extern void func_8001FE94(TObj *o, int n);

void topdownA16ActorInit(TObj *o)
{
    o->box0 = TOPDOWN_A16_ACTOR_DEFS[o->subtype].box[0];
    o->box1 = TOPDOWN_A16_ACTOR_DEFS[o->subtype].box[1];
    o->box2 = TOPDOWN_A16_ACTOR_DEFS[o->subtype].box[2];
    o->box3 = TOPDOWN_A16_ACTOR_DEFS[o->subtype].box[3];
    o->w1e = TOPDOWN_A16_ACTOR_DEFS[o->subtype].w1e;
    o->d3c = SPR_DATA[TOPDOWN_A16_ACTOR_DEFS[o->subtype].spad];
    o->anim = TOPDOWN_A16_ACTOR_DEFS[o->subtype].anims[o->animFrame];
    func_8001FE94(o, 0);
    o->b6b = 0x7F;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
}
