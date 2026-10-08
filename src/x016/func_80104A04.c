// FUNC 80104a04 724 X016
// MATCHING 80104a04 724
#include "TOBJ.H"
typedef struct { char p[8]; unsigned char b8; char q[0x20 - 9]; short w20; } G;
extern G *D_8009C330;
extern short D_8009C944, D_8009C946[];
extern int D_8009D2E8;
extern int D_8009C984[];
extern unsigned short D_8009D670;
extern unsigned short D_1F8003C4;
extern int D_8009F0EC;
extern int D_8009F0EC_r;
extern unsigned char D_801152E8[];
void FUN_800ef490(TObj *o);
void ObjMotionStep(TObj *o);
void ObjSetAnimFromTable(TObj *o);
void AnimJump(TObj *o, short n);
short ObjTileCollide(TObj *, int, int);
void FUN_800ee428(TObj *o);
int func_8004BBC0(TObj *o, int);
void func_800EFC8C(TObj *o, int);
void func_8010E328(TObj *o, int);
void func_80104A04(TObj *o)
{
    o->h->raw += D_8009C944 << 8;
    o->y.raw += D_8009C946[0] << 8;
    FUN_800ef490(o);
    ObjMotionStep(o);
    if (*(unsigned char *)&o->wac == 2) {
        D_8009D2E8 = *(int *)((char *)o + 0xe4);
        D_8009C330->b8 = 0;
        D_8009C330->w20 = 0;
        o->d8c = 0;
        o->ba7 = 0;
        o->ba5 = 0;
        o->step = 0xe;
        o->state = 0;
        return;
    }
    if (o->b69 == 1) {
        ObjSetAnimFromTable(o);
        AnimJump(o, 0);
        D_8009C330->b8 = 0;
        goto reset;
    }
    if (ObjTileCollide(o, 0, 0) != 0) {
        D_8009C330->b8 = 0;
        o->d84 = 0;
        D_8009C330->b8 = 0;
    reset:
        o->b9c = 0;
        o->ba7 = 0;
        o->wb2 = 0;
        o->velY = 0;
        *(unsigned char *)&o->wac = 0;
        o->d8c = D_801152E8[o->wb0];
        o->step = 0;
        o->state = 0;
        return;
    }
    if (o->animFrame & 1) {
        o->d8c += 0x10;
        if (o->d8c >= 0x300) {
            o->d8c = 0x300;
            D_8009C330->b8 = 0;
            *((unsigned char *)o + 0xad) = 0;
            o->b9c = 2;
            o->wb2 = 0;
            D_8009C330->w20 = 0xe;
            if ((D_8009C984[0] & 0x40) && (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 3;
            FUN_800ee428(o);
        }
    } else {
        o->d8c -= 0x10;
        if (o->d8c <= 0x100) {
            o->d8c = 0x100;
            D_8009C330->b8 = 0;
            *((unsigned char *)o + 0xad) = 0;
            o->b9c = 2;
            o->wb2 = 0;
            D_8009C330->w20 = 0xe;
            if ((D_8009C984[0] & 0x40) && (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 3;
            FUN_800ee428(o);
        }
    }
    if (*(unsigned char *)&o->wac < 2) {
        if ((D_8009F0EC = func_8004BBC0(o, 0)) != 0) {
            D_8009C330->b8 = 0;
            D_8009C330->w20 = 0;
            *(unsigned char *)&o->wac = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->wb2 = 0;
            func_800EFC8C(o, D_8009F0EC_r == 1);
        }
    }
    if (o->step == 0xf && !o->b9e) func_8010E328(o, 1);
}
