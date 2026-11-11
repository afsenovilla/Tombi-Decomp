// VERSION PCPX_96084 tomba 8001A914 76
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80018934 (freeObjectLayer7.c): PAL C of src/freeObjectLayer7.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#define FREE_OBJECTS_LAYER7 D_1F800374
#define FREE_OBJECT_COUNT_LAYER7 D_1F800370
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
#endif
