// FUNC 800ed2d0 1248 I0NFO
// MATCHING 800ed2d0 1248
#include "TOBJ.H"
typedef struct { short s[6]; } Pos6;
extern void func_80018C68(TObj *o);
extern void func_8001FEC0(TObj *o);
extern void func_800202B4(TObj *o);
extern TObj *func_8002DCF0(int a, int b, Pos6 *p, int c);
extern void overheadCollideMapTile(TObj *o, int x, int y, int z);
extern void overheadActorWalkStep(TObj *o);
extern void overheadActorWalkStepB(TObj *o);
extern unsigned char D_8009C93F[];
extern unsigned char D_8009C942[];
extern signed char GAME_PLAYER_MODE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned char D_800A60A3;
extern unsigned char D_800A60E4[];

static __inline__ void upd(TObj *o)
{
    if (GAME_PLAYER_MODE == 3) {
        if (o->b6b != 0) {
            func_80018C68(o);
            o->visible = 1;
        }
    } else {
        func_800202B4(o);
    }
}

void overheadActorStep(TObj *o)
{
    Pos6 p;
    TObj *n;
    switch (o->step) {
    case 0:
        upd(o);
        if (o->visible != 0) func_8001FEC0(o);
        break;
    case 1:
        upd(o);
        if (o->visible != 0) {
            switch (o->state) {
            case 0:
                o->velX = 0;
                o->velY = 0;
                o->velH = 0;
                o->velV = 0;
                o->state++;
            case 1:
                if (D_8009C942[0] == 0) {
                    func_8001FEC0(o);
                    if (--o->timer == 0) {
                        o->b6a = 0;
                        o->step = 0;
                        o->state = 0;
                    }
                }
                break;
            }
        }
        break;
    case 2:
        upd(o);
        if (o->visible != 0) overheadActorWalkStep(o);
        break;
    case 3:
        upd(o);
        if (o->visible != 0) {
            switch (o->state) {
            case 0:
                p = *(Pos6 *)&o->a;
                p.s[3] -= 0x40;
                o->d90 = (int)func_8002DCF0(o->w98, o->w9a, &p, o->timer);
                o->state++;
                break;
            case 1:
                func_8001FEC0(o);
                if (--o->timer == 0) {
                    D_8009C93F[0] = 0;
                    o->b6a = 0;
                    o->step = 0;
                    o->state = 0;
                }
                break;
            }
        }
        break;
    case 4:
        upd(o);
        if (o->visible != 0) {
            switch (o->state) {
            case 0:
                o->state++;
            case 1:
                func_8001FEC0(o);
                n = (TObj *)o->d90;
                if (n->b04 == 2) {
                    n->b04 = 3;
                    D_8009C942[0] = 0;
                    D_8009C93F[0] = 0;
                    D_800A60E4[0] = 0;
                    if (!(o->w74 & 0x80)) {
                        PLAYER_SUBSTATE = 0;
                        PLAYER_STEP = 0;
                    }
                    o->b68 = 0;
                    o->b6a = 0;
                    o->step = 0;
                    o->state = 0;
                }
                break;
            }
        }
        break;
    case 5:
        if (GAME_PLAYER_MODE == 3 && o->b6b != 0) {
            func_80018C68(o);
            o->visible = 1;
        }
        overheadActorWalkStepB(o);
        break;
    case 6:
        o->visible = 0;
        if (GAME_PLAYER_MODE == 3 && D_800A60A3 != 1 && o->b6b != 0) {
            func_80018C68(o);
            o->visible = 1;
        }
        break;
    case 9:
        upd(o);
        if (o->visible != 0) {
            func_8001FEC0(o);
            overheadCollideMapTile(o, o->h->p.whole, o->y.p.whole, o->animFrame * 2);
        }
        break;
    }
}
