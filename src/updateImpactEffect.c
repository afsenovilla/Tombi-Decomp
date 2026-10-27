// FUNC 8002be80 472 MAIN0
// MATCHING 8002be80 472
// VERSION SLPM_80165 slpm80165 8002BE54 436
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8002BE80 (func_8002BE80.c): subtype 2 keeps its animation, follows the player at y.
#include "player.h"
#include "TOBJ.H"
extern void *D_80012034[];
extern void *D_80012268;
extern unsigned char D_80079D3C[];
extern unsigned short PLAYER_Y[];
extern int DAT_1f8002d8[];
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern void ObjListPush_1F800220(TObj *);
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);

void func_8002BE80(TObj *o)
{
    unsigned char s = o->b04;
    int d;
    switch (s) {
    case 0:
        o->w1e = 0x15;
        if (o->subtype == 0 || o->subtype == 3) {
            o->w1e = 0x14;
        }
        o->b0d = 0;
        *(signed char *)&o->b0f = -30;
        o->b0a = 0;
        o->animFrame = 0;
        o->anim = D_80012034[o->subtype];
        o->d3c = DAT_1f8002d8[0];
        o->b04++;
        AnimLoadDuration(o);
        if (o->subtype == 2) {
            playSFX(D_80079D3C[2]);
        }
        break;
    case 1:
        if (o->subtype == 2) {
            o->visible = 1;
            ObjListPush_1F800220(o);
            if (PLAYER.obj.animFrame & 1) d = -14; else d = 14;
            o->h->p.whole = (*(Fix16 **)&PLAYER.obj.h)->p.whole + d;
            o->y.p.whole = PLAYER_Y[0];
            if (AnimAdvance(o)) {
                o->b04++;
            }
        } else if (ObjCullRegister(o)) {
            if (AnimAdvance(o)) {
                o->b04++;
            }
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#else
#include "player.h"
#include "TOBJ.H"
extern char D_800125D8[];
extern char D_800122F8[];
extern char D_80012338[];
extern char D_80012628[];
extern char D_800124B8[];
static void *D_80012034[5] = {
    D_800125D8, D_800122F8, D_80012338, D_80012628,
    D_800124B8,
};
extern void *D_80012268;
extern unsigned char D_80079D3C[];
extern unsigned short PLAYER_Y[];
extern int DAT_1f8002d8[];
extern void AnimLoadDuration(TObj *);
extern void playSFX(int);
extern void ObjListPush_1F800220(TObj *);
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);

void updateImpactEffect(TObj *o)
{
    unsigned char s = o->b04;
    int d;
    switch (s) {
    case 0:
        o->w1e = 0x15;
        if (o->subtype == 0 || o->subtype == 3) {
            o->w1e = 0x14;
        }
        o->b0d = 0;
        *(signed char *)&o->b0f = -30;
        o->b0a = 0;
        o->animFrame = 0;
        o->anim = D_80012034[o->subtype];
        o->d3c = DAT_1f8002d8[0];
        o->b04++;
        AnimLoadDuration(o);
        if (o->subtype == 2) {
            o->b0d = 0x80;
            o->w1e = 0x13;
            o->anim = D_80012268;
            playSFX(D_80079D3C[o->subtype]);
        }
        break;
    case 1:
        if (o->subtype == 2) {
            o->visible = 1;
            ObjListPush_1F800220(o);
            if (PLAYER.obj.animFrame & 1) d = -14; else d = 14;
            o->h->p.whole = (*(Fix16 **)&PLAYER.obj.h)->p.whole + d;
            o->y.p.whole = PLAYER_Y[0] - 14;
            if (AnimAdvance(o)) {
                o->b04++;
            }
        } else if (ObjCullRegister(o)) {
            if (AnimAdvance(o)) {
                o->b04++;
            }
        }
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
