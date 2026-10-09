// FUNC 8012ef3c 760 X003
// MATCHING 8012ef3c 760
/* includes the csv piece 8012F220 (the shared epilogue) */
#include "TOBJ.H"

extern TObj D_800A6038;
extern char D_80077CD0[];
extern void *D_8013A6C0[], *D_8013A6C4[];
extern unsigned short D_8007A3F0[];
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void FUN_80026bfc(int, int);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001faf4(TObj *);
extern int TileCollideAt(TObj *, int, int);
extern void FUN_800408d8(TObj *, int, int);
extern void func_8012D66C(TObj *);

void func_8012EF3C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->b9c = 0;
        FUN_80026bfc(1, 6);
        *(signed char *)&o->b0f = -7;
        o->velV = 0x300;
        o->movetab = D_80077CD0;
        o->b68 = 0;
        o->d8c = 0;
        o->wac = 0xa;
        o->state++;
        o->anim = D_8013A6C0[0];
        AnimLoadDuration(o);
        FUN_8001fa88(o, *(unsigned short *)&o->w7a);
        break;
    case 1:
        o->state++;
        o->velV -= 0x20;
        o->y.raw += o->velV << 8;
        if (o->b69 == 1 || (short)TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) o->b69 = 0;
        FUN_8001fa88(o, *(unsigned short *)&o->w7a);
        break;
    case 2:
        o->state++;
        D_800A6038.state = 2;
        *(unsigned char *)&D_800A6038.wac = 3;
        D_800A6038.b9c = 0;
    case 3:
        o->velV -= 0x20;
        if (o->velV < 0) o->state++;
        o->y.raw += o->velV << 8;
        if (o->b69 == 1 || (short)TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0x10))) o->b69 = 0;
        AnimAdvance(o);
        o->animFrame = D_800A6038.animFrame & 1;
        FUN_8001faf4(o);
        func_8012D66C(o);
        break;
    case 4:
        o->velV -= 0x20;
        if (o->velV < -0x300) {
            o->velV = -0x300;
            o->state++;
        }
        o->y.raw += o->velV << 8;
        FUN_800408d8(o, o->h->p.whole, (short)(o->y.p.whole - 0x20));
        AnimAdvance(o);
        o->animFrame = D_800A6038.animFrame & 1;
        FUN_8001faf4(o);
        func_8012D66C(o);
        break;
    case 5:
        o->ba7 += 2;
        o->y.raw += (short)(D_8007A3F0[o->ba7] << 2);
        AnimAdvance(o);
        o->animFrame = D_800A6038.animFrame & 1;
        FUN_8001faf4(o);
        func_8012D66C(o);
        break;
    case 6:
        o->animFrame = D_800A6038.animFrame & 1;
        o->wac = 0xb;
        o->state++;
        o->anim = D_8013A6C4[0];
        AnimLoadDuration(o);
        break;
    case 7:
        break;
    }
}
