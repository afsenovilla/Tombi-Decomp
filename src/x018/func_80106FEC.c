// FUNC 80106fec 784 X018
// MATCHING 80106fec 784
#include "TOBJ.H"
typedef struct { char p[0x2c]; unsigned short w2c; unsigned short w2e; } G;
extern G *D_8009C330;
extern int D_8009C984[];
extern unsigned short D_8009D670;
extern unsigned short D_1F8003C4, D_1F8001FC, D_1F8003C6;
extern unsigned char D_8009D2B0[];
void ObjSetAnimFromTable(TObj *o);
void AnimLoadDuration(TObj *o);
int AnimAdvance(TObj *o);
void FUN_800ef4e4(TObj *o);
void func_800EEA3C(TObj *o);
void func_8010FE70(TObj *o);
void FUN_800ee9cc(TObj *o);
void FUN_800ee88c(TObj *o);
void FUN_800f00ac(TObj *o);
void ObjAddVelY7E(TObj *o);
short ObjTileCollide(TObj *, int, int);
void func_8010E328(TObj *o, int);
void func_80106FEC(TObj *o)
{
    G *g;
    switch (o->state) {
    case 0:
        *((unsigned char *)o + 0xd5) = 0;
        *((unsigned char *)o + 0xd6) = 0;
        *((unsigned char *)o + 0xd7) = 0;
        *((unsigned char *)o + 0xbf) = 0;
        g = D_8009C330;
        o->velX = 0;
        o->velY = 0;
        g->w2c = 0x1a;
        if ((D_8009C984[0] & 0x40) && (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4))
            g->w2c = 0x2a;
        if (D_8009C330->w2e != D_8009C330->w2c) {
            ObjSetAnimFromTable(o);
            AnimLoadDuration(o);
            D_8009C330->w2e = D_8009C330->w2c;
        }
        o->state++;
    case 1:
        FUN_800ef4e4(o);
        func_800EEA3C(o);
        func_8010FE70(o);
        if (o->b69 & 2) {
            AnimAdvance(o);
            func_8010FE70(o);
            o->b69 = 1;
        }
        FUN_800ee9cc(o);
        FUN_800ee88c(o);
        FUN_800f00ac(o);
        if (AnimAdvance(o)) goto stop;
        if ((o->animFrame & 1) && o->wb2 < 0) {
            o->step = 1;
            o->state = 0;
        }
        if (!(o->animFrame & 1) && o->wb2 > 0) {
            o->step = 1;
            o->state = 0;
        }
        if ((o->animFrame & 2) && o->animFrame < 4) {
            D_8009C330->w2e = 0xffff;
            o->step = 0;
            o->state = 0;
        }
        if (D_1F8001FC & D_1F8003C6) {
            D_8009D2B0[0] = 0;
            o->b9c = 1;
            if ((D_8009C984[0] & 0x40) && (*(volatile unsigned short *)&D_8009D670 & D_1F8003C4))
                o->ba7 = 1;
            o->step = 2;
            o->state = 0;
        }
        func_8010E328(o, 0);
        break;
    case 2:
        AnimAdvance(o);
        func_800EEA3C(o);
        ObjAddVelY7E(o);
        o->velY += 0x30;
        if (o->velY > 0) o->b9c = 2;
        if (o->b69 != 0) break;
        if (ObjTileCollide(o, 0, 0) == 0) break;
    stop:
        o->b9c = 0;
        o->velX = 0;
        o->velY = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
