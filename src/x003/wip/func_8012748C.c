/* n51 (still 12): 3 sites: the two Ground2 sites (result used by `if (Ground2(o))`, +2d0/+78c) are the
   non-inverted `beqz;nop;j L;addiu v0,v1,-1;addiu v0,v1,1` shape in the game, the Ground site (result unused, +3xx)
   is inverted. The w-temp form (`int w; if (t<0x80) w=o->d8c-1; else w=o->d8c+1; o->d8c=w;`) gives the game's
   registers (v0 result, v1 load) but reorg steals the -1 from the j's slot and inverts (168). Experiments on the
   matched sibling func_8011D3EC (X009, `hp->p.whole = c ? x-16 : x+16`): the nop shape appears there ONLY when the
   code after the shared store is `j Lx` whose delay slot sets v0 (`j $L16; li $2,3`, a cross-jumped tail); any
   other following code (li before the j, store in the slot, epilogue target) inverts. So the game probably had a
   separate `li v0,1; j L78` for the main path's return 1 (later deleted by relax_delay_slots as a jump around an
   identical insn); every attempt to get that here (inner `return 1`, volatile reload, do{}while(0)/asm barriers,
   uchar/short return type, r = reload var) either lets jump2 cross-jump the lone li (merge, then inversion) or
   schedules the li between lbu/sw so r lands in v1. matchcheck (CC1PSX) agrees with ncheck on all variants. */
// FUNC 8012748c 2236 X003
#include "TOBJ.H"

typedef struct { short w0, w2; } X;
#define XS(o) ((X *)((char *)(o) + 0xb4))

extern char D_80077CF4[];
extern unsigned char D_80135CB0[];
extern void *D_80139520[];
extern unsigned short D_1F80027E;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern TObj *ObjAlloc(void);
extern void playSFX(int);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_8001fb20(TObj *);
extern int func_8004065C(TObj *, int, int, int);
extern int FUN_800408d8(TObj *, int, int);
extern int TileCollideAt(TObj *, int, int);

static __inline__ int Side(TObj *o)
{
    unsigned short w = o->w7a;
    int d;
    if (w & 1) d = -0x18;
    else d = 0x18;
    return func_8004065C(o, (short)(o->h->p.whole + d), o->y.p.whole, (short)w);
}

static __inline__ int Ground(TObj *o)
{
    unsigned char t;
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if ((short)TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0xe)) == 0) return 0;
    if (o->wac >= 0xc) {
        o->d8c = 0;
    } else {
        t = D_1F80027E * 4 + o->d8c;
        if (t) {
            if (t >= 0x80) o->d8c++;
            else o->d8c--;
            o->d8c = *(unsigned char *)&o->d8c;
        }
    }
    return 1;
}

static __inline__ int Ground2(TObj *o)
{
    unsigned char t;
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if ((short)TileCollideAt(o, o->h->p.whole, (short)(o->y.p.whole + 0xe)) == 0) return 0;
    if (o->wac >= 0xc) {
        o->d8c = 0;
    } else {
        t = D_1F80027E * 4 + o->d8c;
        if (t) {
            int v = o->d8c;
            if (t < 0x80) v--; else v++;
            o->d8c = v;
            o->d8c = *(unsigned char *)&o->d8c;
        }
    }
    return 1;
}

#define SETANIM8(o) { \
    unsigned char *p; \
    x->w0 = 1; \
    *(volatile short *)&o->wac = 8; \
    p = &D_80135CB0[o->wac * 4]; \
    o->anim = D_80139520[0]; \
    o->box0 = *p++; \
    o->box1 = *p++; \
    o->box2 = *p; \
    o->box3 = p[1]; \
    AnimLoadDuration(o); \
}

#define SPAWN(o) { \
    TObj *e = ObjAlloc(); \
    if (e) { \
        e->active = 2; \
        e->type = 0x2f; \
        e->animFrame = 1 - o->w7a; \
        e->a.p.whole = o->a.p.whole; \
        e->y.p.whole = o->y.p.whole; \
        e->b.p.whole = o->b.p.whole; \
        e->d8c = o->d8c; \
    } \
}

void func_8012748C(TObj *o)
{
    X *x = XS(o);

    switch (o->state) {
    case 0:
        if (x->w0 == 0 && o->w98 == 0) {
            o->state = 4;
            break;
        }
        if (x->w2) {
            x->w2 += o->b68;
            if (x->w2 > 5) x->w2 = 5;
            o->b68 = 1;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->w9a = 0;
            o->state = 4;
            o->substep = 0;
            break;
        }
        o->velV = -0x400;
        o->movetab = D_80077CF4;
        o->d8c = 0;
        o->b9c = 1;
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        FUN_8001fa88(o, (unsigned short)o->w7a);
        Side(o);
        o->velV += 0x40;
        o->y.raw += o->velV << 8;
        if ((short)FUN_800408d8(o, o->h->p.whole, (short)(o->y.p.whole - 0x10))) o->velV = 0;
        if (o->velV > 0) {
            o->b69 = 0;
            o->b9c = 2;
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        o->velV += 0x40;
        if (o->velV > 0x400) o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_8001fa88(o, (unsigned short)o->w7a);
        Side(o);
        if (Ground2(o)) {
            o->b9c = 0;
            o->velH = 0x180;
            o->state++;
        }
        break;
    case 3:
        if ((short)Side(o)) o->velH = 0;
        FUN_8001fb20(o);
        Ground(o);
        if (o->w7a & 1) o->h->raw -= o->velH << 8;
        else o->h->raw += o->velH << 8;
        o->velH -= 0x20;
        if (o->velH < 0) {
            o->velH = 0;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 1;
            o->substep = 0;
        }
        break;
    case 4:
        if (o->subtype == 2 && o->b9c) {
            SETANIM8(o);
            SPAWN(o);
            o->velV = -0x400;
            o->movetab = D_80077CF4;
            o->d8c = 0;
            o->state++;
            o->b9c = 1;
            o->state = 1;
            playSFX(0x77);
            break;
        }
        if ((unsigned char)(o->d8c - 0x70) < 0x21) {
            o->active = 5;
            o->state++;
            break;
        }
        playSFX(0x77);
        o->active = 1;
        o->b04 = 1;
        o->step = 1;
        o->state = 1;
        o->substep = 0;
        SETANIM8(o);
        SPAWN(o);
        break;
    case 5:
        o->velV = 0;
        o->b69 = 0;
        o->b9c = 2;
        if (o->d8c >= 0x80) o->state = 6;
        else o->state = 7;
        break;
    case 6:
    case 7:
        if (o->state == 6) o->d8c += 8;
        else o->d8c -= 8;
        o->d8c = *(unsigned char *)&o->d8c;
        o->velV += 0x20;
        if (o->velV > 0x600) o->velV = 0x600;
        o->y.raw += o->velV << 8;
        if (Ground2(o)) {
            o->d8c = (-(short)D_1F80027E << 2) & 0xff;
            o->active = 1;
            o->b04 = 1;
            o->step = 1;
            o->state = 2;
            o->substep = 0;
            SETANIM8(o);
            SPAWN(o);
        }
        break;
    }
}
