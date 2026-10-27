// FUNC 8002f960 616 MAIN0
// MATCHING 8002f960 616
#include "TOBJ.H"
typedef struct { void **p; int a, b; } E12;
extern E12 PLAYER_EFFECT_4E_ANIM_TABLE[];
extern unsigned short DAT_1f8001f8;
extern unsigned char DAT_800a6039, DAT_800a6047[], DAT_800a60a1[], DAT_800a6104[];
extern Fix16 *PLAYER_H, *PLAYER_D;
extern unsigned short PLAYER_Y;
extern void FUN_8001fe6c(TObj *);
extern void startPadVibration(int, int, int, int);
extern void FUN_8001fec0(TObj *);
extern int FUN_8001fddc(int, int);
extern int FUN_8001fdac(int, int);

void playerEffect4EStepD(TObj *o)
{
    int t;
    unsigned int u, w;
    switch (o->state) {
    case 0:
        o->anim = *PLAYER_EFFECT_4E_ANIM_TABLE[o->subtype].p;
        FUN_8001fe6c(o);
        o->state++;
        switch (o->b0c) {
        case 0: o->d84 = 0; break;
        case 1: o->d84 = 0x56; break;
        case 2: o->d84 = 0xab; break;
        }
        o->timer = 0x31;
        o->d88 = 0;
        break;
    case 1:
        if (o->b0c == 0 && !(DAT_1f8001f8 & 3))
            startPadVibration(0, 0, 0xff, 2);
        FUN_8001fec0(o);
        o->visible = DAT_800a6039;
        o->h->p.whole = PLAYER_H->p.whole + FUN_8001fddc((short)o->d84, (short)o->d88);
        if (o->b0c)
        {
            t = FUN_8001fdac((short)o->d84, 8) + 8;
            o->y.p.whole = PLAYER_Y + t;
        }
        else
        {
            t = FUN_8001fdac((short)o->d84, 8) + 0x18;
            o->y.p.whole = PLAYER_Y + t;
        }
        o->d->p.whole = PLAYER_D->p.whole + FUN_8001fdac((short)o->d84, (short)o->d88);
        o->d84 = (o->d84 + 0x10) & 0xff;
        o->d88 += o->d88 < 0xc;
        o->b0f = DAT_800a6047[0];
        if (DAT_800a60a1[0])
            o->timer = 0;
        if (--o->timer <= 0) {
            o->timer = 0;
            DAT_800a6104[0] = 3;
            o->b6b = 0;
            o->b04 = 2;
        } else {
            u = o->b6b;
            if (u >= 0x7f)
                w = u;
            else
                w = u + 8;
            o->b6b = w;
        }
        break;
    }
}
