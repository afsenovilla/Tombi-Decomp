// FUNC 800e8644 604 I0NFO
// MATCHING 800e8644 604
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE6C(TObj *o);
extern int GetGraphType(void);
extern void overheadSpawnPlayerShadow(TObj *o, int a);
extern void overheadSpawnFollower(TObj *o);
extern void *D_800106B4;
extern char D_80011790[];
typedef struct { unsigned short a, b; } Pair7A;
extern Pair7A D_8007A024[];
extern TObj *PLAYER_EXT;
extern unsigned char D_8009C970;
typedef struct { unsigned char c; } C970S; typedef struct { int v; } SPV;
extern unsigned short D_8009C982[];
extern unsigned char D_8009C990[];
extern unsigned char D_8009CEAB;
extern unsigned char D_8009CF06;
extern unsigned char D_8009D006;
extern signed char GAME_PLAYER_MODE;
extern unsigned char D_8009D2B3[];
extern unsigned short D_800A6040[];
extern unsigned char D_800A60A3;
extern unsigned char D_800A60FA[];
extern unsigned short D_800A60FC[];

void overheadPlayerInit(TObj *o)
{
    TObj *p;
    void *an;
    int pad[2];
    PLAYER_EXT = (TObj *)((char *)o + 0xF8);
    if (o->step == 0) {
        o->anim = D_80011790;
        func_8001FE6C(o);
        p = PLAYER_EXT;
        if (GetGraphType() != 1) {
            GetGraphType();
        }
        S16(p, 0x14) = 0;
        o->w1e = 0;
        D_800A60FA[0] = D_8009D2B3[0];
        D_800A6040[0] = D_8007A024[D_8009C990[0]].a;
        D_800A60FC[0] = D_8007A024[D_8009D2B3[0]].b;
        U16(PLAYER_EXT, 0x28) = 0xFFFF;
        U16(PLAYER_EXT, 0x2A) = 0xFFFF;
        PLAYER_EXT->animFrame = 0xFFFF;
        PLAYER_EXT->active = 0;
        o->visible = 1;
        o->b0a = 1;
        o->ba5 = 0;
        o->wb0 = 0;
        o->velH = 0;
        o->velV = 0;
        o->wb2 = 0;
        o->b6b = 1;
        o->b9d = 0;
        if (D_8009C982[0] != 0 && D_8009C982[0] != 2) {
            o->animFrame = 4;
        }
        if (D_8009CF06 != 0) {
            o->subtype = 1;
        }
        if (D_8009D006 != 0) {
            o->subtype = 2;
        }
        S16(o, 0x9A) = D_8009C970;
        o->w98 = ((C970S *)&D_8009C970)->c;
        *(signed char *)&o->b0f = -8;
        an = D_800106B4;
        o->d3c = ((SPV *)0x1F8002F0)->v;
        o->anim = an;
        func_8001FE6C(o);
        overheadSpawnPlayerShadow(o, 1);
        if (D_8009CEAB == 3) {
            overheadSpawnFollower(o);
        }
        D_800A60A3 = 0;
        o->b04 = 1;
        o->step = 0x63;
        o->state = 0;
        switch (GAME_PLAYER_MODE) {
        case 4:
            o->h->p.whole += 8;
            o->y.p.whole += 8;
            break;
        case 3:
            o->h->p.whole = -0x49;
            o->animFrame = 6;
            o->b04 = 4;
            o->step = 5;
            o->state = 0;
            o->substep = 0;
            break;
        }
    }
}
