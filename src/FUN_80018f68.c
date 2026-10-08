// FUNC 80018f68 284 MAIN0
// MATCHING 80018f68 284
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
    char *p;

    if (DAT_8009d69c == 0)
        return;
    p = DAT_8009f840;
    FUN_80038024(p);
    switch (DAT_8009c960) {
    case 0:
        switch (DAT_8009c962) {
        case 0: case 1: case 2:
            a = DAT_1f8002b8;
            goto L;
        }
        return;
    case 1:
        switch (DAT_8009c962) {
        case 0: case 1: case 2: case 3: case 4:
            a = DAT_1f8002b8;
            goto L;
        }
        return;
    case 2:
        switch (DAT_8009c962) {
        case 0:
            a = DAT_1f8002bc;
            goto L;
        case 1: case 2:
            a = DAT_1f8002b8;
            goto L;
        }
        return;
    default:
        return;
    }
L:
    FUN_80037e74(a, p);
    FUN_80037fbc(p, 0);
}
