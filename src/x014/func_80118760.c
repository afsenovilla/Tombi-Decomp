// FUNC 80118760 652 X014
// MATCHING 80118760 652
#include "TOBJ.H"

extern unsigned short D_1F8001F8;
extern TObj *ObjAlloc(void);
extern void ObjFree(TObj *);
extern void func_80117AA8(TObj *);
extern void func_80117D94(TObj *);
extern void func_80118048(TObj *);
extern void func_80118390(TObj *);
extern void func_80118544(TObj *);

void func_80118760(TObj *o)
{
    TObj *n;
    int i;

    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        switch (o->subtype) {
        case 0:
            func_80117AA8(o);
            break;
        case 1:
            func_80117D94(o);
            break;
        case 2:
            func_80118048(o);
            break;
        case 3:
            if (--o->timer == -1) {
                o->b04 = 3;
            }
            for (i = 0; i < 2; i++) {
                n = ObjAlloc();
                if (n != 0) {
                    n->active = 1;
                    n->type = 0x61;
                    n->subtype = 4;
                    n->b0c = o->timer;
                    n->b0a = 0x14;
                    n->h->p.whole = o->h->p.whole;
                    n->y.p.whole = o->y.p.whole;
                    n->d->p.whole = o->d->p.whole;
                }
            }
            break;
        case 4:
            func_80118390(o);
            break;
        case 5:
            switch (o->step) {
            case 0:
                o->step++;
                o->timer = 0;
                break;
            case 1:
                if ((D_1F8001F8 & 3) == 0) {
                    n = ObjAlloc();
                    if (n != 0) {
                        n->active = 1;
                        n->type = 0x61;
                        n->subtype = 6;
                        n->b0a = 0x16;
                        n->b0c = o->timer;
                        n->h->p.whole = o->h->p.whole;
                        n->y.p.whole = o->y.p.whole;
                        n->d->p.whole = o->d->p.whole;
                        o->timer++;
                    }
                }
                break;
            }
            break;
        case 6:
            func_80118544(o);
            break;
        }
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
