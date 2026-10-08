// FUNC 80026000 432 MAIN0
// MATCHING 80026000 432
typedef struct { char c[6]; } B;
extern B DAT_80010238;
extern char DAT_8009d610[];
extern unsigned short DAT_8009d612;
extern unsigned short DAT_8009d614;
extern unsigned char DAT_8009d614b;
extern unsigned char DAT_8009d615;
extern unsigned char DAT_8009d616;
extern unsigned char DAT_8009d617;
extern void FUN_80069410();
extern int FUN_80069050(int);
extern void FUN_80069390(int, void *);

void FUN_80026000(void)
{
    B buf;
    char *p;
    buf = DAT_80010238;
    p = DAT_8009d610;
    if (DAT_8009d612 == 0)
        return;
    switch (DAT_8009d616) {
    case 0:
        if (DAT_8009d614 != 0) {
            DAT_8009d614b = 0;
            DAT_8009d615 = 0;
            FUN_80069410(0, p + 4, 2);
        }
        if (FUN_80069050(0) == 6)
            FUN_80069390(0, &buf);
        break;
    case 1:
        if (FUN_80069050(0) == 6) {
            FUN_80069390(0, &buf);
            DAT_8009d616 = 2;
        }
        break;
    case 2:
        if (DAT_8009d617 != 0) {
            DAT_8009d617--;
        } else {
            DAT_8009d614b = 0;
            DAT_8009d615 = 0;
            FUN_80069410(0, p + 4, 2);
            DAT_8009d617 = 0;
            DAT_8009d616 = 3;
        }
        break;
    case 3:
        if (FUN_80069050(0) == 6) {
            FUN_80069390(0, &buf);
            DAT_8009d616 = 0;
        }
        break;
    }
}
