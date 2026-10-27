// FUNC 800f2724 380 X019
// MATCHING 800f2724 380
#include "TOBJ.H"
typedef struct { short x, y; } XY;
extern TObj *PLAYER_EXT;
extern short FREE_OBJECT_COUNT;
extern unsigned short TICK_COUNT;
extern XY PLAYER_FOOT_OFFSET_BY_DIR[];
extern void FUN_800efc04(TObj *, TObj *);
extern void FUN_8001fe94(TObj *, int);
extern short spawnPlayerFootstepEffect(void);
extern TObj *FUN_80018448(void);
extern void playerAirSpinToFall(TObj *);
extern void playerAirAccelerateX(TObj *);
extern void FUN_8010f0f4(TObj *);
extern void FUN_8001fd94(TObj *);

void playerUpdateFall(TObj *o)
{
    TObj *p = PLAYER_EXT;
    TObj *q;
    short x, y, z;
    XY *t;
    p->animTimer = 4;
    if (p->animFrame != 4) {
        p->animTimer = 4;
        FUN_800efc04(o, p);
        FUN_8001fe94(o, 0);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
    }
    if (o->ba7 != 0) {
        x = o->a.p.whole;
        y = o->y.p.whole;
        z = o->b.p.whole;
        if (spawnPlayerFootstepEffect() == 0 && FREE_OBJECT_COUNT >= 6 && (q = FUN_80018448()) != 0) {
            q->active = 1;
            q->type = 0x31;
            q->subtype = 1;
            t = &PLAYER_FOOT_OFFSET_BY_DIR[TICK_COUNT & 7];
            q->a.p.whole = x;
            q->y.p.whole = y;
            q->b.p.whole = z;
            q->h->p.whole += t->x;
            q->y.p.whole += t->y;
        }
    }
    playerAirSpinToFall(o);
    playerAirAccelerateX(o);
    o->h->raw += o->velX << 8;
    FUN_8010f0f4(o);
    FUN_8001fd94(o);
}
