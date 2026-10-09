// FUNC 8011f644 3872 X010
// MATCHING 8011f644 3872
#include "TOBJ.H"
extern volatile unsigned short D_8009D670[];
extern void playSFX(int);

#define XFIX() \
    if (dx < 0) { pw = w; dx = -dx; w = -w; } \
    else { w = (b->box1 - b->box0) + (a->box1 - a->box0); pw = w; }

#define YFIX() \
    if (dy < 0) { ph = h; dy = -dy; h = -h; } \
    else { h = (b->box3 - b->box2) + (a->box3 - a->box2); ph = h; }

#define LAND() \
    if (a->step == 4 || a->step == 0x41) a->step = 0x41; \
    else a->step = 0x3f; \
    a->state = 0;

#define HIT() \
    a->state = 0; \
    if (a->step == 4 || a->step == 0x41) a->step = 0x41; \
    else if (a->step == 10) { a->step = 4; a->state = 3; } \
    else a->step = 0x3f;

void func_8011F644(TObj *a, TObj *b)
{
    short w, dx, dy, h, pw, ph, d, k;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return;
    w = b->box0 + a->box0;
    dx = a->h->p.whole - b->h->p.whole;
    if ((unsigned short)(dx + w) > b->box1 + a->box1) return;
    dy = a->y.p.whole - b->y.p.whole;
    h = b->box2 + a->box2;
    if ((unsigned short)(dy + h) > a->box3 + b->box3) return;

    switch (b->subtype) {
    case 0: case 3: case 4: case 9: case 10: case 11:
        XFIX();
        YFIX();
        if (pw - dx < ph - dy) {
side:
            a->h->p.whole = b->h->p.whole + w;
            if (w < 0) a->ba6 = 2;
            else a->ba6 = 3;
            return;
        }
        if (h <= 0) {
            a->y.p.whole = b->y.p.whole + h;
            if (a->b9c & 1) return;
            playSFX(0xa0);
            a->b9c = 1;
            a->wb0 = 0;
            a->y.p.frac = 0;
            if (a->b04 == 1) {
                LAND();
                if (D_8009D670[0] & 0x20) a->velX = 0x280;
                else if (D_8009D670[0] & 0x80) a->velX = -0x280;
                else a->velX = 0;
                if (D_8009D670[0] & 0x40) a->velY = -0x480;
        else a->velY = -0x680;
            }
            b->b69 = 1;
            return;
        }
        if (a->b9c == 2) return;
        a->b9c = 2;
        a->y.p.whole = b->y.p.whole + h;
        a->wb0 = 0;
        a->y.p.frac = 0;
        if (a->b04 != 1) return;
        HIT();
        playSFX(0xa0);
        a->velY = 0x200;
        return;
    case 1: case 7:
        XFIX();
        if ((unsigned short)(pw - dx) < 4) goto side;
        if (a->b9c & 1) return;
        { short l = b->h->p.whole - b->box0; d = a->h->p.whole - l; }
        k = 0;
        if (d > 0) k = d * (b->box2 + 8) / b->box1;
        if ((b->y.raw >> 16) - k > a->y.p.whole + a->box2) return;
        playSFX(0xa1);
        a->y.p.whole = b->y.p.whole - k - a->box2;
        a->b9c = 1;
        a->wb0 = 0;
        a->y.p.frac = 0;
        if (a->b04 != 1) return;
        LAND();
        if (D_8009D670[0] & 0x20) a->velX = 0x180;
        else if (D_8009D670[0] & 0x80) a->velX = -0x400;
        else a->velX = -0x300;
        if (D_8009D670[0] & 0x40) a->velY = -0x580;
        else a->velY = -0x780;
        return;
    case 5:
        XFIX();
        if ((unsigned short)(pw - dx) < 4) goto side;
        if (a->b9c & 1) return;
        d = (b->h->p.whole + (b->box1 - b->box0)) - a->h->p.whole;
        k = 0;
        if (d > 0) k = d * (b->box2 + 8) / b->box1;
        if ((b->y.raw >> 16) - k > a->y.p.whole + a->box2) return;
        playSFX(0x9f);
        a->y.p.whole = b->y.p.whole - k - a->box2;
        a->b9c = 1;
        a->wb0 = 0;
        a->y.p.frac = 0;
        if (a->b04 != 1) return;
        LAND();
        if (D_8009D670[0] & 0x20) a->velX = 0x300;
        else if (D_8009D670[0] & 0x80) a->velX = -0x100;
        else a->velX = 0x200;
        if (D_8009D670[0] & 0x40) a->velY = -0x400;
        else a->velY = -0x600;
        return;
    case 2:
        XFIX();
        if ((unsigned short)(pw - dx) < 4) goto side;
        if (b->y.p.whole < (a->y.raw >> 16) + 0x10) {
            d = (b->h->p.whole + (b->box1 - b->box0)) - a->h->p.whole;
            k = 0;
            if (d > 0) k = d * (b->box3 - b->box2) / b->box1;
            if ((b->y.raw >> 16) + k < a->y.p.whole - (a->box3 - a->box2)) return;
            playSFX(0x9f);
            a->y.p.whole = b->y.p.whole + k + (a->box3 - a->box2);
            a->b9c = 2;
            a->wb0 = 0;
            a->y.p.frac = 0;
            if (a->b04 != 1) return;
            HIT();
            if (D_8009D670[0] & 0x20) a->velX = 0x300;
        else if (D_8009D670[0] & 0x80) a->velX = -0x100;
        else a->velX = 0x200;
            a->velY = 0x300;
            return;
        }
        if (a->b9c & 1) return;
        { short l = b->h->p.whole - b->box0; d = a->h->p.whole - l; }
        k = 0;
        if (d > 0) k = d * (b->box2 + 8) / b->box1;
        if ((b->y.raw >> 16) - k > a->y.p.whole + a->box2) return;
        playSFX(0x9f);
        a->y.p.whole = b->y.p.whole - k - a->box2;
        a->b9c = 1;
        a->wb0 = 0;
        a->y.p.frac = 0;
        if (a->b04 != 1) return;
        LAND();
        if (D_8009D670[0] & 0x20) a->velX = 0x100;
        else if (D_8009D670[0] & 0x80) a->velX = -0x300;
        else a->velX = -0x200;
        if (D_8009D670[0] & 0x40) a->velY = -0x400;
        else a->velY = -0x600;
        return;
    case 6: case 8:
        XFIX();
        YFIX();
        if (pw - dx < ph - dy) {
            playSFX(0xa0);
            a->h->p.whole = b->h->p.whole + w;
            if (a->b9c == 0) return;
            a->velX = (w < 0) ? -0x300 : 0x300;
            return;
        }
        if (h <= 0) {
            if (a->b9c & 1) return;
            a->y.p.whole = b->y.p.whole + h;
            a->wb0 = 0;
            a->y.p.frac = 0;
            a->b69 = 1;
            a->velY = 0;
            b->b69 = 1;
            return;
        }
        a->y.p.whole = b->y.p.whole + h;
        if (a->velY < 0) a->velY = 0;
        return;
    case 12:
        XFIX();
        if ((unsigned short)(pw - dx) < 4) {
            a->h->p.whole = b->h->p.whole + w;
            if (w < 0) a->ba6 = 2;
            else a->ba6 = 3;
            return;
        }
        if ((b->y.raw >> 16) < (a->y.raw >> 16) + 0x10) {
            { short l = b->h->p.whole - b->box0; d = a->h->p.whole - l; }
            k = 0;
            if (d > 0) k = d * (b->box3 - b->box2) / b->box1;
            if (b->y.p.whole + k < a->y.p.whole - (a->box3 - a->box2)) return;
            playSFX(0xa0);
            a->y.p.whole = b->y.p.whole + k + (a->box3 - a->box2);
            a->b9c = 2;
            a->wb0 = 0;
            a->y.p.frac = 0;
            if (a->b04 != 1) return;
            HIT();
            if (D_8009D670[0] & 0x20) a->velX = -0x300;
        else if (D_8009D670[0] & 0x80) a->velX = 0x100;
        else a->velX = -0x200;
            a->velY = 0x300;
            return;
        }
        d = (b->h->p.whole + (b->box1 - b->box0)) - a->h->p.whole;
        k = 0;
        if (d > 0) k = d * (b->box2 + 8) / b->box1;
        if (b->y.p.whole - k > a->y.p.whole + a->box2) return;
        a->y.p.whole = b->y.p.whole - k - a->box2;
        if (a->b9c & 1) return;
        playSFX(0xa0);
        a->b9c = 1;
        a->wb0 = 0;
        a->y.p.frac = 0;
        if (a->b04 != 1) return;
        LAND();
        if (D_8009D670[0] & 0x20) a->velX = 0x300;
        else if (D_8009D670[0] & 0x80) a->velX = -0x100;
        else a->velX = 0x200;
        if (D_8009D670[0] & 0x40) a->velY = -0x400;
        else a->velY = -0x600;
        return;
    }
}
