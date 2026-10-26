// FUNC 8001c314 80 MAIN0
// MATCHING 8001c314 80
// VERSION SLPM_80165 slpm80165 8001B910 64
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8001C314 (setAreaModeFromSection.c): one mode table per area (no D_8009F838 offset).
extern unsigned short GAME, AREA_VARIANT, DAT_8009c962;
extern unsigned char *PTR_DAT_80077b64[];
extern char *TASK_C;

void setAreaModeFromSection(void)
{
    unsigned char *row = PTR_DAT_80077b64[GAME];
    unsigned char b = row[DAT_8009c962];
    char *p = TASK_C;
    *(short *)(p + 0x4e) = 0;
    *(unsigned short *)(p + 0x4c) = b;
}
#else
extern unsigned short GAME, AREA_VARIANT, DAT_8009c962;
extern unsigned char *PTR_DAT_80077b64[];
extern char *TASK_C;

void setAreaModeFromSection(void)
{
    unsigned char *row = PTR_DAT_80077b64[GAME + AREA_VARIANT];
    unsigned char b = row[DAT_8009c962];
    char *p = TASK_C;
    *(short *)(p + 0x4e) = 0;
    *(unsigned short *)(p + 0x4c) = b;
}
#endif
