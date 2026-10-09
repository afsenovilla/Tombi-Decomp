// FUNC 80134c00 744 X001
// MATCHING 80134c00 744
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xd2 - 0xc0];
    short wd2;
} P;

extern unsigned char D_8009D2C3, D_8009C942;
extern short D_8007A5F0[], D_8007A3F0[];
extern int Rand(void);
extern int ObjCullRegister(TObj *);
extern int FUN_800203dc(P *);
extern int FUN_800201ac(P *, int);
extern void FUN_80018790(P *);
extern void func_80134EE8(P *);
extern void func_80132B64(P *);
extern void func_80133A24(P *);
extern void func_80134944(P *);

void func_80134C00(P *o)
{
    switch (o->t.b04) {
    case 0:
        switch (o->t.step) {
        case 0:
            if (D_8009D2C3 & 1) {
                o->t.b04 = 3;
                break;
            }
            func_80134EE8(o);
            break;
        case 1:
            if (!FUN_800203dc(o)) {
                o->t.step = 0;
                o->t.b04++;
            }
            break;
        }
        break;
    case 1:
        if (D_8009C942) {
            ObjCullRegister(&o->t);
            break;
        }
        if (o->t.b0c == 0) {
            if (o->t.b9e == 0) {
                switch (o->t.subtype) {
                case 0:
                case 1:
                    func_80132B64(o);
                    break;
                case 2:
                    func_80132B64(o);
                    break;
                case 3:
                    func_80132B64(o);
                    break;
                }
                if ((unsigned)(o->t.d88 - 0x40) < 0x80) o->t.animFrame = 1;
                else o->t.animFrame = 0;
            } else if (o->t.b9f == 0) {
                o->wd2 = o->t.h->p.whole;
                o->t.b9f = 0xf;
            } else {
                o->t.h->p.whole = (short)(o->wd2 - 2) + (Rand() & 3);
                if (--o->t.b9f == 0) {
                    o->t.b9e = 0;
                    o->t.h->p.whole = o->wd2;
                }
            }
        } else {
            func_80133A24(o);
            o->t.h->raw = ((TObj *)o->t.d90)->h->raw;
            o->t.d->raw = ((TObj *)o->t.d90)->d->raw;
            o->t.y.raw = ((TObj *)o->t.d90)->y.raw;
            o->t.h->raw -= D_8007A5F0[*(unsigned char *)&o->t.d84] << 8;
            o->t.y.raw -= D_8007A3F0[*(unsigned char *)&o->t.d84] << 8;
        }
        o->t.b69 = 0;
        if (!FUN_800201ac(o, 0xa0)) {
            if (o->t.y.p.whole >= -0x190) o->t.y.p.whole = -0x190;
        }
        break;
    case 2:
        func_80134944(o);
        o->t.b69 = 0;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
