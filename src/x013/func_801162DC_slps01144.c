// VERSION SLPS_01144 x13 801162DC 2196
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
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
extern CMD *D_80116C48[];
extern XY D_80116C4C[];
typedef struct { unsigned char b[4]; } S4;
extern S4 D_8009EC30;
extern unsigned char D_8009E4E2, D_8009E4E7;
extern S4 D_800A7B79;
extern S4 D_8009E51D;
extern unsigned char D_8009E94C[];
extern unsigned char D_8009E9F0, D_8009E973, D_8009E9B3;
extern unsigned short D_8009EE82;
extern unsigned int D_8009E510, D_8009E514;
extern char D_800A3D68[];
extern char D_80115210[], D_80115220[], D_8011522C[];
extern int D_80116E50, D_80116E54, D_80116E58, D_80116E5C;
extern char D_8001D5E0[];
extern void playerSetAnim(PL *, int, int);
extern O *func_8002E3B4(int, int, int, int);
extern void func_8001C88C(int, int, int);
extern void func_8001C988(int, int);
extern void func_8001F050(int);
extern void func_80016740(int, char *);
extern void func_8001EAD0(void);
extern void func_80017D64(O *);
extern int func_8002122C(O *);
extern void func_80020E38(O *);
extern int func_80059270(char *, char *, ...);
extern void func_80021E5C(int, int, int, char *);

void func_801162DC_slps01144(O *o)
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
        D_8009EC30.b[0] = 0;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        c = D_80116C48[o->wb4];
        c += o->wb6;
        p = &PLAYER;
        switch (c->op) {
        case 0:
            break;
        case 1:
            D_8009EC30.b[0] = c->arg;
            o->wb6++;
            break;
        case 2:
            if (o->timer == 0) {
                o->timer = c->arg;
                break;
            }
            if (--o->timer == 0) goto adv;
            break;
        case 3:
            switch (o->step) {
            case 0:
                if (*(unsigned short *)&p->b04 != 0x6505) break;
                D_8009E4E2 = 1;
                D_8009E4E7 = 1;
                o->timer = 1;
                o->step++;
                break;
            case 1:
                if (--o->timer != 0) break;
                PLAYER.b04 = 5;
                PLAYER.step = 0x64;
                PLAYER.state = 0;
                PLAYER.animFrame = 1;
                playerSetAnim(p, 0, 0);
                goto adv;
            }
            break;
        case 4:
            o->d90 = func_8002E3B4(2, c->arg, D_80116C4C[D_8009EC30.b[0] & 0xf].x, D_80116C4C[D_8009EC30.b[0] & 0xf].y);
            o->wb6++;
            break;
        case 5:
            if (o->d90->b04 < 2) break;
            o->d90->b04++;
            o->wb6++;
            break;
        case 6:
            func_8001C88C(c->arg, 0, 0);
            o->wb6++;
            break;
        case 7:
            func_8001C988(c->arg, 0);
            o->wb6++;
            break;
        case 8:
            break;
        case 9:
            D_8009E51D.b[0] = 3;
            PLAYER.xw++;
            o->wb6++;
            break;
        case 10:
            PLAYER.xw++;
            if (D_8009E51D.b[0] != 1) break;
            PLAYER.b04 = 5;
            PLAYER.step = 0x40;
            PLAYER.state = 0;
            D_800A7B79.b[0] = 0;
            o->wb6++;
            break;
        case 11:
            D_8009E51D.b[0] = 4;
            o->wb6++;
            break;
        case 12:
            if (D_8009E51D.b[0] != 0) break;
            o->wb6++;
            break;
        case 13:
            SPAD.b1cc = 1;
            SPAD.b1cd = 0x14;
            SPAD.s1c6 = 2;
            func_8001F050(0);
            func_80016740(1, D_8001D5E0);
            o->wb6++;
            break;
        case 14:
            func_8001EAD0();
            o->wb6++;
            break;
        case 15:
            switch (o->step) {
            case 0:
                PLAYER.animFrame = 0;
                playerSetAnim(p, 1, 0);
                o->step++;
                break;
            case 1:
                if (++PLAYER.xw < 0x105) break;
                { int z = 0; asm volatile("" : "=r"(z) : "0"(z));
                PLAYER.animFrame = 1;
                playerSetAnim(p, z, 0); }
                goto adv;
            }
            break;
        case 16:
            switch (o->step) {
            case 0:
                PLAYER.animFrame = 0;
                playerSetAnim(p, 1, 0);
                o->step++;
                break;
            case 1:
                if (++PLAYER.xw < 0x14b) break;
            adv:
                o->step = 0;
                o->wb6++;
                break;
            }
            break;
        case 17:
            switch (o->step) {
            case 0:
                t = D_8009E510;
                h = t / 216000;
                D_80116E58 = 0;
                D_80116E54 = t / 3600 - h * 60;
                D_80116E50 = h % 1000;
                for (i = 0; i < 0x100; i++) {
                    if (i != 0x12 && D_8009E94C[i]) D_80116E58++;
                }
                D_80116E5C = 0;
                for (i = 0; i < 0x100; i++) {
                    if (i != 0x12 && D_8009E94C[i] == 0xff) D_80116E5C++;
                }
                o->timer = 600;
                o->step++;
            case 1:
                func_80059270(D_800A3D68, D_80115210, D_80116E50, D_80116E54);
                func_80021E5C(32, 64, 0, D_800A3D68);
                func_80059270(D_800A3D68, D_80115220, D_8009E514);
                func_80021E5C(32, 88, 0, D_800A3D68);
                func_80059270(D_800A3D68, D_8011522C, D_80116E5C, D_80116E58);
                func_80021E5C(32, 112, 0, D_800A3D68);
                if (!(SPAD.s1fc & 0x800)) break;
                o->step++;
                break;
            case 2:
                if (D_8009E9F0 != 0xff) {
                    o->d90 = func_8002E3B4(3, 0, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 4;
                }
                break;
            case 3:
            case 5:
            case 7:
                if (--o->timer != 0 && o->d90->b04 != 2) break;
                o->d90->b04 = 3;
                o->step++;
                break;
            case 4:
                if (D_8009E973 != 0xff) {
                    o->d90 = func_8002E3B4(3, 1, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 6;
                }
                break;
            case 6:
                if (D_8009EE82 == 0) {
                    o->d90 = func_8002E3B4(3, 2, 0xa0, 0xa0);
                    o->timer = 0x168;
                    o->step++;
                } else {
                    o->step = 8;
                }
                break;
            case 8:
                if (D_8009E9B3 != 0xff) {
                    o->d90 = func_8002E3B4(3, 3, 0xa0, 0xa0);
                    o->timer = 0x168;
                }
                o->step++;
                break;
            case 9:
                if (--o->timer != 0 && o->d90->b04 != 2) break;
                o->d90->b04 = 3;
                TASK_C->state1 = 2;
                TASK_C->mode = 0;
                TASK_C->step = 0;
                break;
            }
            break;
        }
        if (func_8002122C(o)) func_80020E38(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80017D64(o);
        break;
    }
}
#endif
