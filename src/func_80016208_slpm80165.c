// VERSION SLPM_80165 slpm80165 80016208 92
#if defined(VERSION_SLPM_80165)
// Demo-only helper: reads the next TIM and uploads its CLUT.
typedef struct { short x, y, w, h; } RECT;
typedef struct { unsigned long mode; RECT *crect; unsigned long *caddr; RECT *prect; unsigned long *paddr; } TIM_IMAGE;
extern void func_80057238(void);
extern int ReadTIM(TIM_IMAGE *);
extern int DrawSync(int);
extern void LoadImage(RECT *, unsigned long *);
void func_80016208_slpm80165(void)
{
    TIM_IMAGE t;
    func_80057238();
    ReadTIM(&t);
    DrawSync(0);
    if (t.mode & 8)
        LoadImage(t.crect, t.caddr);
    DrawSync(0);
}
#endif
