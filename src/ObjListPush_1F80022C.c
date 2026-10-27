// FUNC 80018da4 60 MAIN0
// MATCHING 80018da4 60
extern int *SCR_22C;
extern unsigned short CAT7_OBJECT_COUNT;

void ObjListPush_1F80022C(int a)
{
    *--SCR_22C = a;
    CAT7_OBJECT_COUNT++;
}
