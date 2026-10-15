// FUNC 80117148 800 X006
// MATCHING 80117148 800
#include "gcd.h"
#include "gv2.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct { short m[3][3]; short pad; int t[3]; } MAT;
typedef struct { short vx, vy, vz, pad; } SVEC;
extern MAT D_1F800000, D_1F800020;
extern SVEC D_1F800060;
extern int D_1F8002E0[];
extern int D_1F800168[], D_1F80016C[], D_1F800170[];
extern char D_80122A08[];
extern char D_80122A1C[];
extern char D_80122A30[];
extern char D_80122A44[];
extern char D_80122A98[];
extern char D_80122AA0[];
extern char D_80122A4C[];
extern char D_80122AA8[];
extern char D_80122AB0[];
static void *D_801229A8[9] = {
    D_80122A08, D_80122A1C, D_80122A30, D_80122A44,
    D_80122A98, D_80122AA0, D_80122A4C, D_80122AA8,
    D_80122AB0,
};
extern void *D_8011F2DC, *D_8011F2F0, *D_8011F2CC[], *D_8011F2E0[];
extern unsigned char PLAYER[];
extern short D_800A60A4[], D_800A60A6[], D_800A60A8[], D_800A60AA[];
extern int D_800A6048[], D_800A604C[], D_800A6050[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);
extern void loadImageRect(void *, int, int, int, int);
extern void getBaseMatrix(MAT *);
extern void MulMatrix0(MAT *, MAT *, MAT *);
extern void ApplyRotMatrix(SVEC *, int *);

void updateL3Type3BObject(TObj *o)
{
    TObj *p;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 1;
        o->b0d = 0;
        o->wac = 0;
        o->d3c = D_1F8002E0[0];
        o->b0a = 3;
        o->d64 = 0xe80;
        *(signed char *)&o->b0f = -4;
        o->d8c = 0;
        o->animFrame = 0;
        o->anim = D_801229A8[0];
        AnimLoadDuration(o);
        if (GCD.bcf06 || GCD.bd006) {
            loadImageRect(D_8011F2DC, 0x90, 0x1f0, 0x10, 1);
            loadImageRect(D_8011F2F0, 0x90, 0x1f1, 0x10, 1);
        } else {
            loadImageRect(D_8011F2CC[GAME.displayExpBar], 0x90, 0x1f0, 0x10, 1);
            loadImageRect(D_8011F2E0[GV2.bb3], 0x90, 0x1f1, 0x10, 1);
        }
        break;
    case 1:
        ObjCullRegister(o);
        AnimAdvance(o);
        p = (TObj *)o->d90;
        getBaseMatrix(&D_1F800000);
        D_1F800060.vx = o->d30;
        D_1F800060.vy = o->d34;
        D_1F800060.vz = o->d38;
        MulMatrix0((MAT *)((char *)p + 0x48), &D_1F800000, &D_1F800020);
        ApplyRotMatrix(&D_1F800060, D_1F800020.t);
        D_1F800020.t[0] += p->a.p.whole;
        D_1F800020.t[1] += p->y.p.whole;
        D_1F800020.t[2] += p->b.p.whole;
        o->a.p.whole = D_1F800020.t[0];
        o->y.p.whole = D_1F800020.t[1];
        o->b.p.whole = D_1F800020.t[2];
        o->d8c = ((0x1000 - ((TObj *)o->d90)->d8c) & 0xfff) >> 4;
        PLAYER[0] = 3;
        D_800A60A4[0] = 0x18;
        D_800A60A6[0] = 0x30;
        D_800A60A8[0] = 0x18;
        D_800A60AA[0] = 0x30;
        D_800A6048[0] = o->a.raw;
        D_800A604C[0] = o->y.raw;
        D_800A6050[0] = o->b.raw;
        D_1F800168[0] = o->a.raw;
        D_1F80016C[0] = o->y.raw;
        D_1F800170[0] = o->b.raw;
        ((unsigned char*)&GAME.selectedPlane)[0] = 0;
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
