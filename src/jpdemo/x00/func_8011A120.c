// JPDEMO x00 8011A120 144
// Demo-only spawner: a type-12 subtype-22 object at the entry's position.
#include "TOBJ.H"
extern unsigned char D_8006DFA7;
extern TObj *func_800171F0(void);
void func_8011A120(short *e)
{
    TObj *o = func_800171F0();
    if (o != 0) {
        o->active = 1;
        o->type = 12;
        o->subtype = 22;
        o->b0c = 0x80;
        o->animFrame = 0;
        o->b0f = D_8006DFA7;
        o->h->raw = e[1] << 16;
        o->y.raw = e[3] << 16;
        o->d->raw = e[5] << 16;
    }
}
