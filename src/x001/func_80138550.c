// FUNC 80138550 1788 X001
// MATCHING 80138550 1788
/* Debt: D_8013E710s is a second (scalar) name for D_8013E710[0]; the scalar load is hoisted above the
   struct stores in cases 0 and 6 as in the game. */
#include "TOBJ.H"

typedef struct { short z, y, x; } P3;

extern P3 D_8013C9EC[];
extern void *D_8013E710[];
extern void *D_8013E710s;
extern int D_1F8002D4a[];
extern unsigned char D_800A60A1, D_800A60E4, D_800A60D4, D_800A60D6;
extern int D_800A604C;
extern Fix16 *D_800A6078;
extern short D_8007A5F0[], D_8007A1F0[];
extern int Rand(void);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);
extern void ObjAddVelY7E(TObj *);
extern int MulNegSinScaled(int, int);
extern short TileCollideAt(TObj *, short, short);

void func_80138550(TObj *o)
{
    int s1;
    int s2;
    int b;

    switch (o->b04) {
    case 0:
        o->active = 2;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 8;
        o->box3 = 0x10;
        o->b6a = 0;
        o->b69 = 0;
        o->b6b = 1;
        o->a.raw = D_8013C9EC[o->b0c].x << 16;
        o->y.raw = D_8013C9EC[o->b0c].y << 16;
        o->b.raw = D_8013C9EC[o->b0c].z << 16;
        o->w1e = 7;
        *(signed char *)&o->b0f = -9;
        o->b0d = 0x80;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->d3c = D_1F8002D4a[0];
        o->anim = D_8013E710s;
        AnimLoadDuration(o);
        o->timer = (Rand() & 0xff) + 1;
        o->step = 0;
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            if (--o->timer == 0) {
                o->anim = D_8013E710[1];
                AnimLoadDuration(o);
                o->b6a = 0;
                o->active = 1;
                o->step++;
            }
            break;
        case 1:
            if (AnimAdvance(o)) {
                o->anim = D_8013E710[5];
                o->y.p.whole += 0x18;
                AnimLoadDuration(o);
                o->velY = 0;
                o->d8c = 0;
                o->step++;
            }
            break;
        case 2:
            if (o->velY < 0x600) o->velY += 0x20;
            ObjAddVelY7E(o);
            if (o->b6a && !D_800A60A1 && D_800A60E4 < 2 && D_800A60D4 && (unsigned char)(D_800A60D6 - 5) >= 2)
                D_800A604C += o->velY << 8;
            if (o->b69) {
                o->anim = D_8013E710[2];
                AnimLoadDuration(o);
                o->b69 = 0;
                o->step++;
            } else if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 8)) {
                o->d8c = 0;
                o->active = 2;
                o->anim = D_8013E710[6];
                AnimLoadDuration(o);
                o->velY = 0;
                o->step = 6;
            }
            break;
        case 3:
            if (o->b69) {
                o->b69 = 0;
                if ((unsigned)((o->d8c + 0x40) & 0xff) >= 0x81) {
                    o->anim = D_8013E710[4];
                    AnimLoadDuration(o);
                    o->w22 = 4;
                    o->step++;
                }
                s1 = (o->d8c + 4) & 0x7f;
                if ((unsigned)s1 < 8) {
                    if (o->b6a) {
                        if (AnimAdvance(o)) {
                            o->anim = D_8013E710[9];
                            AnimLoadDuration(o);
                        }
                    } else {
                        o->anim = D_8013E710[2];
                    }
                } else {
                    s1 = o->d8c;
                    if (s1 < 0x80) {
                        s1 += 0x80;
                        o->animFrame = 1;
                    } else {
                        o->animFrame = 0;
                    }
                    s1 &= 0xff;
                    s2 = -(short)MulNegSinScaled((o->d8c << 1) & 0x7e, 0x300);
                    b = (short)MulNegSinScaled(o->d8c & 0x7f, 0x400);
                    o->h->raw += (D_8007A5F0[s1] * s2) >> 4;
                    o->y.raw += (D_8007A1F0[s1] * -b) >> 4;
                    if (o->b6a) {
                        if ((unsigned char)(D_800A60D6 - 5) >= 2) {
                            D_800A6078->raw += (D_8007A5F0[s1] * s2) >> 4;
                            D_800A604C += (D_8007A1F0[s1] * -b) >> 4;
                        }
                        if (AnimAdvance(o)) {
                            o->anim = D_8013E710[10];
                            AnimLoadDuration(o);
                        }
                    } else {
                        o->anim = D_8013E710[3];
                    }
                    o->y.p.whole += 2;
                }
                AnimLoadDuration(o);
            } else {
                o->anim = D_8013E710[4];
                AnimLoadDuration(o);
                o->w22 = 4;
                o->step++;
            }
            break;
        case 4:
            if (--o->w22 == 0) {
                o->anim = D_8013E710[5];
                AnimLoadDuration(o);
                o->b6a = 0;
                o->velY = 0;
                o->d8c = 0;
                o->step = 2;
            }
            break;
        case 5:
            o->d8c = 0;
            o->active = 2;
            o->anim = D_8013E710[6];
            AnimLoadDuration(o);
            o->velY = 0;
            o->step++;
        case 6:
            if (AnimAdvance(o)) {
                o->a.raw = D_8013C9EC[o->b0c].x << 16;
                o->y.raw = D_8013C9EC[o->b0c].y << 16;
                o->b.raw = D_8013C9EC[o->b0c].z << 16;
                o->d8c = 0;
                o->anim = D_8013E710s;
                AnimLoadDuration(o);
                o->timer = (Rand() & 0xff) + 1;
                o->step = 0;
            }
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
