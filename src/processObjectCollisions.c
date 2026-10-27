// FUNC 80047af0 900 MAIN0
// MATCHING 80047af0 900
#include "camera.h"
#include "spad.h"
#include "gvars.h"
#include "gamestate.h"
typedef struct {
    unsigned char b0; char p1[3]; unsigned char b4, b5, b6;
    char p7[0x16 - 7]; short s16;
    char p18[0x9e - 0x18]; unsigned char b9e;
} TO;
typedef struct { char p[0x4c]; unsigned short s4c; } C;
extern unsigned short D_8009C960_h;
extern short *PLAYER_ANIM;
extern unsigned short D_80115320[];
extern unsigned short D_800A60A4, D_800A60A6, D_800A60A8, D_800A60AA;
extern TO PLAYER;
extern signed char GAME_PLAYER_MODE;
extern C *TASK_C;
extern void func_8011E514(void), collideCat2Type7WithCat5Type0B(void), collideCat2ObjectsPairwise(void), collideCat2WithCat4Objects(void);
extern void collidePlayerWithCat4Objects(TO *), collidePlayerWithCat5Object(TO *, unsigned char *), collidePlayerWithCat2Objects(TO *), collideArea0Section2Slope(TO *);
extern void collideWeaponsWithCat2Objects(void), collideWeaponsWithCat5Objects(void), collideWeaponsWithCat4Objects(void), collideCat5WithCat4Objects(void);

static __inline__ void collide(TO *p)
{
    unsigned char **q;
    unsigned char *e;
    if (GVARS.b38 == 0 && GAME_PLAYER_MODE != 3) {
        q = (*(unsigned char ***)&SPAD.p228);
        for (SPAD.s19e = SPAD.s248; SPAD.s19e != 0;) {
            e = *q;
            SPAD.s19e--;
            q++;
            if (*e & 3) collidePlayerWithCat5Object(p, e);
        }
    }
}

void processObjectCollisions(void)
{
    unsigned short *t;
    TO *p;

    if (SPAD.b1a5 != 0) return;
    if ((*(int*)&GAME.selectedArea) == 6) {
        func_8011E514();
        return;
    }
    t = &D_80115320[PLAYER_ANIM[1] * 4];
    D_800A60A4 = *t++;
    D_800A60A6 = *t++;
    D_800A60A8 = *t++;
    D_800A60AA = *t++;
    collideCat2Type7WithCat5Type0B();
    collideCat2ObjectsPairwise();
    collideCat2WithCat4Objects();
    p = &PLAYER;
    switch (p->b0) {
    case 0: case 5:
    default:
        goto dflt;
    case 1: case 2: case 3: case 7:
        switch (p->b9e) {
        case 0: case 5: case 6:
            collidePlayerWithCat4Objects(p);
        }
        collide(p);
        if (GVARS.b42 | GVARS.b3f) goto tail;
        collidePlayerWithCat2Objects(p);
        break;
    case 4: case 6:
        collide(p);
        if (GVARS.b42 | GVARS.b3f) goto tail;
        break;
    }
    if (SPAD.b1a4 == 0 && TASK_C->s4c == 1 && GAME_PLAYER_MODE != 3 && CAMERA_SCROLL.maxY + 0xa0 < p->s16) {
        p->b0 = 5;
        p->b4 = 2;
        p->b5 = 3;
        p->b6 = 0;
    }
dflt:
    if (!(GVARS.b42 | GVARS.b3f) && D_8009C960_h == 0 && GAME.selectedSection == 2)
        collideArea0Section2Slope(p);
tail:
    collideWeaponsWithCat2Objects();
    collideWeaponsWithCat5Objects();
    collideWeaponsWithCat4Objects();
    collideCat5WithCat4Objects();
}
