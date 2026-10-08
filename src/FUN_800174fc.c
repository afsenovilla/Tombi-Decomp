// FUNC 800174fc 244 MAIN0
// MATCHING 800174fc 244
typedef struct { short x, y, w, h; } RECT;
typedef struct { unsigned int mode; RECT *crect; unsigned int *caddr; RECT *prect; unsigned int *paddr; } TIM_IMAGE;
extern int OpenTIM(void *);
extern TIM_IMAGE *ReadTIM(TIM_IMAGE *);
extern int LoadImage(RECT *, void *);
void FUN_800174fc(void *p, unsigned short a, short b, unsigned short c, short d)
{
    TIM_IMAGE im;
    TIM_IMAGE *t = &im;
    if (OpenTIM(p) == 0 && ReadTIM(t) != 0) {
        im.prect->x = a;
        im.prect->y = b;
        im.crect->x = c;
        im.crect->y = d;
        if ((im.mode & 8) && (short)c >= 0)
            LoadImage(im.crect, im.caddr);
        if ((short)a >= 0)
            LoadImage(t->prect, t->paddr);
    }
}
