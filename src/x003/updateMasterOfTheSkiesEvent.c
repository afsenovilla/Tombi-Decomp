// FUNC 8012a890 716 X003
// MATCHING 8012a890 716
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned char D_8009C941, D_8009D094;
extern unsigned char D_1F8003D0, MOVIE_PLAY_STATE, MOVIE_ID;
extern short D_1F8001C6;
extern TObj *TASK_C;
extern char D_8001D6A4[];
extern int ObjCullRegister(TObj *o);
extern void freeObjectLayer2(TObj *o);
extern void cdSeekStream(int);
extern void stopBgm(int);
extern void ThreadCreate(int, void *);
extern void removeItemFromInventory(int, int);
extern void initMasterOfTheSkiesEventObj(TObj *o);
extern void masterOfTheSkiesScene(TObj *o);

static __inline__ int chk(void)
{
    return GVARS.b40 == 1 && D_8009C941 == 0x42;
}

void updateMasterOfTheSkiesEvent(TObj *o)
{
    TObj *p;

    switch (o->b04) {
    case 0:
        initMasterOfTheSkiesEventObj(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->subtype) break;
        switch (o->step) {
        case 0:
            if ((GCD.bd004 && GCD.bcdc7 == 0xff) || chk()) {
                GCD.bd004 = 0;
                if (GCD.bcdc7 != 0xff) removeItemFromInventory(0x42, 5);
                o->step++;
                o->state = 0;
            }
            break;
        case 1:
            masterOfTheSkiesScene(o);
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            o->step++;
            o->state = 0;
            cdSeekStream(7);
            break;
        case 1:
            switch (o->state) {
            case 0:
                PLAYER.obj.b04 = 5;
                PLAYER.obj.step = 0x62;
                GVARS.b3f = 1;
                GAME.fadeScreenControl = 3;
                o->state++;
                break;
            case 1:
                if (GAME.fadeScreenControl == 1) {
                    if (D_8009D094 == 0) {
                        D_1F8003D0 = 0;
                        D_8009D094 = 1;
                    } else {
                        D_1F8003D0 = 1;
                    }
                    MOVIE_PLAY_STATE = 1;
                    MOVIE_ID = 7;
                    D_1F8001C6 = 2;
                    stopBgm(0);
                    ThreadCreate(1, D_8001D6A4);
                    o->step++;
                    o->state = 0;
                }
                break;
            }
            break;
        case 2:
            (*(short*)&GAME.nextArea) = 10;
            (*(short*)&GAME.nextSection) = 0;
            (*(short*)&GAME.nextSpawnPoint) = 0;
            p = TASK_C;
            p->w4c = 7;
            p->w4e = 0;
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
