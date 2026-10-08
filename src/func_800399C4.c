// FUNC 800399c4 716 MAIN0
// MATCHING 800399c4 716
#include "TOBJ.H"
typedef struct { char p0[0x8a]; unsigned short c; char p1[0x1190 - 0x8c]; int idx, k, sub, x, y, z; } G;
extern G *D_8009F0F0;
extern TObj *D_8009F2D8[];
extern TObj *FUN_800183b8(void);
extern TObj *FUN_800184d8(void);
extern TObj *ObjAlloc(void);
extern void func_800EDFB8(TObj *o, int n);

void func_800399C4(void)
{
    G *g = D_8009F0F0;
    int k = g->k;
    int idx = g->idx;
    int sub = g->sub;
    int x = g->x;
    int y = g->y;
    int z = g->z;
    switch (k) {
    case 2: case 9: case 10: {
        TObj *o = FUN_800183b8();
        if (o == 0) break;
        if (k == 9) o->active = 1; else o->active = 2;
        if (k == 9) o->type = 0x19; else o->type = 0x18;
        o->animFrame = 0;
        o->subtype = sub;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->b6b = 0x80;
        o->b0c = idx;
        o->b0d = 0;
        o->b68 = 0;
        o->b9c = 0;
        o->_pad0e[0] = 0;
        o->b0f = 0;
        o->w74 = 0;
        o->w76 = 0;
        o->d94 = 0;
        switch (k) {
        case 9:
            o->b0a = 13;
            func_800EDFB8(o, 1);
            break;
        case 10:
            o->b0a = 0x10;
            o->da0 = 0;
            break;
        case 2:
            o->b0a = 2;
            break;
        }
        o->b04 = 0;
        o->step = 0;
        o->state = 0;
        D_8009F2D8[idx] = o;
        break;
    }
    case 3: {
        TObj *o = ObjAlloc();
        if (o == 0) break;
        o->active = 1;
        o->type = 0x2e;
        o->animFrame = 0;
        o->subtype = sub;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->b6b = 0x80;
        o->b0a = 0x10;
        o->b0c = idx;
        o->b0d = 0;
        o->b68 = 0;
        o->b0f = 0;
        o->_pad0e[0] = 0;
        o->w74 = 0;
        o->w76 = 0;
        o->d94 = 0;
        o->b04 = 0;
        o->step = 0;
        o->state = 0;
        D_8009F2D8[idx] = o;
        break;
    }
    case 4: {
        TObj *o = FUN_800184d8();
        unsigned char c;
        if (o == 0) break;
        c = 1;
        o->active = c;
        o->type = 0x1e;
        o->animFrame = 0;
        o->subtype = sub;
        o->a.raw = x << 16;
        o->y.raw = y << 16;
        o->b.raw = z << 16;
        o->b0a = 2;
        o->b0c = idx;
        o->b0d = 0;
        o->b68 = 0;
        o->b6b = c;
        o->b0f = 0;
        o->d94 = 0;
        o->b04 = 0;
        o->step = 0;
        o->state = 0;
        D_8009F2D8[idx] = o;
        break;
    }
    case 5: case 6: case 7: case 8:
        break;
    }
    g->c++;
}
