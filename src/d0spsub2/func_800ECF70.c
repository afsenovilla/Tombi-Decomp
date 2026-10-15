// FUNC 800ecf70 88 D0SPSUB2
// MATCHING 800ecf70 88
extern int D_8009F10C, D_8009F110, D_8009F114, D_8009F118;
extern int TestEvent(int ev);

void func_800ECF70(void)
{
    TestEvent(D_8009F10C);
    TestEvent(D_8009F110);
    TestEvent(D_8009F114);
    TestEvent(D_8009F118);
}
