// FUNC 80122044 464 X001
// MATCHING 80122044 464
#include "TOBJ.H"

typedef struct { char pad[2]; short w2; } PL;
extern PL *D_8009C330;
typedef struct { char pad[0xa2]; unsigned char ba2; char pad2[7]; unsigned char baa; } XB;
#define X(o) ((XB *)(o))
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F[];
extern unsigned short D_1F8001F8;
extern unsigned char D_801152E8[];
extern void PlayerSetAnimIfChanged(TObj *, int);
extern int AnimAdvance(TObj *);
extern short TileCollideAt(TObj *, short, short);
extern void FUN_8003fb90(TObj *);
extern void SfxPlay2(int, int);

void func_80122044(TObj *o)
{
    switch (o->state) {
    case 0:
        PlayerSetAnimIfChanged(o, 0x17);
        o->wb6 = 0;
        D_8009C330->w2 = 0;
        X(o)->ba2 = 3;
        o->wb0 = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d8c = 0;
        X(o)->baa = 1;
        o->timer = 0x20;
        o->y.p.whole += 8;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        o->state++;
    case 1:
        o->y.p.whole++;
        AnimAdvance(o);
        if (--o->timer <= 0) {
            o->state++;
        }
        break;
    case 2:
        o->y.p.whole++;
        AnimAdvance(o);
        if (TileCollideAt(o, o->h->p.whole + ((o->animFrame & 1) ? 9 : -9), o->y.p.whole + 0x10)) {
            FUN_8003fb90(o);
            D_8009C942[0] = 0;
            D_8009C93F[0] = 0;
            X(o)->baa = 0;
            o->b69 = 0;
            o->b9e = 0;
            o->d8c = D_801152E8[o->wb0];
            o->step = 0;
            o->state = 0;
        }
        if ((D_1F8001F8 & 0xf) == 0) {
            SfxPlay2(0x1d, 0);
        }
        break;
    }
}
