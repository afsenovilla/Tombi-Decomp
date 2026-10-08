// FUNC 800425c4 764 MAIN0
// MATCHING 800425c4 764
#include "TOBJ.H"
extern TObj *D_8009C934;
extern TObj *D_8009C934_a[]; /* same global */

int func_800425C4(TObj *o, TObj *p, int r)
{
    unsigned char c;
    TObj **d;

    switch (o->type) {
    case 0:
        r = 1;
        o->active = 2;
        o->ba5 = 0;
        o->wa8 = 0x4ff;
        break;
    case 1:
        r = 0;
        o->ba5 = 0;
        o->active = 2;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        break;
    case 10:
        r = 7;
        o->b6a = 1;
        o->active = 2;
        break;
    case 4:
        r = 0;
        o->active = 2;
        o->b04 = 2;
        o->ba5 = 0;
        o->step = 1;
        o->state = 0;
        break;
    case 5:
    case 6:
    case 7:
        o->b6a = 1;
        o->active = 2;
        r = o->type - 1;
        if ((p->category & 0x7f) == 4)
            o->wa8 = 0x4ff;
        break;
    case 8:
        r = 2;
        d = &D_8009C934;
        p->b68 = 0;
        *d = 0;
        o->ba5 = 0;
        o->active = 2;
        if (o->wa8 > 0x500)
            o->wa8 = 0x4ff;
        c = p->category & 0x7f;
        if (c == 4) {
            switch (p->type) {
            case 0x24:
                D_8009C934 = p;
                p->b68 = 1;
                o->b69 = 1;
                o->a.p.whole = p->a.p.whole;
                o->y.p.whole = p->y.p.whole + 0x12;
                break;
            case 2:
            case 4:
            case 5:
            case 0x1d:
                D_8009C934 = p;
                p->b68 = 1;
                o->b69 = 1;
                if (o->y.p.whole < p->y.p.whole)
                    o->y.p.whole = p->y.p.whole + 4;
                break;
            case 3:
            case 6:
            case 0xb:
            case 0x39:
                goto hit;
            default:
                goto miss;
            }
        } else {
            if (c != 2)
                goto miss;
            if (p->type == 0xe)
                goto land;
            if (p->type != 0x38 || p->subtype == 0)
                goto miss;
            *d = p;
            o->a.p.whole = p->a.p.whole;
            o->y.p.whole = p->y.p.whole + 8;
            goto land;
        }
        break;
    case 9:
        r = 3;
        D_8009C934_a[0] = 0;
        o->ba5 = 0;
        o->active = 2;
        if (o->wa8 > 0x500)
            o->wa8 = 0x4ff;
        c = p->category & 0x7f;
        if (c == 4) {
            switch (p->type) {
            case 2:
            case 4:
            case 5:
            case 0x1d:
            case 0x24:
                D_8009C934 = p;
            case 3:
            case 6:
            case 0xb:
            case 0x39:
                break;
            default:
                goto miss;
            }
        hit:
            p->b68 = 1;
            o->b69 = 1;
            if (o->y.p.whole < p->y.p.whole)
                o->y.p.whole = p->y.p.whole + 4;
        } else if (c == 2 && p->type == 0xe) {
        land:
            p->b68 = 1;
            o->b69 = 1;
        } else {
        miss:
            o->b69 = 0;
        }
        break;
    }
    if (o->type != 1 && o->w98 == 2)
        r += 6;
    return r;
}
