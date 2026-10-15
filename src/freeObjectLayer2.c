// FUNC 80018790 84 MAIN0
// MATCHING 80018790 84
// VERSION SLPM_80165 slpm80165 80017418 76
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80018790 (freeObjectLayer2.c): does not clear +0x9e/+0x9f.
extern int *g208;
extern unsigned short g238;
void freeObjectLayer2(volatile int *o)
{
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((volatile unsigned char *)o)[0x1c] = 0;
    ((volatile unsigned char *)o)[0x9c] = 0;
    ((volatile unsigned char *)o)[0x9d] = 0;
    g238++;
    g208 -= 1;
    g208[0] = (int)o;
}
#else
extern int *g208;
extern unsigned short g238;
void freeObjectLayer2(volatile int *o)
{
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((volatile unsigned char *)o)[0x1c] = 0;
    ((volatile unsigned char *)o)[0x9c] = 0;
    ((volatile unsigned char *)o)[0x9d] = 0;
    ((volatile unsigned char *)o)[0x9e] = 0;
    ((volatile unsigned char *)o)[0x9f] = 0;
    g238++;
    g208 -= 1;
    g208[0] = (int)o;
}
#endif
