// FUNC 80030f38 148 MAIN0
// MATCHING 80030f38 148
extern void (*tab[])(void *);
extern char objs[];
extern int idx;
void FUN_80030f38(void)
{
    char *p = objs;
    idx = 0;
    do {
        if (*p) tab[(unsigned char)p[2]](p);
        idx++;
        p += 0x6c;
    } while (idx < 10);
}
