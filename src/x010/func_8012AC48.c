// FUNC 8012ac48 2720 X010
// MATCHING 8012ac48 2720
/* Row 5 of the box table is read through scalar externs (D_8012F3E8..EB): a fixed-address scalar does not
   conflict with the o-> stores, so those loads hoist as in the game, while the indexed reads stay ordered.
   n (a1) and the shared byte offset k = (short)n * 4 (a0) feed both tables at the `set` tail. */
#include "TOBJ.H"

typedef struct { short a, x, y, z, sub; } Q;
typedef struct { char p[0xd2]; short wd2; } E;

extern unsigned char D_8012F3D4[];
extern unsigned char D_8012F3E8, D_8012F3E9, D_8012F3EA, D_8012F3EB;
extern void *D_80132310[];
extern void *D_8013237C[];
extern unsigned short D_8012F49C[];
extern int D_1F8002D0[];
extern unsigned short D_1F800176, D_1F800186;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int Rand(void);
extern TObj *ObjAlloc(void);
extern short FUN_80040278(TObj *, int, int);
extern short FUN_8004065c(TObj *, short, short, short);
extern void FUN_8001f8e4(TObj *);
extern void FUN_8001e4f0(int);
extern int FUN_800203dc(TObj *);
extern void func_80128D78(TObj *);
extern void func_80129F7C(TObj *);
extern void func_80129048(TObj *);
extern void func_801291C4(TObj *);
extern void func_801297D8(TObj *);
extern void func_80128764(TObj *);
extern void func_8012A664(TObj *);
extern void func_8012AAE4(TObj *);

static __inline__ int onscreen(TObj *o)
{
    if ((unsigned short)(o->a.p.whole - D_1F800176 + 0xe0) >= 0x301) return 0;
    return (unsigned short)(D_1F800186 - o->y.p.whole + 0x78) < 0x1e1;
}

static __inline__ int blocked(TObj *o, short f)
{
    short d;
    if (f & 1) d = -0x10;
    else d = 0x10;
    return FUN_8004065c(o, o->h->p.whole + d, o->y.p.whole, f) != 0;
}

