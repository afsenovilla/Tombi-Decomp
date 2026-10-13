// FUNC 80019cec 156 MAIN0
// MATCHING 80019cec 156
extern char *TASK_C;
extern void a(), b(), c(), d();
void gameStateDispatcher(void)
{
    switch (*(unsigned short *)(TASK_C + 0x4a)) {
    case 0: a(); break;
    case 1: b(); break;
    case 2: c(); break;
    case 3: d(); break;
    }
}
