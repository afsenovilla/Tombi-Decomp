// FUNC 80018934 76 MAIN0
// MATCHING 80018934 76
extern int *FREE_OBJECTS_LAYER7;
extern unsigned short FREE_OBJECT_COUNT_LAYER7;

void freeObjectLayer7(volatile int *o)
{
    int *p;
    unsigned char c = ((volatile char *)o)[0x1c];
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((volatile char *)o)[0x1c] = c & 0x7f;
    FREE_OBJECT_COUNT_LAYER7 = FREE_OBJECT_COUNT_LAYER7 + 1;
    p = FREE_OBJECTS_LAYER7;
    FREE_OBJECTS_LAYER7 = p - 1;
    p[-1] = (int)o;
}
