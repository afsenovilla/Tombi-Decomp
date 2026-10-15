// FUNC 80118304 236 X006
// MATCHING 80118304 236
#include "TOBJ.H"
typedef struct { short k, dx, dy, pad; } E8;
extern char D_8011F2F4[];
extern char D_8011F304[];
extern char D_8011F314[];
extern char D_8011F324[];
extern char D_8011F334[];
static char D_8011F374[40] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0x28, 0, 0, 0, 0, 0,
    2, 0, 0x50, 0, 0, 0, 0, 0, 3, 0, 0x78, 0, 0, 0, 0, 0,
    -1, -1,
};
static char D_8011F39C[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0x28, 0, 0, 0, 0, 0,
    2, 0, 0x50, 0, 0, 0, 0, 0, 3, 0, 0x78, 0, 0, 0, 0, 0,
    4, 0, -0x60, 0, 0, 0, 0, 0, -1, -1,
};
static E8 *D_8011F3CC[7] = {
    (E8 *)D_8011F2F4, (E8 *)D_8011F304, (E8 *)D_8011F314, (E8 *)D_8011F324,
    (E8 *)D_8011F334, (E8 *)D_8011F374, (E8 *)D_8011F39C,
};
extern TObj *ObjAlloc(void);

void spawnRaceMessage(short sub, short x, short y)
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
