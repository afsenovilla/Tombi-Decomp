// FUNC 80018f68 284 MAIN0
// MATCHING 80018f68 284
extern int DAT_8009d69c;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern int DAT_1f8002b8;
extern int DAT_1f8002bc;
extern char DAT_8009f840[];
extern void scriptReset(char *);
extern void openScriptFile(int, char *);
extern void scriptStart(char *, int);

void initAreaScripts(void)
{
    int a;
    char *p;

    if (DAT_8009d69c == 0)
        return;
    p = DAT_8009f840;
    scriptReset(p);
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
    openScriptFile(a, p);
    scriptStart(p, 0);
}
