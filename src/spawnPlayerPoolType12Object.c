// FUNC 8002c654 232 MAIN0
// MATCHING 8002c654 232
#include "TOBJ.H"
extern unsigned char DAT_8009cef8;
extern TObj *ObjAlloc(void);
extern unsigned short GetClut(int, int);
extern unsigned short PLAYER_ANIM_FRAME[];
extern int PLAYER_POS[];
extern unsigned char PLAYER_DRAW_DEPTH[];
extern int DAT_1f8002cc;

void spawnPlayerPoolType12Object(void)
{
    TObj *o;
    int v;

    if (DAT_8009cef8 != 0 && (o = ObjAlloc()) != 0) {
        o->active = 1;
        o->type = 0x12;
        o->animFrame = PLAYER_ANIM_FRAME[0] & 1;
        o->a.raw = PLAYER_POS[0];
        o->y.raw = PLAYER_POS[1];
        o->b.raw = PLAYER_POS[2];
        o->w1e = 0;
        o->w08 = GetClut(0x80, 499);
        o->b0d = 1;
        o->b0a = 8;
        o->subtype = 1;
        o->b0f = PLAYER_DRAW_DEPTH[0] + 1;
        v = DAT_1f8002cc;
        o->category |= 0x80;
        o->b1d = 0x4d;
        o->d3c = v;
    }
}
