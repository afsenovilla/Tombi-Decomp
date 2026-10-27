// FUNC 800f0448 360 I0NFO2
// MATCHING 800f0448 360
#include "TOBJ.H"
typedef struct {
    unsigned char box[4];
    unsigned short w1e;
    short spad;
    void **anims;
} ObjInit;
extern ObjInit TOPDOWN_ACTOR_DEFS[];
extern int SPR_DATA[];
extern void func_8001FE6C(TObj *o);

void topdownActorInit(TObj *o)
{
    o->box0 = TOPDOWN_ACTOR_DEFS[o->subtype].box[0];
    o->box1 = TOPDOWN_ACTOR_DEFS[o->subtype].box[1];
    o->box2 = TOPDOWN_ACTOR_DEFS[o->subtype].box[2];
    o->box3 = TOPDOWN_ACTOR_DEFS[o->subtype].box[3];
    o->w1e = TOPDOWN_ACTOR_DEFS[o->subtype].w1e;
    o->d3c = SPR_DATA[TOPDOWN_ACTOR_DEFS[o->subtype].spad];
    o->anim = TOPDOWN_ACTOR_DEFS[o->subtype].anims[(unsigned short)o->wb4];
    func_8001FE6C(o);
    o->d8c = 0;
    o->b0d = 0;
    if ((unsigned short)o->wb6 != 0) o->b6b = 0;
    else o->b6b = 0x80;
    o->category |= 0x80;
    o->b04++;
}
