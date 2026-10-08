// FUNC 801202a4 496 X000
// MATCHING 801202a4 496
typedef struct O {
    unsigned char active, vis, type, b3, state, step, b6, b7;
    char p0[2]; unsigned char b0a; char p1[2]; unsigned char b0d; char p2; signed char b0f;
    int x, y, z; char p3; unsigned char b1d; unsigned short w1e; char p4[4]; int anim; char p5[6]; unsigned short w2e;
    char p6[0x3c - 0x30]; int d3c; char p7[0x68 - 0x40]; unsigned char b68, b69; char p8[2];
    unsigned short w6c, w6e, w70, w72; char p9[0x90 - 0x74]; struct O *child;
} O;
extern int G_1f8002d4;
extern int D_8013b150;
extern O *FUN_80018448(void);
extern void FUN_8001fe6c(O *), FUN_800202b4(O *), FUN_80120164(O *), FUN_80018838(O *);

void FUN_801202a4(O *o)
{
    O *c;
    int *g;
    switch (o->state) {
    case 0:
        switch (o->step) {
        case 0:
            g = &G_1f8002d4;
            o->w6c = 20;
            o->w6e = 40;
            o->w70 = 12;
            o->w72 = 24;
            o->d3c = *g;
            o->w1e = 8;
            o->b0d = 0;
            o->b0a = 0;
            o->b69 = 0;
            o->b68 = 0;
            o->b0f = 4;
            c = FUN_80018448();
            if (c != 0) {
                c->active = 1;
                c->type = 4;
                c->x = o->x;
                c->y = o->y + 0x100000;
                c->z = o->z;
                c->w1e = o->w1e;
                c->b0f = -10;
                c->b0d = 0;
                c->b0a = 0;
                c->w2e = 1;
                c->d3c = *g;
                c->b1d = o->b1d;
                c->anim = D_8013b150;
                FUN_8001fe6c(c);
                o->child = c;
                c->state = 1;
            }
            o->step++;
            break;
        case 1:
            o->state++;
            o->step = 0;
            o->b6 = 0;
            o->b7 = 0;
            break;
        }
        break;
    case 1:
        FUN_800202b4(o);
        if (o->vis != 0) FUN_80120164(o);
        break;
    case 2:
        o->state = 3;
        break;
    case 3:
        FUN_80018838(o);
        break;
    }
}
