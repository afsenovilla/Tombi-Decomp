// FUNC 800e900c 520 I0NFO3
// MATCHING 800e900c 520
#include "TOBJ.H"
#include "raw7.h"
#include "spad.h"
#include "gamestate.h"
extern void func_8001FE6C(TObj *o);
extern int func_8005EFE4(void);
extern void *MAP_PLAYER_IDLE_ANIM_D2;
extern char D_80011790[];
extern unsigned short D_8007A024[][2];
typedef struct { unsigned char b0; char p1[0x13]; unsigned short w14; char p16[0x12]; unsigned short w28, w2a, w2c, w2e; } P330;
extern P330 *PLAYER_EXT;
extern unsigned char D_8009C990[];
extern unsigned char D_8009CEF7[];
extern unsigned char D_8009CF06[];
extern unsigned char D_8009D006[];
extern signed char GAME_PLAYER_MODE[];
extern unsigned char GAME_EQUIPPED_PANTS[];
extern unsigned short D_800A6040[];
extern unsigned char D_800A60A3[];
extern unsigned char D_800A60FA[];
extern unsigned short D_800A60FC[];
extern unsigned char TUNNEL_SPAWN_POINT_DIRS[];
extern unsigned short GAME_SELECTED_SPAWN_POINT;
extern unsigned char GAME_PLAYER_HEALTH;

void tunnelPlayerInit(TObj *o)
{
    short pad[4];
    PLAYER_EXT = (P330 *)((char *)o + 0xF8);
    if (o->step == 0) {
        o->anim = D_80011790;
        func_8001FE6C(o);
        PLAYER_EXT->w14 = (func_8005EFE4() == 1 || func_8005EFE4() == 2) ? 0 : 0;
        o->w1e = 0;
        D_800A60FA[0] = GAME_EQUIPPED_PANTS[0];
        D_800A6040[0] = D_8007A024[D_8009C990[0]][0];
        D_800A60FC[0] = D_8007A024[GAME_EQUIPPED_PANTS[0]][1];
        PLAYER_EXT->w28 = 0xFFFF;
        PLAYER_EXT->w2a = 0xFFFF;
        PLAYER_EXT->w2e = 0xFFFF;
        PLAYER_EXT->b0 = 0;
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
        o->animFrame = TUNNEL_SPAWN_POINT_DIRS[GAME_SELECTED_SPAWN_POINT];
        if (GAME.selectedSpawnPoint != 0) o->animFrame = 4;
        o->w9a = GAME_PLAYER_HEALTH;
        o->w98 = GAME.playerHealth;
        *(signed char *)&o->b0f = -8;
        o->d3c = SPAD.i2f0;
        o->anim = MAP_PLAYER_IDLE_ANIM_D2;
        func_8001FE6C(o);
        D_800A60A3[0] = 0;
        o->step = 0x63;
        o->b04 = 1;
        o->state = 0;
        o->animFrame = 6;
        if (GAME_PLAYER_MODE[0] == 3) {
            o->b04 = 4;
            o->step = 5;
            o->state = 0;
            o->substep = 0;
        }
    }
}