void func_8012AC48(TObj *o)
{
    Q *q = (Q *)&o->wb4;
    TObj *e;
    short yy;
    int n;
    unsigned char *p;
    int k;
    int c;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->active = 2;
            o->step++;
            q->a = 0;
            q->x = o->a.p.whole;
            q->y = o->y.p.whole;
            q->z = o->b.p.whole;
            q->sub = o->subtype;
            o->w98 = 3;
            o->box0 = 0xa;
            o->box1 = 0x14;
            o->box2 = 0x20;
            o->box3 = 0x3a;
            o->w9a = 0;
            o->b68 = 0;
            o->b9e = 0;
            o->d8c = 0;
            o->ba7 = 0;
            o->w1e = 1;
            o->b0d = 0;
            {
                int t = D_1F8002D0[0];
                void *a = D_80132310[0];
                o->d3c = t;
                o->anim = a;
            }
            break;
        case 1:
            switch (o->subtype) {
            case 0:
            case 1:
            case 2:
                switch (o->state) {
                case 0:
                    if (((D_1F8001F8 + D_1F800198) & 3) == 0 && onscreen(o)) o->state++;
                    break;
                case 1:
                    o->step = 0;
                    o->state = 0;
                    o->anim = 0;
                    o->wb4 = 0;
                    o->w98 = 3;
                    o->b04++;
                    break;
                }
                break;
            case 3:
                switch (o->state) {
                case 0:
                    o->active = 2;
                    o->timer = D_8012F49C[o->b0c & 7];
                    o->state++;
                case 1:
                    if (--o->timer == -1) {
                        o->step = 0;
                        o->state = 0;
                        o->b04++;
                    }
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 1:
        ObjCullRegister(o);
        if (!(o->b68 | o->b9e)) {
            switch (o->subtype) {
            case 0:
                switch (o->step) {
                case 0:
                    func_80128D78(o);
                    break;
                case 1:
                    func_80129F7C(o);
                    break;
                }
                break;
            case 1:
                switch (o->step) {
                case 0:
                    func_80129048(o);
                    break;
                case 1:
                    func_801291C4(o);
                    break;
                case 2:
                    func_801297D8(o);
                    break;
                }
                break;
            case 2:
                switch (o->step) {
                case 0:
                    o->active = 1;
                    o->b9c = 0;
                    FUN_8001f8e4(o);
                    o->box0 = 0xa;
                    o->box1 = 0x14;
                    o->box2 = 0xc;
                    o->box3 = 0x26;
                    o->wac = 0x1b;
                    o->step++;
                    o->anim = D_8013237C[0];
                    AnimLoadDuration(o);
                    break;
                case 1:
                    if (o->visible) {
                        AnimAdvance(o);
                        if (((D_1F8001F8 + D_1F800198) & 0x1f) == 0) FUN_8001e4f0(0xad);
                    }
                    break;
                }
                break;
            case 3:
                func_80128764(o);
                break;
            }
            if (!onscreen(o)) o->b04 = 3;
        } else if (o->b9e) {
            if (o->b9f == 0) {
                ((E *)o)->wd2 = o->h->p.whole;
                o->b9f = 15;
            } else {
                o->h->p.whole = (short)(((E *)o)->wd2 - 2) + (Rand() & 3);
                if (--o->b9f == 0) {
                    o->b9e = 0;
                    o->h->p.whole = ((E *)o)->wd2;
                }
            }
        } else if (*(unsigned short *)&o->wb4 == 0 && o->w98 == 0) {
            o->b68 = 0;
            o->wb4 = 1;
            e = ObjAlloc();
            if (e != 0) {
                e->active = 2;
                e->type = 0x48;
                e->animFrame = o->w7a;
                if (o->animFrame) e->a.p.whole = o->a.p.whole - 0xd;
                else e->a.p.whole = o->a.p.whole + 0xd;
                e->y.p.whole = o->y.p.whole - 2;
                e->b.p.whole = o->b.p.whole;
            }
            switch (o->wac) {
            case 11:
                n = 0xf;
                break;
            case 12:
                n = 0x10;
                break;
            case 19:
                n = 0x1d;
                break;
            case 0:
            case 22:
                n = 1;
                break;
            case 36:
                n = 0x25;
                break;
            default:
                n = -1;
                break;
            }
            if ((short)n >= 0) {
                k = (short)n;
                k *= 4;
                p = (unsigned char *)D_8012F3D4 + k;
                goto set;
            }
        } else if (o->w9a == 0) {
            short f = o->w7a;
            int w = o->wac;
            int hx = o->h->p.whole;
            o->w9a = 0xf;
            o->animFrame = 1 - f;
            o->d34 = w;
            o->box0 = D_8012F3E8;
            o->box1 = D_8012F3E9;
            o->box2 = D_8012F3EA;
            o->box3 = D_8012F3EB;
            o->wac = 5;
            o->d30 = hx;
            o->anim = D_80132310[5];
            AnimLoadDuration(o);
        } else {
            AnimAdvance(o);
            if (o->w7a) o->d30--;
            else o->d30++;
            o->h->p.whole = (short)(o->d30 - 2) + (Rand() & 3);
            blocked(o, o->w7a);
            yy = o->y.p.whole;
            o->y.p.whole = yy + 2;
            if (o->b69 == 1 || FUN_80040278(o, o->h->p.whole, (short)(yy + 0x1c))) o->b69 = 0;
            if (--o->w9a == 0) {
                p = (unsigned char *)D_8012F3D4;
                n = o->d34;
                o->b68 = 0;
                k = n << 16;
                k >>= 14;
                p += k;
            set:
                c = *p++;
                o->box0 = c;
                c = *p++;
                o->box1 = c;
                c = *p++;
                o->box2 = c;
                o->box3 = *p++;
                o->wac = n;
                o->anim = *(void **)((char *)D_80132310 + k);
                AnimLoadDuration(o);
            }
        }
        o->b9d = 0;
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_8012A664(o);
            if (!onscreen(o)) o->b04 = 3;
            break;
        case 1:
            switch (o->state) {
            case 0:
                FUN_8001e4f0(0xa7);
                o->state++;
                o->b9c = 0;
                {
                    int b0 = D_8012F3D4[3 * 4 + 0];
                    int b1 = D_8012F3D4[3 * 4 + 1];
                    int b2 = D_8012F3D4[3 * 4 + 2];
                    int b3 = D_8012F3D4[3 * 4 + 3];
                    o->wac = 3;
                    o->box0 = b0;
                    o->box1 = b1;
                    o->box2 = b2;
                    o->box3 = b3;
                }
                o->anim = D_80132310[3];
                AnimLoadDuration(o);
                o->box2 = 0xc;
                o->box3 = 0x26;
                break;
            case 1:
                break;
            case 2:
                o->state++;
                break;
            }
            break;
        case 2:
            func_8012AAE4(o);
            if (!o->visible) o->b04 = 3;
            break;
        }
        break;
    case 3:
        o->timer = 0xf0;
        o->b0b = 0;
        *(signed char *)&o->b0f = -9;
        o->d8c = 0;
        o->b68 = 0;
        o->b9e = 0;
        o->b04++;
        break;
    case 4:
        if (o->timer == 0) {
            if (((D_1F8001F8 + D_1F800198) & 0xf) == 0 && !FUN_800203dc(o)) {
                q->a = 0;
                o->w98 = 3;
                o->subtype = q->sub;
                o->w9a = 0;
                o->b68 = 0;
                o->b9e = 0;
                o->a.p.whole = q->x;
                o->y.p.whole = q->y;
                o->b.p.whole = q->z;
                o->b04 = 0;
                o->step = 1;
                o->state = 0;
                o->substep = 0;
            }
        } else
            o->timer--;
        break;
    }
}
