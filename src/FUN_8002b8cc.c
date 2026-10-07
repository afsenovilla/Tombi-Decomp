// FUNC 8002b8cc 84 MAIN0
extern int DAT_8009c954;
extern char *ObjAlloc(void);

void FUN_8002b8cc(char v)
{
    char *p;
    if (DAT_8009c954 == 0) {
        p = ObjAlloc();
        if (p != 0) {
            p[0] = 1;
            p[2] = 0xd;
            p[0xc] = v;
        }
    }
}
