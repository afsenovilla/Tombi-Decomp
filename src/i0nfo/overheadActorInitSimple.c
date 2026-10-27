// FUNC 800ed910 312 I0NFO
// MATCHING 800ed910 312
#include "TOBJ.H"
typedef struct {
    unsigned char box[4];
    unsigned short w1e;
    short spad;
    void **anims;
} ObjInit;
extern ObjInit OVERHEAD_ACTOR_DEFS[];
extern int SPR_DATA[];
extern void func_8001FE6C(TObj *o);

void overheadActorInitSimple(TObj *o)
{
    o->box0 = OVERHEAD_ACTOR_DEFS[o->subtype].box[0];
    o->box1 = OVERHEAD_ACTOR_DEFS[o->subtype].box[1];
    o->box2 = OVERHEAD_ACTOR_DEFS[o->subtype].box[2];
    o->box3 = OVERHEAD_ACTOR_DEFS[o->subtype].box[3];
    o->w1e = OVERHEAD_ACTOR_DEFS[o->subtype].w1e;
    o->d3c = SPR_DATA[OVERHEAD_ACTOR_DEFS[o->subtype].spad];
    o->anim = *OVERHEAD_ACTOR_DEFS[o->subtype].anims;
    func_8001FE6C(o);
    o->b04++;
}
