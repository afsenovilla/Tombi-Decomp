// FUNC 80123d64 1016 X014
// MATCHING 80123d64 1016
// VERSION SLPS_01144 x14 8012103C 1016
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x14/nonmatchings/x14", func_8012103C_slps01144);
#else
#include "spad.h"
#include "TOBJ.H"
extern char D_8012A2FC[];
extern char D_8012A304[];
extern char D_8012A30C[];
extern char D_8012A314[];
extern char D_8012A31C[];
extern char D_8012A324[];
extern char D_8012A32C[];
extern char D_8012A334[];
extern char D_8012A33C[];
static void *D_80129FA8[9] = {
    D_8012A2FC, D_8012A304, D_8012A30C, D_8012A314,
    D_8012A31C, D_8012A324, D_8012A32C, D_8012A334,
    D_8012A33C,
};
extern unsigned short D_801266E0[];
extern unsigned short PLAYER_D_WHOLE[], PLAYER_Y_WHOLE[];
extern short COS_TABLE[], D_8007A1F0[];
extern short D_800A457C, D_800A457E, D_800A4582;
extern short D_800A4580[]; /* debt: second name for D_800A4582 keeps the game's reload */
extern void FUN_800202b4(TObj *);
extern void AnimLoadDuration(TObj *);
extern int FUN_8001f9e0(void);
extern int angleFromDelta(short, short);
extern TObj *allocObjectLayer2(void);
extern short probeSlopeWallSideAt(TObj *, int, int);
extern short probeFloorPlaneAt(TObj *, int, int);

static __inline__ short Out(TObj *o)
{
    if (o->h->p.whole < D_800A457C - 0x9e) return 1;
    if (o->h->p.whole > D_800A457E + 0x9e) return 1;
    return o->y.p.whole > D_800A4580[1] + 0x80;
}

static __inline__ int Chk(TObj *o, short k)
{
    short r = probeSlopeWallSideAt(o, o->h->p.whole, o->y.p.whole);
    int n = k;
    if (r != 0 && n == r && SPAD.s284 == 0)
        n++;
    else if (probeFloorPlaneAt(o, o->h->p.whole, o->y.p.whole) == 0)
        n = 0;
    else
        n = SPAD.s284 == 0;
    return n;
}

void lavaCavesPigShotDrop(TObj *o)
{
    TObj *n;
    TObj *p;
    short s;

    switch (o->step) {
    case 0:
        FUN_800202b4(o);
        o->y.p.whole -= 4;
        if (o->visible) break;
        o->timer = 0x1e;
        o->step++;
        break;
    case 1:
        if (--o->timer != -1) break;
        o->step++;
        o->anim = D_80129FA8[0];
        AnimLoadDuration(o);
        o->velH = 0x480;
        o->h->p.whole = (*(unsigned short*)((char*)&SPAD.i174 + 2)) + D_801266E0[FUN_8001f9e0() & 7];
        o->y.p.whole = ((unsigned short*)&SPAD.s17a)[0] - 0xf0;
        o->d->p.whole = PLAYER_D_WHOLE[0];
        o->d38 = angleFromDelta(((unsigned short*)((char*)&SPAD.i168 + 2))[0] - o->h->p.whole, PLAYER_Y_WHOLE[0] - o->y.p.whole);
        o->d8c = (unsigned char)(o->d38 + 0x40);
        o->d38 = *(unsigned char *)&o->d38;
        n = allocObjectLayer2();
        if (n) {
            unsigned char f;
            n->active = 2;
            n->type = 0x4b;
            f = o->b0f;
            n->b0a = 10;
            n->subtype = 0;
            n->b0c = 0;
            n->ba5 = 0;
            n->ba6 = 0;
            n->ba7 = 0;
            n->velX = 0x50;
            n->b0f = f - 4;
            n->d8c = o->d8c;
            n->a.p.whole = o->a.p.whole;
            n->y.p.whole = o->y.p.whole - 10;
            n->b.p.whole = o->b.p.whole;
            o->d94 = (int)n;
        }
        o->active = 1;
        break;
    case 2:
        FUN_800202b4(o);
        p = (TObj *)o->d94;
        {
            int i = o->d38;
            short v = o->velH;
            int a = v * COS_TABLE[i];
            int b = v * D_8007A1F0[i];
            o->h->raw += (a << 4 >> 16) << 8;
            o->y.raw += (b << 4 >> 16) << 8;
        }
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole - 10;
        s = 2;
        if (((o->d38 - 0x40) & 0xff) > 0x80) s = 1;
        if ((o->b69 = Chk(o, s)) != 0) {
            o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            break;
        }
        if (o->y.p.whole > D_800A4582 + 0xa0 || Out(o)) {
            o->active = 2;
            o->b04 = 3;
            p->b04 = 3;
        }
        break;
    }
}
#endif
