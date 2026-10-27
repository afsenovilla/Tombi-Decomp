// FUNC 800187e4 84 MAIN0
// MATCHING 800187e4 84
// FLAGS -O2 -G0
extern int *FREE_OBJECTS;
extern unsigned short FREE_OBJECT_COUNT;

void ObjFree(volatile int *o)
{
    int *p;
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((volatile char *)o)[0x1c] = 0;
    ((volatile char *)o)[0x9c] = 0;
    ((volatile char *)o)[0x9d] = 0;
    ((volatile char *)o)[0x9e] = 0;
    ((volatile char *)o)[0x9f] = 0;
    FREE_OBJECT_COUNT = FREE_OBJECT_COUNT + 1;
    p = FREE_OBJECTS;
    FREE_OBJECTS = p - 1;
    p[-1] = (int)o;
}
