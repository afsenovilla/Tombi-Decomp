// VERSION PCPX_96084 tomba 8001AC5C 60
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80018C2C (ObjListPush_1F800218.c): PAL C of src/ObjListPush_1F800218.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
#define ONSCREEN_OBJECT_COUNT D_1F8002E2
#define ONSCREEN_OBJECT_LIST D_1F800320
extern int **ONSCREEN_OBJECT_LIST;
extern unsigned short ONSCREEN_OBJECT_COUNT;

void ObjListPush_1F800218(int *p)
{
    *--ONSCREEN_OBJECT_LIST = p;
    ONSCREEN_OBJECT_COUNT++;
}
#endif
