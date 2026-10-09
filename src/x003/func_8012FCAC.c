// FUNC 8012fcac 1028 X003
// MATCHING 8012fcac 1028
#include "TOBJ.H"

extern unsigned char D_8009D087;
extern unsigned char D_8009C93F, D_8009C940, D_8009C941, D_8009C942;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern void *D_80138FC8[], *D_80138FD8[];
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);
extern void FUN_80026e0c(int, int);

static __inline__ void setAnim(TObj *o, void *a)
{
    o->anim = a;
    FUN_8001fe6c(o);
}

void func_8012FCAC(TObj *o)
{
    int m, n;
    Fix16 *q;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        if (D_8009D087 == 0) o->state = 1;
        else if (D_8009D087 == 1) o->state = 8;
        else if (D_8009D087 == 2) o->state = 0x10;
        else if (D_8009D087 == 4) o->b04 = 3;
        break;
    case 1:
        if (o->b68 == 0) break;
        m = 8;
        n = 0;
        q = &o->a;
        o->state++;
        D_8009D087 = 1;
        goto talk;
    case 2:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
        o->state++;
        break;
    case 3:
        AnimAdvance(o);
        o->d90 = FUN_8002dcc8(8, 1, &o->a);
        o->state++;
        break;
    case 4:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
        o->state++;
        FUN_8005a8a8(0xbe, 0, 0);
        o->timer = 300;
        break;
    case 6:
        AnimAdvance(o);
        o->d90 = FUN_8002dcc8(8, 2, &o->a);
        o->state++;
        break;
    case 8:
        if (D_8009C940 && D_8009C941 == 0x8f) {
            o->state = 10;
            break;
        }
        if (o->b68 == 0) break;
        m = 8;
        n = 2;
        q = &o->a;
        o->state++;
        goto talk;
    case 10:
        o->state++;
        D_8009D087 = 2;
        D_8009C940 = 0;
        FUN_80026e0c(0x8f, 1);
        m = 8;
        n = 3;
        q = &o->a;
        goto talk;
    case 11:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
        o->state++;
        break;
    case 12:
        o->state++;
        o->d90 = FUN_8002dcc8(8, 4, &o->a);
        break;
    case 13:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
        o->state++;
        FUN_8005a9a4(0xbe, 0);
        o->timer = 300;
        break;
    case 5:
    case 14:
        if (--o->timer == -1) o->state++;
        break;
    case 16:
        if (o->visible == 0) {
            D_8009D087 = 4;
            o->b04 = 3;
            break;
        }
        if (o->b68 == 0) break;
        m = 8;
        n = 4;
        q = &o->a;
        o->state++;
    talk:
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        o->d90 = FUN_8002dcc8(m, n, q);
        o->wac = 2;
        o->animFrame = o->b68 & 1;
        setAnim(o, D_80138FC8[0]);
        break;
    case 7:
    case 9:
    case 17:
        AnimAdvance(o);
        { TObj *p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        }
    case 15:
        D_800A603C = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_800A603D = 0;
        D_800A603E = 0;
        o->state = 0;
        o->wac = 6;
        o->anim = D_80138FD8[0];
        break;
    }
}
