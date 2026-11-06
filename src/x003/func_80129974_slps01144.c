// VERSION SLPS_01144 x03 80129974 496
// FLAGS -O2 -G0
// JP-only: the "play movie `id` then warp to the area-10 start" tail of the event handlers (PAL inlines it in each
// handler, e.g. updateMasterOfTheSkiesEvent case 2).
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
#include "gamestate.h"

typedef struct { char p0[0x4c]; short w4c, w4e; } G;
extern unsigned char D_8009E4E7_[];
#define D_8009E4E7 D_8009E4E7_[0]
extern unsigned char D_8009E51D_[];
#define D_8009E51D D_8009E51D_[0]
extern unsigned char D_800A7B7E_[];
#define D_800A7B7E D_800A7B7E_[0]
extern unsigned char D_800A7B7D_[];
#define D_800A7B7D D_800A7B7D_[0]
extern unsigned char D_800A7B7C_[];
#define D_800A7B7C D_800A7B7C_[0]
extern unsigned char D_1F8001CD_[];
#define D_1F8001CD D_1F8001CD_[0]
extern unsigned char D_1F8001CC_[];
#define D_1F8001CC D_1F8001CC_[0]
extern short D_1F8001C6_[];
#define D_1F8001C6 D_1F8001C6_[0]
extern short D_800A7C2A_[];
#define D_800A7C2A D_800A7C2A_[0]
extern G *D_1F8001D4;
extern void func_8001D584(int);
extern void stopBgm(int);
extern void ThreadCreate(int, void *);
extern void startAreaBgm(void);
extern void func_8001C988(int, int);
extern void func_8001D5E0_slps01144(void);

void func_80129974_slps01144(TObj *o, int id)
{
    G *t;

    switch (o->step) {
    case 0:
        o->step++;
        o->state = 0;
        break;
    case 1:
        switch (o->state) {
        case 0:
            func_8001D584((short)id);
            D_800A7B7C = 5;
            D_800A7B7D = 98;
            D_8009E4E7 = 1;
            D_8009E51D = 3;
            o->state++;
            break;
        case 1:
            if (D_8009E51D == 1) {
                D_1F8001CC = 1;
                D_1F8001CD = id;
                D_1F8001C6 = 2;
                stopBgm(0);
                ThreadCreate(1, func_8001D5E0_slps01144);
                o->step = 10;
                o->state = 0;
            }
            break;
        }
        break;
    case 2:
        switch (o->state) {
        case 0:
            D_8009E51D = 4;
            startAreaBgm();
            o->state++;
        case 1:
            if (D_8009E51D == 0) {
                D_800A7C2A = 0;
                D_800A7B7C = 1;
                D_800A7B7D = 0;
                D_800A7B7E = 0;
                D_8009E4E7 = 0;
                func_8001C988(22, 0);
                o->step = 0;
            }
            break;
        }
    case 10:
        t = D_1F8001D4;
        GAME.nextArea = 10;
        GAME.nextSection = 0;
        GAME.nextSpawnPoint = 0;
        t->w4c = 7;
        t->w4e = 0;
        break;
    }
}
#endif
