// VERSION PCPX_96084 tomba 80019528 108
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: SLPS_01144 C of src/func_80016ACC_slps01144.c (func_80016ACC, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
typedef struct { short x, y, w, h; } RECT;
typedef struct { unsigned long mode; RECT *crect; unsigned long *caddr; RECT *prect; unsigned long *paddr; } TIM_IMAGE;
extern void func_8005E62C_slps01144(void);
extern int ReadTIM(TIM_IMAGE *);
extern int DrawSync(int);
extern void LoadImage(RECT *, unsigned long *);
void func_80016ACC(void)
{
    TIM_IMAGE t;
    func_8005E62C_slps01144();
    ReadTIM(&t);
    DrawSync(0);
    LoadImage(t.prect, t.paddr);
    if (t.mode & 8)
        LoadImage(t.crect, t.caddr);
    DrawSync(0);
}
#endif
