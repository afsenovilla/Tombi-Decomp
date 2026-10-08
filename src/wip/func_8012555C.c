// FUNC 8012555c 664 X000
// WIP score 40: dy assigned inside the y test (box3 sum < expr form); game evaluates the dy expression first and copies it to a1 after the adds; here box3 sum is evaluated first. Tried: (u16)expr > sum (58, right order but dy in t5), dy after test (47, extra move), int dy, e temp, e0/e1 locals.
#include "TOBJ.H"

extern unsigned char D_1F8001A4;
void func_80043C74();
void FUN_8004258c(TObj *o, int n);

void func_8012555C(TObj *o, TObj *p)
{
    short dx, wx, px, sx, cx, dy;

    if (p->subtype == 0) {
        func_80043C74(o, p);
        return;
    }
    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return;
    wx = p->box0 + o->box0;
    px = wx;
    dx = o->h->p.whole - p->h->p.whole;
    if ((unsigned short)(dx + wx) > p->box1 + o->box1)
        return;
    if (o->box3 + p->box3 < (unsigned short)((dy = o->y.p.whole - p->y.p.whole) + (p->box2 + o->box2)))
        return;
    sx = dx;
    cx = px;
    if (dx < 0) {
        dx = -dx;
        px = -px;
    } else {
        px = (p->box1 - p->box0) + (o->box1 - o->box0);
        cx = px;
    }
    if ((unsigned short)(cx - dx) < 5) {
        o->h->p.whole = p->h->p.whole + px;
        return;
    }
    if (dy <= 0) {
        if (*(unsigned char *)&o->wac == 2) {
            short a = p->box2, b = p->y.p.whole, c = o->box2;
            o->y.p.frac = 0;
            o->b69 = 1;
            o->y.p.whole = b - (a + c);
        } else if (!(o->active & 2) && p->b6a != 0) {
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
                FUN_8004258c(o, 1);
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
}
