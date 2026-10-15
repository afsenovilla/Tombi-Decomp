// FUNC 8011d9d0 320 X014
// MATCHING 8011d9d0 320
#include "spad.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    short wb4, wb6, wb8, wba, wbc;
    unsigned char bbe, bbf;
    short wc0, wc2, wc4, wc6;
} SB;
extern int D_80125CC4[], D_80125CE4[];
static int D_80125D04[7] = {
    7, 6, 8, 9, 8, 100729856, 328450,
};
extern unsigned char SCRIPT_FLAGS[];
#define S16(o, x) (*(short *)((char *)(o) + (x)))
#define S32(o, x) (*(int *)((char *)(o) + (x)))
#define U8(o, x) (*(unsigned char *)((char *)(o) + (x)))

static __inline__ void setRank(SB *s)
{
    int i, n;

    i = 0;
    n = 0;
    for (; i < 0x100; i++) {
        if (SCRIPT_FLAGS[i] == 0xff) n++;
    }
    if (n < 0x33) i = 7;
    else if (n < 0x3d) i = 6;
    else if (n < 0x47) i = 5;
    else if (n < 0x51) i = 4;
    else if (n < 0x5b) i = 3;
    else if (n < 0x65) i = 2;
    else i = n < 0x6f;
    s->wc4 = i;
}

void initEvilPigBoss(TObj *t)
{
    S16(t, 0xc0) = 0;
    S16(t, 0xc2) = 0;
    S16(t, 0xc4) = 0;
    S16(t, 0xc6) = 0;
    S16(t, 0x1e) = 4;
    t->d3c = SPAD.i2d0;
    t->wb4 = 0;
    t->wae = 0;
    t->d84 = 0;
    t->d88 = 0;
    t->d8c = 0;
    t->b0d = 0;
    t->b6a = 0;
    (*(int*)&t->wa8) = D_80125CC4[t->type];
    t->d90 = D_80125CE4[t->type];
    t->d94 = D_80125D04[t->type];
    (*(int*)&t->anim) = **(int **)((char *)t + 0xa8);
    if (GAME.selectedSection != 7) return;
    setRank((SB *)&t->wb4);
}
