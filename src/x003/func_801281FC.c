// FUNC 801281fc 1532 X003
// MATCHING 801281fc 1532
#include "TOBJ.H"

typedef struct { short a, b, x, y, z; } Q;
typedef struct { char p[0xd2]; short wd2; } E;
typedef void (*Fn)(TObj *);

extern int D_1F8002DC[];
extern unsigned char D_8009C942;
extern unsigned short D_1F80027E;
extern unsigned short D_1F8001F8;
extern int D_1F800198;
extern Fn D_80135D6C[];
extern unsigned short D_80135D3C[];
extern int ObjCullRegister(TObj *);
extern int Rand(void);
extern short FUN_80040278(TObj *, int, int);
extern int func_801256F4(TObj *, int);
extern void func_80125834(TObj *);
extern void func_801259AC(TObj *);
extern void func_80125CF8(TObj *);
extern void func_8012632C(TObj *);
extern void func_80126B68(TObj *);
extern void func_8012748C(TObj *);
extern void func_80127D48(TObj *);
extern void func_80128060(TObj *);
extern void FUN_80020490(TObj *);
extern void FUN_80018790(TObj *);
extern int FUN_800203dc(TObj *);

void func_801281FC(TObj *o)
{
    Q *q = (Q *)&o->wb4;
    short yy;
    unsigned int v;

    switch (o->b04) {
    case 0:
        if (o->step == 0) {
            q->x = o->h->p.whole;
            q->y = o->y.p.whole;
            q->z = o->d->p.whole;
            o->w1e = 0xb;
            o->step++;
            o->d3c = D_1F8002DC[0];
        } else {
            q->a = 0;
            q->b = 0;
            *(signed char *)&o->b0f = -9;
            o->d84 = 0;
            o->d88 = 0;
            o->b69 = 0;
            o->w98 = 3;
            o->w9a = 0;
            o->b68 = 0;
            o->b9e = 0;
            o->step = 0;
            *(int *)&o->anim = 0;
            o->b04++;
        }
        break;
    case 1:
        if (D_8009C942) {
            ObjCullRegister(o);
            break;
        }
        ObjCullRegister(o);
        if (!(o->b68 | o->b9e)) {
            switch (o->step) {
            case 0:
                D_80135D6C[o->subtype](o);
                break;
            case 1:
                switch (o->state) {
                case 0:
                    func_80125834(o);
                    break;
                case 1:
                    func_801259AC(o);
                    break;
                case 2:
                    func_80125CF8(o);
                    break;
                case 3:
                    func_8012632C(o);
                    break;
                case 4:
                    func_80126B68(o);
                    break;
                }
                break;
            }
        } else {
            if (o->b9e) {
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
            } else {
                if (o->w9a == 0) {
                    o->d30 = o->h->p.whole;
                    o->w9a = 0x14;
                } else {
                    o->h->p.whole = (short)(o->d30 - 2) + (Rand() & 3);
                    if (o->wac != 0x1e && q->b == 0) {
                        if (o->w7a) o->d30--;
                        else o->d30++;
                        if (func_801256F4(o, o->w7a)) {
                            if (o->w7a) o->d30++;
                            else o->d30--;
                        }
                        yy = o->y.p.whole;
                        o->y.p.whole = yy + 2;
                        if (o->b69 == 1) {
                            o->b69 = 0;
                        } else if (FUN_80040278(o, o->h->p.whole, (short)(yy + 0x10))) {
                            if (o->wac >= 0xc) {
                                o->d8c = 0;
                            } else {
                                v = ((D_1F80027E << 2) + o->d8c) & 0xff;
                                if (v != 0) {
                                    if (v < 0x80) o->d8c = o->d8c - 1;
                                    else o->d8c = o->d8c + 1;
                                    o->d8c = *(unsigned char *)&o->d8c;
                                }
                            }
                        }
                    }
                    if (--o->w9a == 0) {
                        o->b68 = 0;
                        if (o->wac == 0x1e || q->b != 0) o->h->p.whole = o->d30;
                    }
                }
            }
        }
        o->b9d = 0;
        break;
    case 2:
        ObjCullRegister(o);
        switch (o->step) {
        case 0:
            func_8012748C(o);
            if (!o->visible) FUN_80020490(o);
            break;
        case 1:
            func_80127D48(o);
            break;
        case 2:
            func_80128060(o);
            if (!o->visible) o->b04 = 3;
            break;
        }
        break;
    case 3:
        if (o->subtype >= 2) {
            (*(short *)o->d90)--;
            FUN_80018790(o);
        } else {
            *(signed char *)&o->b0f = -9;
            o->timer = D_80135D3C[Rand() & 7] + 0x12c;
            o->b0b = 0;
            o->h->p.whole = q->x;
            o->y.p.whole = q->y;
            o->d->p.whole = q->z;
            o->b04++;
        }
        break;
    case 4:
        if (o->timer) {
            o->timer--;
        } else if (((D_1F8001F8 + D_1F800198) & 0x3f) == 0 && !FUN_800203dc(o)) {
            o->active = 1;
            o->b04 = 0;
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
