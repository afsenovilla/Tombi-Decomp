// FUNC 80118304 236 X006
// MATCHING 80118304 236
#include "TOBJ.H"
typedef struct { short k, dx, dy, pad; } E8;
extern E8 *D_8011F3CC[];
extern TObj *ObjAlloc(void);

void func_80118304(short sub, short x, short y)
{
    E8 *e = D_8011F3CC[sub];
    TObj *o;

    while (e->k != -1) {
        o = ObjAlloc();
        if (o != 0) {
            o->active = 1;
            o->type = 0x56;
            o->h->raw = (x + e->dx) << 16;
            o->y.raw = (y + e->dy) << 16;
            o->d->raw = 0;
            o->subtype = sub;
            o->b0c = e->k;
        }
        e++;
    }
}
