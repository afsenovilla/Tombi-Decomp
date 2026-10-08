// FUNC 80016ca8 396 MAIN0
// MATCHING 80016ca8 396
extern void FUN_8005e244(void *, int, int, int, int);
extern void FUN_8005e314(void *, int, int, int, int);
extern void FUN_8005f9c8(void *);
extern void FUN_8005f770(void *);
extern char DAT_8009e35c[], DAT_8009e348[], DAT_8009f06c[], DAT_8009f058[];
extern short DAT_8009e354, DAT_8009e356, DAT_8009f064, DAT_8009f066;
extern char DAT_8009f084, DAT_8009e374, DAT_8009f082, DAT_8009e372, DAT_8009f083, DAT_8009e373;
extern unsigned short DAT_8009e350, DAT_8009e352, DAT_8009f060, DAT_8009f062, DAT_8009d4fc, DAT_8009d4fe;
extern char DAT_8009e375, DAT_8009e376, DAT_8009e377, DAT_8009f085, DAT_8009f086, DAT_8009f087;

void FUN_80016ca8(char a, char b, char c)
{
    FUN_8005e244(DAT_8009e35c, 0x180, 0x100, 0x140, 0x100);
    FUN_8005e314(DAT_8009e35c - 0x14, 0x2c0, 0x100, 0x140, 0x100);
    FUN_8005e244(DAT_8009e35c + 0xd10, 0x2c0, 0x100, 0x140, 0x100);
    FUN_8005e314(DAT_8009e35c + 0xcfc, 0x180, 0x100, 0x140, 0x100);
    DAT_8009e354 = 0x100;
    DAT_8009e356 = 0x100;
    DAT_8009f064 = 0x100;
    DAT_8009f066 = 0x100;
    DAT_8009f084 = 1;
    DAT_8009e374 = 1;
    DAT_8009f082 = 1;
    DAT_8009e372 = 1;
    DAT_8009f083 = 0;
    DAT_8009e373 = 0;
    DAT_8009e350 = DAT_8009d4fc;
    DAT_8009e352 = DAT_8009d4fe;
    DAT_8009f060 = DAT_8009d4fc;
    DAT_8009f062 = DAT_8009d4fe;
    DAT_8009e375 = a;
    DAT_8009e376 = b;
    DAT_8009e377 = c;
    DAT_8009f085 = a;
    DAT_8009f086 = b;
    DAT_8009f087 = c;
    FUN_8005f9c8(DAT_8009e35c - 0x14);
    FUN_8005f770(DAT_8009e35c);
}
