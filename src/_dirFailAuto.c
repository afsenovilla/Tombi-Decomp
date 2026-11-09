// FUNC 8006be6c 224 MAIN0
// MATCHING 8006be6c 224
// VERSION SCUS_94236 scus94236 8006B3B8 220
// VERSION BETA_980203 beta980203 80069ADC 220
// CC gcc-2.8.1
// FLAGS -O2 -G8
#if defined(VERSION_SCUS_94236)
// USA version of PAL main0 8006BE6C (_dirFailAuto.c): PAL C of src/_dirFailAuto.c (0.937 masked), edited (tools/jp_autofix.py edits) [-O2 -G8] (tools/demo_sweep.py)
extern void (*_padFuncClrInfo)(unsigned char *);

void _dirFailAuto(unsigned char *o)
{
    int s = o[0x46];
    unsigned int t;
    *(int *)(o + 0x4c) = *(int *)(o + 0x4c) + 1;
    if (s == 0)
        goto tail;
    if (s == 1) {
        t = o[0x4a];
        if (t > 1) {
            o[0x49] = 2;
            o[0x46] = 0xff;
            return;
        }
        o[0x4a] = t + 1;
        return;
    }
    t = o[0x4a];
    if (t <= 3) {
        o[0x4a] = t + 1;
        return;
    }
    if (o[0x49] != 0)
        _padFuncClrInfo(o);
tail:
    if (**(unsigned char **)(o + 0x3c) != 0xf3) {
        **(unsigned char **)(o + 0x30) = 0xff;
        (*(unsigned char **)(o + 0x30))[1] = 0;
        o[0xe8] = 0;
    }
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 8006BE6C (_dirFailAuto.c): SCUS_94236 C of src/_dirFailAuto.c (func_8006B3B8_scus94236, twin score 1.100) [-O2 -G8] (tools/demo_sweep.py)
// USA version of PAL main0 8006BE6C (_dirFailAuto.c): PAL C of src/_dirFailAuto.c (0.937 masked), edited (tools/jp_autofix.py edits) [-O2 -G8] (tools/demo_sweep.py)
extern void (*_padFuncClrInfo)(unsigned char *);

void _dirFailAuto(unsigned char *o)
{
    int s = o[0x46];
    unsigned int t;
    *(int *)(o + 0x4c) = *(int *)(o + 0x4c) + 1;
    if (s == 0)
        goto tail;
    if (s == 1) {
        t = o[0x4a];
        if (t > 1) {
            o[0x49] = 2;
            o[0x46] = 0xff;
            return;
        }
        o[0x4a] = t + 1;
        return;
    }
    t = o[0x4a];
    if (t <= 3) {
        o[0x4a] = t + 1;
        return;
    }
    if (o[0x49] != 0)
        _padFuncClrInfo(o);
tail:
    if (**(unsigned char **)(o + 0x3c) != 0xf3) {
        **(unsigned char **)(o + 0x30) = 0xff;
        (*(unsigned char **)(o + 0x30))[1] = 0;
        o[0xe8] = 0;
    }
}
#else
extern void (*_padFuncClrInfo)(unsigned char *);

void _dirFailAuto(unsigned char *o)
{
    int s = o[0x46];
    unsigned int t;
    *(int *)(o + 0x4c) = *(int *)(o + 0x4c) + 1;
    if (s == 0)
        goto tail;
    if (s == 1) {
        t = o[0x4a];
        if (t > 10) {
            o[0x49] = 2;
            o[0x46] = 0xff;
            return;
        }
        o[0x4a] = t + 1;
        return;
    }
    t = o[0x4a];
    if (t <= 10) {
        o[0x4a] = t + 1;
        return;
    }
    if (o[0x49] != 0)
        _padFuncClrInfo(o);
tail:
    if (**(unsigned char **)(o + 0x3c) != 0xf3) {
        **(unsigned char **)(o + 0x30) = 0xff;
        (*(unsigned char **)(o + 0x30))[1] = 0;
        o[0xe8] = 0;
        o[0x35] = 0;
    }
}
#endif
