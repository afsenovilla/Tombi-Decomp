// FUNC 800e900c 520 I0NFO3
// MATCHING 800e900c 520
#include "TOBJ.H"
#include "raw7.h"
#include "spad.h"
#include "gamestate.h"
extern void func_8001FE6C(TObj *o);
extern int func_8005EFE4(void);
extern void *D_800106B4;
extern char D_80011790[];
extern unsigned short D_8007A024[][2];
typedef struct { unsigned char b0; char p1[0x13]; unsigned short w14; char p16[0x12]; unsigned short w28, w2a, w2c, w2e; } P330;
extern P330 *D_8009C330;
extern unsigned char D_8009C990[];
extern unsigned char D_8009CEF7[];
extern unsigned char D_8009CF06[];
extern unsigned char D_8009D006[];
extern signed char D_8009D2B0[];
extern unsigned char D_8009D2B3[];
extern unsigned short D_800A6040[];
extern unsigned char D_800A60A3[];
extern unsigned char D_800A60FA[];
extern unsigned short D_800A60FC[];
extern unsigned char D_800EECE8[];
extern unsigned short D_8009C982;
extern unsigned char D_8009C970;

void func_800E900C(TObj *o)
{
    short pad[4];
    D_8009C330 = (P330 *)((char *)o + 0xF8);
    if (o->step == 0) {
        o->anim = D_80011790;
        func_8001FE6C(o);
        D_8009C330->w14 = (func_8005EFE4() == 1 || func_8005EFE4() == 2) ? 0 : 0;
        o->w1e = 0;
        D_800A60FA[0] = D_8009D2B3[0];
        D_800A6040[0] = D_8007A024[D_8009C990[0]][0];
        D_800A60FC[0] = D_8007A024[D_8009D2B3[0]][1];
        D_8009C330->w28 = 0xFFFF;
        D_8009C330->w2a = 0xFFFF;
        D_8009C330->w2e = 0xFFFF;
        D_8009C330->b0 = 0;
        o->visible = 1;
        o->b0a = 1;
        o->ba5 = 0;
        o->wb0 = 0;
        o->velH = 0;
        o->velV = 0;
        o->wb2 = 0;
        o->b6b = 1;
        o->b9d = 0;
        o->subtype = 0;
        D_8009CEF7[0] = 0;
        if (D_8009CF06[0] != 0) o->subtype = 1;
        if (D_8009D006[0] != 0) o->subtype = 2;
        o->animFrame = D_800EECE8[D_8009C982];
        if (GAME.selectedSpawnPoint != 0) o->animFrame = 4;
        o->w9a = D_8009C970;
        o->w98 = GAME.playerHealth;
        *(signed char *)&o->b0f = -8;
        o->d3c = SPAD.i2f0;
        o->anim = D_800106B4;
        func_8001FE6C(o);
        D_800A60A3[0] = 0;
        o->step = 0x63;
        o->b04 = 1;
        o->state = 0;
        o->animFrame = 6;
        if (D_8009D2B0[0] == 3) {
            o->b04 = 4;
            o->step = 5;
            o->state = 0;
            o->substep = 0;
        }
    }
}
