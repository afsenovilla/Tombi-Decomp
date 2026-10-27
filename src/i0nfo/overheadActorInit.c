// FUNC 800ed7b0 352 I0NFO
// MATCHING 800ed7b0 352
#include "TOBJ.H"
typedef struct {
    unsigned char box[4];
    unsigned short w1e;
    short spad;
    void **anims;
} ObjInit;
extern ObjInit OVERHEAD_ACTOR_DEFS[];
extern int SPR_DATA[];
extern void func_8001FE94(TObj *o, int n);
extern void func_800202B4(TObj *o);

void overheadActorInit(TObj *o)
{
    o->box0 = OVERHEAD_ACTOR_DEFS[o->subtype].box[0];
    o->box1 = OVERHEAD_ACTOR_DEFS[o->subtype].box[1];
    o->box2 = OVERHEAD_ACTOR_DEFS[o->subtype].box[2];
    o->box3 = OVERHEAD_ACTOR_DEFS[o->subtype].box[3];
    o->w1e = OVERHEAD_ACTOR_DEFS[o->subtype].w1e;
    o->d3c = SPR_DATA[OVERHEAD_ACTOR_DEFS[o->subtype].spad];
    o->anim = OVERHEAD_ACTOR_DEFS[o->subtype].anims[o->w74];
    func_8001FE94(o, o->w76);
    o->w08 = 0x7809;
    o->d8c = 0;
    o->b0d = 0;
    o->category |= 0x80;
    o->b04++;
    func_800202B4(o);
}
