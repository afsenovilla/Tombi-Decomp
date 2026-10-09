// FUNC 80134944 700 X001
// MATCHING 80134944 700
#include "TOBJ.H"

extern short D_8007A3F0[];
extern short D_8007A5F0[];
extern TObj *D_8009C948;
extern int ObjCullRegister(TObj *);
extern void TileCollideAt(TObj *, int, int);
extern void func_80133A24(TObj *);
extern void func_80133BA4(TObj *);
extern void func_80134070(TObj *);
extern void func_801344FC(TObj *);
extern void func_80134674(TObj *);

void func_80134944(TObj *o)
{
    TObj *e;

    switch (o->step) {
    case 0:
        ObjCullRegister(o);
        if (o->b0c == 0) {
            if (o->w98 == 0) {
                o->b04 = 2;
                o->step = 5;
                o->state = 0;
            } else {
                func_80133BA4(o);
            }
        } else {
            func_80133A24(o);
            o->d->raw = ((TObj *)o->d90)->d->raw;
            o->h->raw = ((TObj *)o->d90)->h->raw;
            o->h->raw -= D_8007A5F0[*(unsigned char *)&o->d84] << 8;
            if (o->b69) {
                o->y.raw = ((TObj *)o->d90)->y.raw;
                o->y.raw -= D_8007A3F0[*(unsigned char *)&((TObj *)*(int *)&o->wa8)->w74] << 8;
            } else {
                o->y.raw = ((TObj *)o->d90)->y.raw;
                o->y.raw -= D_8007A3F0[*(unsigned char *)&o->d84] << 8;
            }
            TileCollideAt(o, o->h->p.whole, o->y.p.whole);
        }
        break;
    case 1:
        ObjCullRegister(o);
        func_80134070(o);
        break;
    case 2:
        if (ObjCullRegister(o) == 0) o->b04 = 3;
        else func_801344FC(o);
        break;
    case 3:
        ObjCullRegister(o);
        break;
    case 4:
        o->b04 = 3;
        (*(short *)&D_8009C948->b0a)--;
        {
            TObj *q;
        for (q = (TObj *)o->d90; q; q = (TObj *)q->d90) q->b04 = 3;
        for (q = (TObj *)o->d94; q; q = (TObj *)q->d94) q->b04 = 3;
        }
        break;
    case 5:
        if (ObjCullRegister(o) == 0) {
            if (o->b0c == 0) {
                o->b04 = 3;
                for (e = (TObj *)o->d94; e; e = (TObj *)e->d94) e->b04 = 3;
                (*(short *)&D_8009C948->b0a)--;
            }
        } else {
            func_80134674(o);
        }
        break;
    }
}
