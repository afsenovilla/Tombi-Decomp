// FUNC 80018da4 60 MAIN0
extern int *SCR_22C;
extern unsigned short SCR_240;

void ObjListPush_1F80022C(int a)
{
    int *p = SCR_22C;
    SCR_22C = p - 1;
    p[-1] = a;
    SCR_240++;
}
