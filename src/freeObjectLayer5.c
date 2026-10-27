// FUNC 800188e0 84 MAIN0
// MATCHING 800188e0 84
typedef struct S {
    int a, b, c, d;
    char pad[0x1c - 0x10];
    char e;
    char pad2[0x9c - 0x1d];
    char f, g, h, i;
} S;
extern unsigned short FREE_OBJECT_COUNT;
extern int *FREE_OBJECTS;

void freeObjectLayer5(volatile S *o)
{
    o->a = 0; o->b = 0; o->c = 0; o->d = 0;
    o->e = 0; o->f = 0; o->g = 0; o->h = 0; o->i = 0;
    FREE_OBJECT_COUNT = FREE_OBJECT_COUNT + 1;
    FREE_OBJECTS = FREE_OBJECTS - 1;
    *FREE_OBJECTS = (int)o;
}
