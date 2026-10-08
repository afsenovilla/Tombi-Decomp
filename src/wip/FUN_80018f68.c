// FUNC 80018f68 284 MAIN0
extern int DAT_8009d69c;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern int DAT_1f8002b8;
extern int DAT_1f8002bc;
extern char DAT_8009f840[];
extern void FUN_80038024(char *);
extern void FUN_80037e74(int, char *);
extern void FUN_80037fbc(char *, int);

void FUN_80018f68(void)
{
    int a;

    if (DAT_8009d69c == 0)
        return;
    FUN_80038024(DAT_8009f840);
    switch (DAT_8009c960) {
    case 0:
        if (!(DAT_8009c962 < 3 && DAT_8009c962 >= 0))
            return;
        a = DAT_1f8002b8;
        break;
    case 1:
        if (!(DAT_8009c962 < 5 && DAT_8009c962 >= 0))
            return;
        a = DAT_1f8002b8;
        break;
    case 2:
        if (DAT_8009c962 == 0) {
            a = DAT_1f8002bc;
        } else {
            if (!(DAT_8009c962 < 3 && DAT_8009c962 >= 0))
                return;
            a = DAT_1f8002b8;
        }
        break;
    default:
        return;
    }
    FUN_80037e74(a, DAT_8009f840);
    FUN_80037fbc(DAT_8009f840, 0);
}
