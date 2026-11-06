// JP-only (x04), written from the disassembly
// VERSION SLPS_01144 x04 8011A6BC 1256
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned short SPAWN_AXIS_SWAP;
extern void func_800182C4(TObj *);
extern void func_80017E0C(TObj *);
extern void playSFX(int);
extern void func_8001C88C(int, int, int);
extern void func_8001C988(int, int);
extern int func_80119DD8(TObj *);
extern int func_8011A048(TObj *);
extern void func_80119F68_slps01144(TObj *);
extern void func_8011A268(TObj *);
extern void func_8011D598_slps01144(TObj *);
extern unsigned char D_8009EC82[], D_8009EC81[], D_800A7C23[];

#define S172 (*(short*)((char*)&SPAD.i170 + 2))
#define DONE(id, cond) \
    if (cond) func_8001C988(id, 0); \
    else { func_8001C88C(id, 0, 0); o->state = 0; }

void func_8011A6BC_slps01144(TObj *o)
{
    int r;
    short *p;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->d84 = 0;
        o->d8c = 0;
        switch (o->animFrame) {
        case 0:
            o->d88 = 0;
            break;
        case 1:
            o->d88 = 0x400;
            break;
        case 2:
            o->d88 = 0x800;
            break;
        }
        if (o->subtype == 0x16 || o->subtype == 0x17)
            *(signed char *)&o->b0f = -2;
        else
            o->b0f = 4;
        break;
    case 1:
        if (o->subtype < 22) {
            func_8011D598_slps01144(o);
        } else {
            o->visible = 1;
            func_800182C4(o);
        }
        switch (o->subtype) {
        case 4:
            if (SPAWN_AXIS_SWAP & 1) break;
            r = func_80119DD8(o);
            if (r == 1 && S172 == 900) {
                DONE(51, D_8009EC82[0] != 0)
            } else if (r == 2) {
                playSFX(0x7e);
            }
            break;
        case 2:
        case 6:
        case 14:
        case 16:
        case 18:
        case 20:
            if (func_80119DD8(o) == 2) playSFX(0x7e);
            break;
        case 3:
        case 5:
        case 7:
        case 15:
        case 17:
        case 19:
        case 21:
            func_80119F68_slps01144(o);
            break;
        case 0:
            if (SPAWN_AXIS_SWAP & 1) break;
            p = &S172;
            if (*p >= 900) break;
            r = func_8011A048(o);
            if (r == 1 && *p == 810) {
                DONE(41, D_800A7C23[0] == r)
            } else if (r == 2) {
                playSFX(0x80);
            }
            break;
        case 10:
            if (!(SPAWN_AXIS_SWAP & 1)) break;
            r = func_8011A048(o);
            if (r == 1 && S172 == 1350) {
                DONE(42, D_800A7C23[0] == 2)
            } else if (r == 2) {
                playSFX(0x81);
            }
            break;
        case 8:
            if (SPAWN_AXIS_SWAP & 1) break;
            r = func_8011A048(o);
            if (r == 1 && S172 == 450) {
                DONE(55, D_8009EC81[0] != 0)
            } else if (r == 2) {
                playSFX(0x7f);
            }
            break;
        case 12:
            if (SPAWN_AXIS_SWAP & 1) break;
            if (func_8011A048(o) == 2) playSFX(0x7f);
            break;
        case 1:
        case 9:
        case 11:
        case 13:
            switch (o->state) {
            case 0:
                o->state = ((TObj *)o->d90)->state;
                o->d30 = 0;
                if (o->state == 3) {
                    o->d30 = 0;
                    o->h->p.whole += 0x20;
                }
                break;
            case 1:
                o->h->raw += 0x18000;
                o->d30 += 0x18000;
                if ((o->d30 >> 16) >= 0x20) o->state++;
                break;
            case 2:
                break;
            case 3:
                o->h->raw += -0x18000;
                o->d30 += -0x18000;
                if ((o->d30 >> 16) < -0x1f) o->state++;
                break;
            case 4:
                o->state = ((TObj *)o->d90)->state;
                break;
            }
            break;
        case 22:
        case 23:
            func_8011A268(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        func_80017E0C(o);
        break;
    }
}
#endif
