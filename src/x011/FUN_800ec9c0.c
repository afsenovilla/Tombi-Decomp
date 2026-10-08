// FUNC 800ec9c0 968 X011
// MATCHING 800ec9c0 968
#include "TOBJ.H"
typedef struct { short vx, vy, vz, pad; } SVec;
#define SV(o, i) (((SVec *)((char *)(o) + 0xb4))[i])
#define BA5(o) (*(signed char *)&(o)->ba5)
extern unsigned short DAT_1f8001c8;
extern unsigned short DAT_1f80016a;
extern unsigned short DAT_1f80016e;
extern unsigned short DAT_1f800172;
extern short DAT_8007a3f0[];
extern short DAT_8007a5f0[];
extern unsigned char DAT_800a6047;
extern void FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void FUN_800ec9c0(TObj *o)
{
    SVec *p = &SV(o, 0);
    SVec *q = &SV(o, 1);
    short t;
    int k;
    switch (o->b04) {
    case 0:
        o->b04++;
        SV(o, 0).vx = -4;
        SV(o, 0).vy = 0;
        SV(o, 0).vz = 0;
        SV(o, 1).vx = 4;
        SV(o, 1).vy = 0;
        SV(o, 1).vz = 0;
        SV(o, 2).vx = -4;
        SV(o, 2).vy = 0;
        SV(o, 2).vz = 0;
        SV(o, 3).vx = 4;
        SV(o, 3).vy = 0;
        SV(o, 3).vz = 0;
        break;
    case 1:
        switch (DAT_1f8001c8 & 1) {
        case 0:
            o->animFrame = (o->animFrame + 3) & 0xff;
            o->d88 = ((o->animFrame - 0x40) & 0xff) << 4;
            k = (DAT_8007a3f0[(unsigned char)o->animFrame] * 3 >> 11) - 0x10;
            o->y.p.whole = DAT_1f80016e - k;
            break;
        case 1:
            o->animFrame = (o->animFrame - 3) & 0xff;
            o->d88 = ((o->animFrame - 0x80) & 0xff) << 4;
            k = (DAT_8007a3f0[(unsigned char)o->animFrame] * 3 >> 11) + 0x10;
            o->y.p.whole = DAT_1f80016e + k;
            break;
        }
        o->h->p.whole = DAT_1f80016a + (DAT_8007a5f0[(unsigned char)o->animFrame] * BA5(o) >> 12);
        o->d->p.whole = DAT_1f800172 + (DAT_8007a3f0[(unsigned char)o->animFrame] * BA5(o) >> 12);
        o->b0f = DAT_800a6047 - 1;
        t = o->timer;
        if (t >= 0x32) {
            if (t % 7 == 0)
                o->ba5++;
            p->vy -= 2;
            q->vy -= 2;
        } else {
            switch (o->subtype) {
            case 0:
                o->d30 -= 6;
                o->d34 -= 1;
                o->d38 -= 1;
                break;
            case 1:
                o->d30 -= 1;
                o->d34 -= 1;
                o->d38 -= 6;
                break;
            case 2:
                o->d30 -= 1;
                o->d34 -= 6;
                o->d38 -= 1;
                break;
            }
            if (o->d30 < 0)
                o->d30 = 0;
            if (o->d34 < 0)
                o->d34 = 0;
            if (o->d38 < 0)
                o->d38 = 0;
        }
        FUN_800202b4(o);
        if (--o->timer == -1)
            o->b04 = 3;
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
