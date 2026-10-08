// FUNC 800fa35c 1472 X016
// MATCHING 800fa35c 1472
#include "TOBJ.H"
typedef struct {
    unsigned char p0[8];
    unsigned char b8;
    unsigned char p9[0x2c - 9];
    unsigned short w2c;
    unsigned short w2e;
} P800FA35C;

extern P800FA35C *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_800ff610(TObj *, int);

static __inline__ void setanim(TObj *o, unsigned short x)
{
    P800FA35C *p = DAT_8009c330;
    p->w2c = x;
    if (p->w2e != x) {
        p->w2c = x;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0);
        DAT_8009c330->w2e = DAT_8009c330->w2c;
    }
}

#define TURN() \
    x = DAT_801152e8[o->wb0]; \
    c = (unsigned char)(x - o->d8c); \
    d = c; \
    if (d == 0) return; \
    u = c; \
    if (u < 0x80) { \
        if (d >= 4) o->d8c = o->d8c + 4; \
        else if (d >= 2) o->d8c = o->d8c + 2; \
        else o->d8c = o->d8c + 1; \
    } else { \
        if (d < 0xfd) o->d8c = o->d8c - 4; \
        else if (d < 0xff) o->d8c = o->d8c - 2; \
        else o->d8c = o->d8c - 1; \
    }

void FUN_800fa35c(TObj *o)
{
    int t;
    unsigned char x;
    unsigned int c;
    short d;
    unsigned short u;

    switch (o->substep) {
    case 0:
        o->timer = 0x14;
        o->velX = 0;
        o->velY = 0;
        setanim(o, 0x10);
        o->substep++;
        FUN_800ff610(o, 0);
    case 1:
        if (o->timer != 0) o->timer--;
        FUN_8001fec0(o);
        {
            int d0 = o->d8c;
            if (o->animFrame & 1) t = (d0 - 0x10) & 0xff;
            else t = (d0 + 0x10) & 0xff;
            o->d8c = t;
        }
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x800;
        if (o->velY > 0x680) o->velY = 0x680;
        if (o->ba6 != 0 || (o->b69 != 0 && (o->velY = 0, o->timer <= 0))) {
            o->d8c = 0;
            setanim(o, 0x2f);
            o->timer = 0x1e;
            o->substep++;
        } else if (FUN_8003fd78(o, 0, 0)) {
            o->b69 = 0;
            o->velY = 0;
            if (o->timer <= 0) {
                o->d8c = 0;
                setanim(o, 0x2f);
                o->timer = 0x1e;
                o->substep++;
            }
            FUN_800ff610(o, 0);
        }
        break;
    case 2:
        FUN_8001fec0(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x800;
        if (o->velY > 0x680) o->velY = 0x680;
        if (o->timer != 0 && --o->timer <= 0) o->substep++;
        if (o->b69 != 0 || FUN_8003fd78(o, 0, 0)) {
            o->velY = 0;
            FUN_800ff610(o, 0);
        }
        TURN();
        o->d8c = *(unsigned char *)&o->d8c;
        break;
    case 3:
        o->b69 = 0;
        *(unsigned char *)&o->wac = 0;
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        setanim(o, 0x30);
        o->substep++;
        TURN();
        o->d8c = *(unsigned char *)&o->d8c;
        break;
    case 4:
        o->y.raw += o->velY << 8;
        o->velY += 0x800;
        if (o->velY > 0x680) o->velY = 0x680;
        if (o->b69 != 0 || FUN_8003fd78(o, 0, 0)) {
            o->velY = 0;
            FUN_800ff610(o, 0);
        }
        if (FUN_8001fec0(o)) {
            DAT_8009c330->b8 = 0;
            o->active = 1;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
        }
        TURN();
        o->d8c = *(unsigned char *)&o->d8c;
        break;
    }
}
