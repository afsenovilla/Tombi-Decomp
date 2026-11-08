// FUNC 8006acf4 84 MAIN0
// MATCHING 8006acf4 84
// VERSION SCUS_94236 scus94236 8006A2A4 84
// VERSION BETA_980203 beta980203 800689E8 84
#if defined(VERSION_SCUS_94236)
// USA version of PAL main0 8006ACF4 (padQueueMainModeCmd.c): PAL C of src/padQueueMainModeCmd.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { char p[0x2c]; unsigned char *ptr; char q[5]; unsigned char b36; unsigned char b37; char r[0x47-0x38]; unsigned char k; } O;
void padQueueMainModeCmd(O *o)
{
    int k;
    k = o->k;
    switch (k) {
    case 2:
        o->b37 = 0x44;
        o->ptr = (unsigned char *)o + 0x51;
        o->b36 = k;
        break;
    case 3:
        o->b37 = 0x4d;
        o->ptr = (unsigned char *)o + 0x5d;
        o->b36 = 6;
        break;
    }
}
#elif defined(VERSION_BETA_980203)
// Beta version of PAL main0 8006ACF4 (padQueueMainModeCmd.c): SCUS_94236 branch of src/padQueueMainModeCmd.c [-O2 -G0] (tools/demo_sweep.py)
// USA version of PAL main0 8006ACF4 (padQueueMainModeCmd.c): PAL C of src/padQueueMainModeCmd.c (1.000 masked), edited (tools/jp_autofix.py edits) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { char p[0x2c]; unsigned char *ptr; char q[5]; unsigned char b36; unsigned char b37; char r[0x47-0x38]; unsigned char k; } O;
void padQueueMainModeCmd(O *o)
{
    int k;
    k = o->k;
    switch (k) {
    case 2:
        o->b37 = 0x44;
        o->ptr = (unsigned char *)o + 0x51;
        o->b36 = k;
        break;
    case 3:
        o->b37 = 0x4d;
        o->ptr = (unsigned char *)o + 0x5d;
        o->b36 = 6;
        break;
    }
}
#else
typedef struct { char p[0x2c]; unsigned char *ptr; char q[6]; unsigned char b36; unsigned char b37; char r[0x46-0x38]; unsigned char k; } O;
void padQueueMainModeCmd(O *o)
{
    int k;
    k = o->k;
    switch (k) {
    case 2:
        o->b37 = 0x44;
        o->ptr = (unsigned char *)o + 0x51;
        o->b36 = k;
        break;
    case 3:
        o->b37 = 0x4d;
        o->ptr = (unsigned char *)o + 0x5d;
        o->b36 = 6;
        break;
    }
}
#endif
