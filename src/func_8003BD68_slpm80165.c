// VERSION SLPM_80165 slpm80165 8003BD68 704
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of updateL5Type09Item (src/updateL5Type09Item.c): the floor probe runs also while rising, with a fixed offset of 8, no section 3
// floor clamp, and the pick-up always sets the section flag; the reward handler index is a byte of the 20-byte
// item table.
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { short h; short v; } VT;
extern VT L5_TYPE09_ITEM_VEL_TABLE[];
typedef struct { unsigned char b0, handler; char pad[18]; } ItemDef;
extern ItemDef D_8006DDEC[];
extern void (*D_8006DDB8[])(TObj *);
extern void initItemObject(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjApplyVelocity(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern void AnimAdvance(TObj *);
extern void setSectionFlagBit(int);
extern void freeObjectLayer5(TObj *);

void func_8003BD68(TObj *o)
{
    switch (o->b04) {
    case 0:
        initItemObject(o);
        o->timer = 30;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        if (ObjCullRegister(o) == 0)
            break;
        switch (o->step) {
        case 0:
            ObjApplyVelocity(o);
            if (o->velV <= 0x800)
                o->velV += 0x20;
            if (o->b68 & 1) {
                o->b68 = 0;
                o->velH = L5_TYPE09_ITEM_VEL_TABLE[o->animFrame].h;
                o->velV = L5_TYPE09_ITEM_VEL_TABLE[o->animFrame].v;
            } else if (o->b69 & 1) {
                o->b69 = 0;
                if (o->velV <= 0x100) {
                    o->active = 2;
                    o->step++;
                } else {
                    o->velV = -o->velV / 4;
                }
            } else {
                if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8)) {
                    if (o->velV <= 0x100) {
                        o->active = 2;
                        o->step++;
                    } else {
                        o->velV = -o->velV / 4;
                    }
                }
            }
            break;
        case 1:
            if (o->b68 & 1) {
                o->b68 = 0;
                o->velH = L5_TYPE09_ITEM_VEL_TABLE[o->animFrame].h;
                o->velV = L5_TYPE09_ITEM_VEL_TABLE[o->animFrame].v;
                o->step = 0;
            }
            break;
        }
        AnimAdvance(o);
        if (o->timer && --o->timer == 0)
            o->active = 1;
        break;
    case 2:
        setSectionFlagBit(o->b6b);
        D_8006DDB8[D_8006DDEC[o->subtype].handler](o);
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
#endif
