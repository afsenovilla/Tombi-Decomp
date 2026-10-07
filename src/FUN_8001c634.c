// FUNC 8001c634 84 MAIN0
typedef struct Th { char pad[0x4a]; short a; short b; } Th;
extern Th *DAT_1f8001d4;
extern char DAT_1f8001cf;
extern char DAT_8009e375, DAT_8009e376, DAT_8009e377;
extern char DAT_8009f085, DAT_8009f086, DAT_8009f087;

void FUN_8001c634(void)
{
    int t = (int)DAT_1f8001d4;
    DAT_1f8001cf = 1;
    DAT_8009e375 = 0;
    DAT_8009e376 = 0;
    DAT_8009e377 = 0;
    DAT_8009f085 = 0;
    DAT_8009f086 = 0;
    DAT_8009f087 = 0;
    *(short *)(t + 0x4a) = 3;
    *(short *)(t + 0x4c) = 0;
}
