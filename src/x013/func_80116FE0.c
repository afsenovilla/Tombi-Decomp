// FUNC 80116fe0 2516 X013
// MATCHING 80116fe0 2516
#include "gv2.h"
#include "gvars.h"
#include "gamestate.h"
#include "gametask.h"
typedef struct { short x, y; } XY;
typedef struct { short op, arg; } CMD;
typedef struct O {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x20 - 8];
    short timer;
    char p22[0x90 - 0x22];
    struct O *d90;
    char p94[0xb4 - 0x94];
    unsigned short wb4, wb6;
} O;
typedef struct {
    unsigned char active, visible, type, subtype, b04, step, state, substep;
    char p08[0x12 - 8];
    short xw;
    char p14[0x2e - 0x14];
    unsigned short animFrame;
} PL;
typedef struct { char p[0x4a]; short w4a, w4c, w4e; } Q;

extern PL PLAYER;
extern CMD *D_801181FC[];
extern XY D_80118200[];
extern unsigned char D_8009D088[];
extern unsigned char D_8009C975x[];
#define D_8009C975 D_8009C975x[0]
extern unsigned char SCRIPT_FLAGS[];
extern int D_8011A8C0, D_8011A8C4, D_8011A8C8, D_8011A8CC;
extern unsigned char D_8009CE48, D_8009CDCB, D_8009CE0B;
extern unsigned short D_1F8001FC;
extern unsigned char D_1F8001CC, D_1F8001CD;
extern short D_1F8001C6;
extern char D_8001D6A4[];
extern void FUN_800eea7c(PL *, int, int);
extern O *showMessageBox(int, int, int, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);
extern void stopBgm(int);
extern void ThreadCreate(int, char *);
extern void startAreaBgm(void);
extern void func_801179B4(int, int, int, int);
extern void freeObjectLayer2(O *);

