// VERSION PCPX_96084 tomba 8002FDE4 148
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 8002B020 (updateObjectsLayer8.c): PAL C of src/updateObjectsLayer8.c (0.973 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
extern char OBJECT_POOL_LAYER8[];
extern void (*PTR_DAT_80079b88[])(char *);
extern int UPDATE_OBJECT_INDEX;

void updateObjectsLayer8(void)
{
    char *p = OBJECT_POOL_LAYER8;
    UPDATE_OBJECT_INDEX = 0;
    do {
        if (*p != 0)
            PTR_DAT_80079b88[(unsigned char)p[2]](p);
        UPDATE_OBJECT_INDEX++;
        p += 0xd8;
    } while (UPDATE_OBJECT_INDEX < 0x60);
}
#endif
