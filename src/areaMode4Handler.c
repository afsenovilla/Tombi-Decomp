// FUNC 8001b49c 484 MAIN0
// MATCHING 8001b49c 484
// VERSION SLPM_80165 slpm80165 8001B080 480
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8001B49C (areaMode4Handler.c): func_80018254 calls, no pad reset, exits to mode 7 (PAL 8).
typedef struct G { char pad[0x4c]; unsigned short w4c, w4e; } G;
extern G *TASK_C;
extern int DAT_8009c968[];
extern unsigned char DAT_1f8001c2;
extern volatile unsigned short DAT_8009d670[];
extern volatile unsigned short DAT_8009d674;
extern unsigned short DAT_1f8001fc;
extern unsigned char DAT_800b1410;
extern unsigned char DAT_1f8001cf;
extern unsigned char DAT_8009c967;
extern unsigned char DAT_8009e375, DAT_8009e376, DAT_8009e377;
extern unsigned char DAT_8009f085, DAT_8009f086, DAT_8009f087;
extern void FUN_800175f0(void);
extern void FUN_80017b44(void);
extern void FUN_80021858(void);
extern void initSectionCamera(void);
extern void spawnSectionUnlayeredObjects(void);
extern void initHudState(void);
extern void startAreaBgm(void);
extern void areaMode4Tick(void);
extern unsigned char *allocObjectLayer7(void);

extern char D_800A2A18[];
extern void func_80018254(char *);
void areaMode4Handler(void)
{
    unsigned char *q;
    char pad[8];
    switch (TASK_C->w4e) {
    case 0:
        FUN_800175f0();
        FUN_80017b44();
        FUN_80021858();
        initSectionCamera();
        spawnSectionUnlayeredObjects();
        initHudState();
        DAT_800b1410 = 0;
        func_80018254(D_800A2A18);
        DAT_1f8001cf = 1;
        if (DAT_8009c967 != 1)
            startAreaBgm();
        TASK_C->w4e++;
        q = allocObjectLayer7();
        if (q != 0) {
            q[0] = 1;
            q[2] = 5;
            q[3] = 0;
            *(short *)(q + 0x12) = 0;
            *(short *)(q + 0x16) = 0;
            *(short *)(q + 0x1a) = 0;
        }
        break;
    case 1:
        DAT_8009c968[0]++;
        areaMode4Tick();
        if (DAT_1f8001c2 != 0 && (DAT_8009d670[0] & 0x800) && (DAT_8009d670[0] & 8))
            TASK_C->w4e = 3;
        break;
    case 2:
    case 3:
        func_80018254(D_800A2A18);
        DAT_8009e375 = 0;
        DAT_8009e376 = 0;
        DAT_8009e377 = 0;
        DAT_8009f085 = 0;
        DAT_8009f086 = 0;
        DAT_8009f087 = 0;
        TASK_C->w4c = 7;
        TASK_C->w4e = 0;
        break;
    }
}
#else
typedef struct G { char pad[0x4c]; unsigned short w4c, w4e; } G;
extern G *TASK_C;
extern int DAT_8009c968[];
extern unsigned char DAT_1f8001c2;
extern volatile unsigned short DAT_8009d670[];
extern volatile unsigned short DAT_8009d674;
extern unsigned short DAT_1f8001fc;
extern unsigned char DAT_800b1410;
extern unsigned char DAT_1f8001cf;
extern unsigned char DAT_8009c967;
extern unsigned char DAT_8009e375, DAT_8009e376, DAT_8009e377;
extern unsigned char DAT_8009f085, DAT_8009f086, DAT_8009f087;
extern void FUN_800175f0(void);
extern void FUN_80017b44(void);
extern void FUN_80021858(void);
extern void initSectionCamera(void);
extern void spawnSectionUnlayeredObjects(void);
extern void initHudState(void);
extern void startAreaBgm(void);
extern void areaMode4Tick(void);
extern unsigned char *allocObjectLayer7(void);

void areaMode4Handler(void)
{
    unsigned char *q;
    char pad[8];
    switch (TASK_C->w4e) {
    case 0:
        FUN_800175f0();
        FUN_80017b44();
        FUN_80021858();
        initSectionCamera();
        spawnSectionUnlayeredObjects();
        initHudState();
        DAT_800b1410 = 0;
        DAT_1f8001cf = 1;
        if (DAT_8009c967 != 1)
            startAreaBgm();
        TASK_C->w4e++;
        q = allocObjectLayer7();
        if (q != 0) {
            q[0] = 1;
            q[2] = 5;
            q[3] = 0;
            *(short *)(q + 0x12) = 0;
            *(short *)(q + 0x16) = 0;
            *(short *)(q + 0x1a) = 0;
        }
        DAT_8009d674 = 0;
        DAT_1f8001fc = 0;
        DAT_8009d670[0] = DAT_8009d674;
        break;
    case 1:
        DAT_8009c968[0]++;
        areaMode4Tick();
        if (DAT_1f8001c2 != 0 && (DAT_8009d670[0] & 8) && (DAT_8009d670[0] & 0x800))
            TASK_C->w4e = 3;
        break;
    case 2:
    case 3:
        DAT_8009e375 = 0;
        DAT_8009e376 = 0;
        DAT_8009e377 = 0;
        DAT_8009f085 = 0;
        DAT_8009f086 = 0;
        DAT_8009f087 = 0;
        TASK_C->w4c = 8;
        TASK_C->w4e = 0;
        break;
    }
}
#endif
