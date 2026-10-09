// FUNC 801251b4 1072 X014
// MATCHING 801251b4 1072
#include "TOBJ.H"
extern int D_1F8002DC[];
extern void *D_8012A004[];
extern unsigned char D_8009C942;
extern TObj *FUN_800183b8(void);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_80018790(TObj *);
extern void func_80124AD0(TObj *);
extern void func_80124F28(TObj *);

static __inline__ void setbox(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

#define SPAWN(o, st, c) { \
    TObj *e = FUN_800183b8(); \
    if (e) { \
        e->active = 2; \
        e->type = 0x50; \
        e->subtype = st; \
        e->b0c = c; \
        e->a.raw = o->a.p.whole << 16; \
        e->y.raw = o->y.p.whole << 16; \
        e->b.raw = o->b.p.whole << 16; \
        e->animFrame = o->animFrame & 1; \
    } }

static __inline__ void fin(TObj *o, unsigned char st, unsigned char n)
{
    SPAWN(o, st, 1);
    o->b04 = n;
    o->y.p.whole -= 8;
}

static __inline__ void sp2(TObj *o, unsigned char st)
{
    SPAWN(o, st, 0);
    fin(o, st, 3);
}

void func_801251B4(TObj *o)
{
    switch (o->b04) {
    case 0:
        switch (o->subtype) {
        case 0:
            o->active = 1;
            setbox(o, 0x10, 0x20, 0x10, 0x20);
            o->b0a = 0;
            o->b0d = 0x80;
            break;
        case 1:
            setbox(o, 8, 0x10, 10, 0x14);
            o->b0a = 2;
            o->b0d = 0;
            break;
        case 2:
            o->active = 1;
            setbox(o, 0x10, 0x18, 4, 8);
            o->b0a = 10;
            o->velX = 0xc0;
            o->ba5 = 0;
            o->ba6 = 0;
            o->ba7 = 0;
            o->b0d = 0x80;
            break;
        }
        *(signed char *)&o->b0f = -12;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->w1e = 1;
        o->b04++;
        o->d3c = D_1F8002DC[0];
        o->anim = D_8012A004[o->subtype];
        FUN_8001fe6c(o);
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            if (D_8009C942 == 1) goto anim;
            if (!FUN_8001fec0(o)) goto anim;
            o->y.p.whole += 8;
            sp2(o, 2);
            goto anim;
        case 1:
            if (D_8009C942 != 1) func_80124AD0(o);
            goto anim;
        case 2:
            if (D_8009C942 == 1) {
                if (o->visible == 1) FUN_800202b4(o);
            } else {
                func_80124F28(o);
                if (o->visible == 1) goto anim;
            }
            break;
        }
        break;
    case 2:
        if (D_8009C942 == 1) {
        anim:
            FUN_800202b4(o);
            break;
        }
        FUN_800202b4(o);
        if (o->subtype == 1) {
            SPAWN(o, 0, 0);
            o->b04++;
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
