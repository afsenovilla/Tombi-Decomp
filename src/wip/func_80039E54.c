// FUNC 80039e54 848 MAIN0
#include "TOBJ.H"
typedef struct { char p0[0x8a]; unsigned short c; char p1[0x1190 - 0x8c]; int idx, k, a, b, cc, d, e, f; } G;
typedef struct { short p0, x, p1, y, p2, z; } V;
extern G *D_8009F0F0;
extern TObj *D_8009F2D8[];
extern int FUN_8002dcc8(int a, int b, V *v);
extern void func_80113754(TObj *o);
extern void func_800ECBBC(TObj *o);
extern unsigned char D_8009D2B0, D_8009C93F, D_800A603C, D_800A603D, D_800A603E, D_800A60E4, D_8009C942;
extern unsigned short D_800A6066[];

void func_80039E54(void)
{
    G *g = D_8009F0F0;
    TObj *o = D_8009F2D8[g->idx];
    int k = g->k;
    V v;
    int t;
    if (o != 0) {
        switch (k) {
        case 1:
            o->b6a = 1;
            o->timer = g->a;
        case 0: case 9:
            o->b6a = 0;
            o->step = k;
            o->state = 0;
            break;
        case 2:
            o->timer = g->a;
            o->b6a = 1;
            o->step = k;
            o->state = 0;
            o->velX = g->b;
            break;
        case 3:
            o->timer = g->a;
            o->d30 = g->b;
            o->d34 = g->cc;
            o->w98 = g->d;
            o->b6a = 1;
            o->step = k;
            o->state = 0;
            o->w9a = g->e;
            break;
        case 8:
            v.x = g->d;
            v.y = g->e;
            v.z = g->f;
        case 4:
            if (k == 4) v = *(V *)&o->a;
            o->d90 = FUN_8002dcc8(g->a, g->b, &v);
            switch (o->type & 0x7f) {
            case 0x18:
                o->w76 = 0;
                o->w74 = g->cc;
                func_80113754(o);
                D_8009D2B0 = 2;
                D_8009C93F = 1;
                D_800A603C = 5;
                D_800A603D = 0;
                D_800A603E = 0;
                break;
            case 0x19:
                o->b68 = 2;
                D_800A60E4 = 3;
                D_8009D2B0 = 2;
                D_8009C93F = 1;
                t = g->cc;
                if (t & 0x80) {
                    o->w74 = o->animFrame + (t & 0x7f);
                } else {
                    o->animFrame = o->b69 - 1;
                    o->w74 = o->animFrame + g->cc;
                }
                o->w76 = 0;
                func_800ECBBC(o);
                D_800A603C = 1;
                D_800A603D = 2;
                D_800A603E = 0;
                D_800A6066[0] |= 8;
                break;
            }
            D_8009C942 = 1;
            o->b6a = 1;
            o->step = 4;
            o->state = 0;
            break;
        case 5:
            t = o->type & 0x7f;
            if (t == 0x18 || t == 0x19) {
                o->velX = g->a;
                o->velY = g->b;
            }
            o->b6a = 1;
            o->step = k;
            o->state = 0;
            break;
        case 6:
            o->b6a = 0;
            o->step = k;
            o->state = 0;
            break;
        case 7:
            t = o->type & 0x7f;
            if (t == 0x18 || t == 0x19) {
                o->velX = g->a;
                o->velY = g->b;
                o->w78 = g->cc;
            }
            o->b6a = 1;
            o->step = k;
            o->state = 0;
            break;
        }
    }
    g->c++;
}
