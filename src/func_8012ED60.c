// FUNC 8012ed60 1108 X000
// MATCHING 8012ed60 1108
#include "TOBJ.H"
typedef struct {
    short p0, x, p4, y, p8, z;
} Pos8012ED60;

extern unsigned char D_8009D07B;
extern void *D_8013B224[];
extern unsigned short D_80139208[];
extern unsigned char D_800A6047;
extern TObj *D_8009C948;
extern int D_8009C984;
extern unsigned char D_800A6038;
extern void FUN_80123b64(Pos8012ED60 *);
extern int ObjCullRegister(TObj *);
extern void FUN_8012f1b4(TObj *);
extern void FUN_8012f380(TObj *);
extern void FUN_8012fb3c(TObj *);
extern void FUN_8012fe98(TObj *);
extern void FUN_801302a0(TObj *);
extern void FUN_80018790(TObj *);

static __inline__ int nearObj(TObj *o, TObj *p)
{
    if (p == 0) return 0;
    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 45) >= 91) return 0;
    if ((unsigned short)(o->y.p.whole - p->y.p.whole + 126) >= 253) return 0;
    return (unsigned short)(o->h->p.whole - p->h->p.whole + 16) < 33;
}

static __inline__ int nearCam(TObj *o)
{
    short dv;
    if ((unsigned short)(o->d->p.whole - *(unsigned short *)0x1F800172 + 45) >= 91) return 0;
    dv = *(unsigned short *)0x1F80016E - o->y.p.whole;
    if (dv >= -31 || (unsigned short)(dv + 172) >= 173) return 0;
    return (unsigned short)(o->h->p.whole - *(unsigned short *)0x1F80016A + 16) < 33;
}

void func_8012ED60(TObj *o)
{
    Pos8012ED60 s;
    unsigned short *b;
    int r;

    switch (o->b04) {
    case 0:
        o->w1e = 12;
        if (o->b0c == 1) {
            if (D_8009D07B == 2) {
                o->b04 = 3;
                break;
            }
            if (D_8009D07B == 1) {
                if (o->subtype != 0) {
                    o->b04 = 3;
                    break;
                }
                s.x = o->h->p.whole;
                s.y = o->y.p.whole - 16;
                s.z = o->d->p.whole;
                FUN_80123b64(&s);
                o->b04 = 3;
                break;
            }
        }
        o->animFrame = 1;
        o->b0d = 0;
        o->b0a = 2;
        o->anim = D_8013B224[o->subtype];
        b = &D_80139208[o->subtype * 4];
        o->d3c = ((struct { char pad[0x2d4]; int v; } *)0x1F800000)->v;
        o->box0 = *b++;
        o->box1 = *b++;
        o->box2 = b[0];
        o->box3 = b[1];
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b6b = 0;
        o->b6a = 0;
        o->b69 = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        o->d34 = o->y.p.whole;
        o->b04++;
        switch (o->subtype) {
        case 0:
            o->w7a = 2;
            break;
        case 1:
            o->w7a = -2;
            break;
        case 2:
            o->w7a = -1;
            o->active = 2;
            break;
        }
        break;
    case 1:
        o->b0f = D_800A6047 + *(unsigned char *)&o->w7a;
        if (ObjCullRegister(o)) {
            if (o->step == 0) {
                if (o->b0c == 1 && nearObj(o, D_8009C948)) {
                    o->step = 1;
                    o->state = 0;
                    goto sw;
                }
                r = nearCam(o);
                if (r) {
                    if (!(D_8009C984 & 2) && D_800A6038 == 1) {
                        o->step = 1;
                        o->state = 0;
                    }
                } else if (o->b68) {
                    o->step = 2;
                    o->state = 0;
                }
            }
sw:
            switch (o->step) {
            case 0:
                FUN_8012f1b4(o);
                break;
            case 1:
                FUN_8012f380(o);
                break;
            case 2:
                FUN_8012fb3c(o);
                break;
            case 3:
                FUN_8012fe98(o);
                break;
            case 4:
                FUN_801302a0(o);
                break;
            }
            o->b68 = 0;
            o->b69 = 0;
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
