// FUNC 80018da4 60 MAIN0
// MATCHING 80018da4 60
extern int *SCR_22C;
extern unsigned short SCR_240;

void ObjListPush_1F80022C(int a)
{
    *--SCR_22C = a;
    SCR_240++;
}
