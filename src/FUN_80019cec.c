// FUNC 80019cec 156 MAIN0
// MATCHING 80019cec 156
extern char *G;
extern void a(), b(), c(), d();
void FUN_80019cec(void)
{
    switch (*(unsigned short *)(G + 0x4a)) {
    case 0: a(); break;
    case 1: b(); break;
    case 2: c(); break;
    case 3: d(); break;
    }
}
