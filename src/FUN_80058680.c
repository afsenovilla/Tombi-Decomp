// FUNC 80058680 296 MAIN0
// MATCHING 80058680 296
typedef struct { short x, y; } Pt;
extern void FUN_8005f990(char *);
extern void FUN_80059464(int, int);
extern void FUN_800587a8(char *, int, int);
extern void FUN_8005892c(char *, int, int);
extern void FUN_80058b18(char *, int, int, int);
extern void FUN_80058f98(char *, int, int);
extern void FUN_80058d28(char *, int, int, int, int);
extern Pt DAT_800802c8[];
extern short DAT_800802d4[];
extern unsigned char DAT_8009c990;
extern char DAT_800b1468[];

void FUN_80058680(char *o)
{
    char env[0x60];
    int i;
    FUN_8005f990(env);
    FUN_80059464(*(short *)(env + 0x14), 1);
    FUN_800587a8(o, 0x18, 0xf0);
    FUN_8005892c(o, 0x12c, 0x10);
    FUN_80058b18(o, 0x24, 0x1c, *(short *)(o + 0x52));
    FUN_80058f98(o, 0x48, 0x10);
    for (i = 0; i < 3; i++) {
        if (DAT_8009c990 == DAT_800802d4[i]) {
            FUN_80058d28(o, i, DAT_800802c8[i].x, DAT_800802c8[i].y, 0);
        } else if (DAT_800b1468[i] != 0) {
            FUN_80058d28(o, i, DAT_800802c8[i].x, DAT_800802c8[i].y, 1);
        }
    }
}
