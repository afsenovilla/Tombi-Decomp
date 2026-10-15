// FUNC 800288c4 444 MAIN0
// MATCHING 800288c4 444
// VERSION SLPS_01144 slps01144 800296D4 444
// VERSION SLPM_80165 slpm80165 800287BC 444
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 800288C4 (cameraScrollInputStep.c): 64 -> 16384, 16 -> 4096.
extern unsigned char DAT_800a60d4;
extern short DAT_1f8000e6;
extern unsigned short DAT_8009d670;
extern void cameraScrollToPlayerYB(char *, int);
extern void cameraScrollToPlayerYA(char *, int);

static __inline__ void decay(void)
{
    short v = DAT_1f8000e6;
    if (v == 0) return;
    if (v > 0) {
        v -= 2;
        DAT_1f8000e6 = v;
        if (v < 0) DAT_1f8000e6 = 0;
    } else {
        v += 2;
        DAT_1f8000e6 = v;
        if (v > 0) DAT_1f8000e6 = 0;
    }
}

void cameraScrollInputStep(char *o, int a)
{
    volatile unsigned short *k;
    
    if (DAT_800a60d4 != 0) {
        decay();
        return;
    }
    k = &DAT_8009d670;
    if (*k & 0x1000) {
        if (a != 0 || *(int *)(o + 0x20) < -0x9ff) {
            cameraScrollToPlayerYB(o, a);
            return;
        }
        decay();
    } else if (*k & 0x4000) {
        if (a != 0 || *(int *)(o + 0x20) >= 0xa00) {
            cameraScrollToPlayerYA(o, a);
            return;
        }
        decay();
    } else {
        decay();
    }
}
#elif defined(VERSION_SLPM_80165)
// Demo version of PAL main0 800288C4 (cameraScrollInputStep.c): pad bits 0x1000/0x4000 (PAL 0x10/0x40).
extern unsigned char DAT_800a60d4;
extern short DAT_1f8000e6;
extern unsigned short DAT_8009d670;
extern void cameraScrollToPlayerYB(char *, int);
extern void cameraScrollToPlayerYA(char *, int);

static __inline__ void decay(void)
{
    short v = DAT_1f8000e6;
    if (v == 0) return;
    if (v > 0) {
        v -= 2;
        DAT_1f8000e6 = v;
        if (v < 0) DAT_1f8000e6 = 0;
    } else {
        v += 2;
        DAT_1f8000e6 = v;
        if (v > 0) DAT_1f8000e6 = 0;
    }
}

void cameraScrollInputStep(char *o, int a)
{
    volatile unsigned short *k;
    
    if (DAT_800a60d4 != 0) {
        decay();
        return;
    }
    k = &DAT_8009d670;
    if (*k & 0x1000) {
        if (a != 0 || *(int *)(o + 0x20) < -0x9ff) {
            cameraScrollToPlayerYB(o, a);
            return;
        }
        decay();
    } else if (*k & 0x4000) {
        if (a != 0 || *(int *)(o + 0x20) >= 0xa00) {
            cameraScrollToPlayerYA(o, a);
            return;
        }
        decay();
    } else {
        decay();
    }
}
#else
extern unsigned char DAT_800a60d4;
extern short DAT_1f8000e6;
extern unsigned short DAT_8009d670;
extern void cameraScrollToPlayerYB(char *, int);
extern void cameraScrollToPlayerYA(char *, int);

static __inline__ void decay(void)
{
    short v = DAT_1f8000e6;
    if (v == 0) return;
    if (v > 0) {
        v -= 2;
        DAT_1f8000e6 = v;
        if (v < 0) DAT_1f8000e6 = 0;
    } else {
        v += 2;
        DAT_1f8000e6 = v;
        if (v > 0) DAT_1f8000e6 = 0;
    }
}

void cameraScrollInputStep(char *o, int a)
{
    volatile unsigned short *k;
    
    if (DAT_800a60d4 != 0) {
        decay();
        return;
    }
    k = &DAT_8009d670;
    if (*k & 0x10) {
        if (a != 0 || *(int *)(o + 0x20) < -0x9ff) {
            cameraScrollToPlayerYB(o, a);
            return;
        }
        decay();
    } else if (*k & 0x40) {
        if (a != 0 || *(int *)(o + 0x20) >= 0xa00) {
            cameraScrollToPlayerYA(o, a);
            return;
        }
        decay();
    } else {
        decay();
    }
}
#endif
