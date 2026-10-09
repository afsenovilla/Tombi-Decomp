// FUNC 8011d544 704 X004
// MATCHING 8011d544 704
#include "TOBJ.H"
extern void *D_80134D50[];
extern int D_1F8002D4[];
extern unsigned short D_1F800176;
extern unsigned short D_1F800186;
extern int D_800A4570;
extern int D_800A4574;
extern unsigned char D_8009D2C3;
extern unsigned char D_8009C975;
extern unsigned char D_8009C93C;
extern void func_8011D804(TObj *);
extern void FUN_80059d44(short);
extern void playSFX(int);
extern void ObjListPush_1F80022C(TObj *);
extern void FUN_80018934(TObj *);

static __inline__ void place(TObj *o)
{
    int x;
    short a;
    short y;

    {
        int d = o->d30;
        x = ((unsigned int)((d - D_1F800176 + ((D_800A4574 >> 8) << 4)) << 21)) >> 23;
    }
    a = x;
    if (x >= 0x180 && x <= 0x1c0) {
        return;
    }
    if (x > 0x1c0) {
        a = -(unsigned char)(~x + 1);
    }
    {
        unsigned short s = D_1F800186;
        int g = D_800A4570;
        y = (short)(o->d34 - s - ((g >> 8) << 3)) >> 2;
    }
    *(short *)((char *)o + 0x12) = a;
    o->b.p.whole = 0;
    o->visible = 1;
    o->y.p.whole = y;
    ObjListPush_1F80022C(o);
}

void func_8011D544(TObj *o)
{
    unsigned char t;
    int pa;
    int dd;
    int c;

    t = o->b04;
    switch (t) {
    case 0:
        o->b0d = 0;
        o->w1e = 10;
        o->y.p.whole += 0x40;
        o->anim = D_80134D50[o->b0c];
        dd = D_1F8002D4[0];
        pa = o->a.p.whole;
        o->d30 = pa;
        o->d34 = o->y.p.whole;
        o->b6b = 0;
        o->animFrame = 0;
        o->timer = 0xe8;
        o->b04++;
        o->w22 = 0x3c;
        o->d3c = dd;
        break;
    case 1:
        if (!(D_8009D2C3 & 8) && o->subtype) {
            func_8011D804(o);
            switch (o->step) {
            case 0:
                if (D_8009C975 == 0) {
                    D_8009C93C = 1;
                    o->animTimer = 0x17c;
                    o->animFrame = 0x80;
                    o->step++;
                }
                break;
            case 1:
                FUN_80059d44(o->animFrame);
                o->animFrame = (o->animFrame - 0x10) & 0xff;
                if (o->animFrame == 0) {
                    playSFX(0x82);
                    o->animTimer = 0x17c;
                    o->step++;
                }
                if (D_8009C975 == 3) {
                    o->step = 3;
                }
                break;
            case 2:
                c = o->animTimer;
                o->animTimer = c - 1;
                if (c == 0) {
                    o->animFrame = 0x80;
                    o->step--;
                }
                if (D_8009C975 == 3) {
                    o->step = 3;
                }
                break;
            }
        }
        place(o);
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