void func_80116FE0(O *o)
{
    CMD *c;
    PL *p;
    int i;
    unsigned int t, h;
    char pad[16];

    switch (o->b04) {
    case 0:
        o->wb4 = 0;
        o->wb6 = 0;
        o->active = 2;
        o->timer = 0;
        D_8009D088[0] = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        c = D_801181FC[o->wb4];
        c += o->wb6;
        p = &PLAYER;
        switch (c->op) {
        case 0:
            TASK_C->state1 = 2;
            TASK_C->mode = 0;
            TASK_C->step = 0;
            break;
        case 1:
            D_8009D088[0] = c->arg;
            o->wb6++;
            break;
        case 2:
            if (o->timer == 0) {
                o->timer = c->arg;
                break;
            }
            if (--o->timer != 0) break;
            o->step = 0;
            o->wb6++;
            break;
        case 3:
            switch (o->step) {
            case 0:
                if (*(unsigned short *)&p->b04 != 0x6505) break;
                GVARS.b3a = 1;
                GVARS.b3f = 1;
                o->timer = 1;
                o->step++;
                break;
            case 1:
                if (--o->timer != 0) break;
                PLAYER.b04 = 5;
                PLAYER.step = 0x64;
                PLAYER.state = 0;
                PLAYER.animFrame = 1;
                FUN_800eea7c(p, 0, 0);
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 4:
            o->d90 = showMessageBox(2, c->arg, D_80118200[D_8009D088[0] & 0xf].x, D_80118200[D_8009D088[0] & 0xf].y);
            o->wb6++;
            break;
        case 5:
            if (o->d90->b04 < 2) break;
            o->d90->b04++;
            o->wb6++;
            break;
        case 6:
            setEventStarted(c->arg, 0, 0);
            o->wb6++;
            break;
        case 7:
            setEventComplete(c->arg, 0);
            o->wb6++;
            break;
        case 8:
            break;
        case 9:
            D_8009C975 = 3;
            PLAYER.xw++;
            o->wb6++;
            break;
        case 10:
            PLAYER.xw++;
            if (D_8009C975 != 1) break;
            PLAYER.b04 = 5;
            PLAYER.step = 0x40;
            PLAYER.state = 0;
            PLAYER.visible = 0;
            o->wb6++;
            break;
        case 11:
            D_8009C975 = 4;
            o->wb6++;
            break;
        case 12:
            if (D_8009C975 != 0) break;
            o->wb6++;
            break;
        case 13:
            D_1F8001CC = 1;
            D_1F8001CD = 0x14;
            D_1F8001C6 = 2;
            stopBgm(0);
            ThreadCreate(1, D_8001D6A4);
            o->wb6++;
            break;
        case 14:
            startAreaBgm();
            o->wb6++;
            break;
        case 15:
            switch (o->step) {
            case 0:
                PLAYER.animFrame = 0;
                FUN_800eea7c(p, 1, 0);
                o->step++;
                break;
            case 1:
                if (++PLAYER.xw < 0x105) break;
                PLAYER.animFrame = 1;
                FUN_800eea7c(p, 0, 0);
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 16:
            switch (o->step) {
            case 0:
                PLAYER.animFrame = 0;
                FUN_800eea7c(p, 1, 0);
                o->step++;
                break;
            case 1:
                if (++PLAYER.xw < 0x14b) break;
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 17:
            switch (o->step) {
            case 0:
                t = (*(unsigned int*)&GAME.totalTimePlayed);
                h = t / 180000;
                D_8011A8C8 = 0;
                D_8011A8C4 = t / 3000 - h * 60;
                D_8011A8C0 = h % 1000;
                for (i = 0; i < 0x100; i++) {
                    if (i != 0x12 && SCRIPT_FLAGS[i]) D_8011A8C8++;
                }
                D_8011A8CC = 0;
                for (i = 0; i < 0x100; i++) {
                    if (i != 0x12 && SCRIPT_FLAGS[i] == 0xff) D_8011A8CC++;
                }
                o->step++;
            case 1:
                func_801179B4(D_8011A8C0, D_8011A8C4, D_8011A8C8, D_8011A8CC);
                if (!(D_1F8001FC & 0x6008)) break;
                if (D_8009CE48 == 0xff && D_8009CDCB == 0xff && GV2.sda != 0 && D_8009CE0B == 0xff) {
                    D_8009C975 = 3;
                    o->step = 0xb;
                    break;
                }
                o->timer = 1;
                o->step++;
                break;
            case 2:
                if (D_8009CE48 != 0xff) {
                    o->d90 = showMessageBox(3, 0, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 4;
                }
                break;
            case 3:
                if (--o->timer != 0 && o->d90->substep != 1) break;
                if (D_8009CDCB == 0xff && GV2.sda != 0 && D_8009CE0B == 0xff) {
                    o->timer = 1;
                    o->step = 9;
                    break;
                }
                o->d90->b04 = 3;
                o->step++;
                break;
            case 4:
                if (D_8009CDCB != 0xff) {
                    o->d90 = showMessageBox(3, 1, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 6;
                }
                break;
            case 5:
                if (--o->timer != 0 && o->d90->substep != 1) break;
                if (GV2.sda != 0 && D_8009CE0B == 0xff) {
                    o->timer = 1;
                    o->step = 9;
                    break;
                }
                o->d90->b04 = 3;
                o->step++;
                break;
            case 6:
                if (GV2.sda == 0) {
                    o->d90 = showMessageBox(3, 2, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 8;
                }
                break;
            case 7:
                if (--o->timer != 0 && o->d90->substep != 1) break;
                if (D_8009CE0B != 0xff) {
                    o->d90->b04 = 3;
                    o->step++;
                    break;
                }
                o->timer = 1;
                o->step = 9;
                break;
            case 8:
                if (D_8009CE0B != 0xff) {
                    o->d90 = showMessageBox(3, 3, 0xa0, 0xa0);
                    o->timer = 0x168;
                } else {
                    o->timer = 1;
                }
                o->step++;
                break;
            case 9:
                if (--o->timer != 0 && o->d90->substep != 1) break;
                o->d90->substep = 1;
                D_8009C975 = 3;
                o->step++;
                break;
            case 11:
                func_801179B4(D_8011A8C0, D_8011A8C4, D_8011A8C8, D_8011A8CC);
            case 10:
                if (D_8009C975 != 1) break;
                o->wb6++;
                break;
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
