// FUNC 80028250 464 MAIN0
// MATCHING 80028250 464
// VERSION SLPM_80165 slpm80165 8002826C 440
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80028250 (FUN_80028250.c): does not set flag 8 at +0x44.
extern int VIEW_CAMERA_Y_FIX;
extern short VIEW_CAMERA_Y;
extern int CAMERA_TARGET_Y_FIX;
extern void stepScrollApproachX(unsigned char *);
#define W(o) (*(int *)((o) + 0x18))

void FUN_80028250(unsigned char *o)
{
    int *p, t;
    switch (*(short *)(o + 0x5c)) {
    case 0:
        o[0x6d] = 0;
        *(signed char *)(o + 0x6f) = -10;
        (*(short *)(o + 0x5c))++;
    case 1:
        W(o) += 0x2000;
        if (W(o) > 0x3ffff)
            W(o) = 0x40000;
        if (W(o) > 0x12000)
            stepScrollApproachX(o);
        p = &VIEW_CAMERA_Y_FIX;
        *p += W(o);
        if (VIEW_CAMERA_Y > *(short *)(o + 0x32)) {
            VIEW_CAMERA_Y_FIX = *(short *)(o + 0x32) << 16;
            W(o) = 0;
        }
        t = *p - CAMERA_TARGET_Y_FIX;
        if (t > 0x580000)
            (*(short *)(o + 0x5c))++;
        break;
    case 2:
        W(o) += 0x2000;
        if (W(o) > 0x2ffff)
            W(o) = 0x30000;
        p = &VIEW_CAMERA_Y_FIX;
        *p += W(o);
        if (VIEW_CAMERA_Y > *(short *)(o + 0x32)) {
            VIEW_CAMERA_Y_FIX = *(short *)(o + 0x32) << 16;
            W(o) = 0;
        }
        t = *p - CAMERA_TARGET_Y_FIX;
        if (t <= 0)
            o[3] = 3;
        break;
    }
}
#else
extern int VIEW_CAMERA_Y_FIX;
extern short VIEW_CAMERA_Y;
extern int CAMERA_TARGET_Y_FIX;
extern void stepScrollApproachX(unsigned char *);
#define W(o) (*(int *)((o) + 0x18))

void lookScrollStepX(unsigned char *o)
{
    int *p, t;
    switch (*(short *)(o + 0x5c)) {
    case 0:
        o[0x6d] = 0;
        *(signed char *)(o + 0x6f) = -10;
        (*(short *)(o + 0x5c))++;
    case 1:
        W(o) += 0x2000;
        if (W(o) > 0x3ffff)
            W(o) = 0x40000;
        if (W(o) > 0x12000)
            stepScrollApproachX(o);
        p = &VIEW_CAMERA_Y_FIX;
        *p += W(o);
        if (VIEW_CAMERA_Y > *(short *)(o + 0x32)) {
            VIEW_CAMERA_Y_FIX = *(short *)(o + 0x32) << 16;
            W(o) = 0;
            *(unsigned short *)(o + 0x44) |= 8;
        }
        t = *p - CAMERA_TARGET_Y_FIX;
        if (t > 0x580000)
            (*(short *)(o + 0x5c))++;
        break;
    case 2:
        W(o) += 0x2000;
        if (W(o) > 0x2ffff)
            W(o) = 0x30000;
        p = &VIEW_CAMERA_Y_FIX;
        *p += W(o);
        if (VIEW_CAMERA_Y > *(short *)(o + 0x32)) {
            VIEW_CAMERA_Y_FIX = *(short *)(o + 0x32) << 16;
            W(o) = 0;
            *(unsigned short *)(o + 0x44) |= 8;
        }
        t = *p - CAMERA_TARGET_Y_FIX;
        if (t <= 0)
            o[3] = 3;
        break;
    }
}
#endif
