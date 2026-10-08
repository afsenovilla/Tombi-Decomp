// FUNC 80025f40 192 MAIN0
// MATCHING 80025f40 192
typedef struct { char c[6]; } B;
extern B DAT_80010238;
extern char DAT_8009d610[];
extern unsigned short DAT_8009d612;
extern unsigned short DAT_8009d614;
extern unsigned char DAT_8009d614b;
extern unsigned char DAT_8009d615;
extern unsigned char DAT_8009d616;
extern unsigned char DAT_8009d617;
extern void FUN_80069410(unsigned char, void *, int, int);

void FUN_80025f40(int a, int b, int c, int d)
{
    B buf;
    char *p;
    buf = DAT_80010238;
    p = DAT_8009d610;
    if (DAT_8009d612 != 0 && DAT_8009d616 == 0 && DAT_8009d614 == 0) {
        DAT_8009d615 = c;
        DAT_8009d614b = b;
        FUN_80069410(a, p + 4, 2, d);
        DAT_8009d617 = d;
        DAT_8009d616 = 1;
    }
}
