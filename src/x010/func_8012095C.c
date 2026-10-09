// FUNC 8012095c 812 X010
// MATCHING 8012095c 812
#include "TOBJ.H"

extern unsigned char D_1F8001A4;
short func_80043C74();
void FUN_8004258c(TObj *o, int n);

void func_8012095C(TObj *o, TObj *p)
{
    short d, w, t, e0, e1, sx, c;
    int dy, dd;
    unsigned short u;

    if (p->subtype == 0) {
        if (!func_80043C74(o, p)) return;
        if (o->active & 2) return;
        if (D_1F8001A4) return;
        goto kill;
    }
    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return;
    w = p->box0 + o->box0;
    t = w;
    d = o->h->p.whole - p->h->p.whole;
    e0 = p->box1; e1 = o->box1;
    if ((unsigned short)(d + w) > e0 + e1)
        return;
    dd = (unsigned short)o->y.p.whole - (unsigned short)p->y.p.whole;
    u = dd + (p->box2 + o->box2);
    dy = dd;
    if (u > o->box3 + p->box3)
        return;
    sx = d;
    c = t;
    if ((d << 16) < 0) {
        d = -d;
        t = -t;
    } else {
        t = (e0 - p->box0) + (e1 - o->box0);
        c = t;
    }
    if ((unsigned short)(c - d) < 5) {
        o->h->p.whole = p->h->p.whole + t;
        if (o->active & 2) return;
        if (D_1F8001A4) return;
        goto kill;
    }
    if ((short)dy <= 0) {
        if (*(unsigned char *)&o->wac == 2) {
            short a = p->box2, b = p->y.p.whole, c = o->box2;
            o->y.p.frac = 0;
            o->b69 = 1;
            o->y.p.whole = b - (a + c);
        } else if (!(o->active & 2)) {
            if (p->b6a == 0) goto kill;
            if (D_1F8001A4 == 0) {
                TObj *a = (TObj *)p->d90;
                TObj *b = (TObj *)p->d94;
                p->b6a = 0;
                p->b69 = 1;
                a->b69 = 1;
                b->b69 = 1;
                p->active = 2;
                a->active = 2;
                b->active = 2;
                o->active = 2;
                o->b04 = 2;
                o->step = 1;
                o->state = 0;
                FUN_8004258c(o, 4);
            }
        } else {
            short a = p->box2, b = p->y.p.whole, c = o->box2;
            unsigned char t;
            o->b69 = 1;
            t = o->ba6;
            o->y.p.frac = 0;
            o->y.p.whole = b - (a + c);
            if (t == 0) {
                if (sx >= 0) {
                    o->bbe = 8;
                    o->wb0 = 2;
                } else {
                    o->bbe = 9;
                    o->wb0 = -2;
                }
            }
        }
        return;
    }
    o->y.p.whole = p->y.p.whole + ((p->box3 - p->box2) + (o->box3 - o->box2));
    if (o->velY < 0)
        o->velY = 0;
    if (o->active & 2) return;
kill:
    { int t;
    o->active = 2;
    t = p->h->p.whole > o->h->p.whole;
    o->b04 = 2;
    o->step = 0;
    o->state = 0;
    o->animFrame = t;
    FUN_8004258c(o, 2); }
}
