// FUNC 80101d74 924 X005
// MATCHING 80101d74 924
#include "TOBJ.H"
#include "raw7.h"
extern TObj *D_8009C330;
extern unsigned char *D_8009D2E8;
extern unsigned char *D_800A611C;
extern int D_8009C934;
extern int D_8009C960;
extern unsigned char D_8009C93A[];
extern unsigned char D_8009D00F;
extern unsigned char D_1f8003ce;
extern unsigned char D_801152E8[];
extern char D_80010748[];
extern void func_80101938(TObj *);
extern void FUN_80109494(TObj *);
extern void FUN_801231f4(TObj *);
extern void func_8010A9F4(TObj *);
extern void FUN_8010ac90(TObj *);
extern void func_80108168(TObj *);
extern void func_80108338(TObj *);
extern void FUN_80108dbc(TObj *);
extern void func_8011DD48(TObj *);
extern void FUN_8010ae94(TObj *);
extern void FUN_800f1308(TObj *);
extern void FUN_8010df84(TObj *);
extern void AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern short ObjTileCollide(TObj *, int, int);
extern void func_8003F7CC(TObj *);
void func_80101D74(TObj *o)
{
    short c;
    int v;
    switch (o->step) {
    case 1:
        if (--o->timer > 0) goto common;
        if (D_8009C960 == 0x40000) {
            S8(o, 0xf) = 8;
        } else if (D_8009C960 == 0x50000) {
            S8(o, 0xf) = -5;
        } else {
            S8(o, 0xf) = -8;
        }
        v = D_801152E8[o->wb0];
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        o->substep = 0;
        o->timer = 0;
        o->d8c = v;
        D_8009C93A[0] = 1;
        break;
    case 2:
        FUN_80109494(o);
        break;
    case 3:
        FUN_801231f4(o);
        break;
    case 4:
        func_8010A9F4(o);
        break;
    case 5:
        FUN_8010ac90(o);
        break;
    case 0:
    case 6:
        func_80101938(o);
        break;
    case 7:
        func_80108168(o);
        break;
    case 8:
        func_80108338(o);
        break;
    case 9:
        FUN_80108dbc(o);
        break;
    case 10:
        if (--o->timer <= 0) goto anim;
    common:
        o->animFrame &= 1;
        if (U8(o, 0xac) >= 2) {
            D_8009D2E8 = D_800A611C;
            D_8009D2E8[4] = 2;
            D_8009D2E8[5] = 2;
            D_8009D2E8[6] = 0;
        }
        U8(o, 0xac) = 0;
        D_8009C934 = 0;
        U8(o, 0xc7) = 1;
        o->b9d = 0;
        U8(o, 0xc6) = 0;
        U8(o, 0xe3) = 0;
        U8(D_8009C330, 0) = 0;
        v = 0x128;
        if (o->animFrame & 1) {
            v = -0x128;
        }
        o->wb2 = v;
        FUN_8010df84(o);
        if (o->b69 != 0) {
            o->y.p.whole += 2;
        }
        ObjTileCollide(o, 0, 0);
        o->d8c = D_801152E8[o->wb0];
        break;
    case 11:
        func_8011DD48(o);
        break;
    case 12:
        FUN_8010ae94(o);
        break;
    case 0x40:
        o->visible = 0;
        break;
    case 0x61:
        U8(o, 0xa2) = 0;
        U8(o, 0xa3) = 0;
        AnimAdvance(o);
        o->d8c = 0;
        break;
    case 0x62:
        o->anim = D_80010748;
        AnimLoadDuration(o);
        o->step++;
    case 0x63:
        o->anim = D_80010748;
        AnimLoadDuration(o);
        o->d8c = 0;
        break;
    case 0x64:
        o->d8c = 0;
    case 0x65:
        if (D_8009D00F != 0) {
            FUN_800f1308(o);
        } else if (D_1f8003ce == 0 || *(unsigned short *)o->anim != 0) {
        anim:
            AnimAdvance(o);
        }
        break;
    }
    c = 0;
    U8(o, 0xa8) = 0;
    o->ba6 = 0;
    if (o->b9e == 0 && o->b04 == 5) {
        c = o->step == 0x40;
        if (o->step == 0x41) c++;
        if (o->step == 0x65) c++;
    }
    if (c == 0) func_8003F7CC(o);
}
