// FUNC 80119f58 360 X010
// MATCHING 80119f58 360
typedef struct { short x, y, z, pad; } P;
typedef struct {
    unsigned char active, b01, b02, b03, b04, step, state, b07;
    char p08[0xf - 8];
    signed char b0f;
    char p10[0xb4 - 0x10];
    P p[4];
} S;

void func_80119F58(S *o)
{
    P *a = &o->p[0];
    P *b = &o->p[1];
    P *c = &o->p[2];
    P *d = &o->p[3];

    if (o->step == 0) {
        o->step++;
        o->active = 2;
        switch (o->b0f) {
        case 0:
            a->x = 0x764; a->y = -0x146;
            b->x = 0x770; b->y = -0x146;
            c->x = 0x858; c->y = -0x50;
            d->x = 0x864; d->y = -0x50;
            break;
        case 1:
            a->x = 0x810; a->y = -0x132;
            b->x = 0x81c; b->y = -0x132;
            c->x = 0x932; c->y = -0x50;
            d->x = 0x93e; d->y = -0x50;
            break;
        case 2:
            a->x = 0x8d2; a->y = -0x12c;
            b->x = 0x8de; b->y = -0x12c;
            c->x = 0x9b0; c->y = -0x50;
            d->x = 0x9bc; d->y = -0x50;
            break;
        case 3:
            a->x = 0x93e; a->y = -0x13c;
            b->x = 0x94a; b->y = -0x13c;
            c->x = 0x9ce; c->y = -0xc6;
            d->x = 0x9da; d->y = -0xc6;
            break;
        case 4:
            a->x = 0x9ce; a->y = -0xc6;
            b->x = 0x9da; b->y = -0xc6;
            c->x = 0xb17; c->y = -0x50;
            d->x = 0xb23; d->y = -0x50;
            break;
        }
        a->z = 0;
        b->z = 0;
        c->z = 0;
        d->z = 0;
        o->b0f = 0;
    }
}
