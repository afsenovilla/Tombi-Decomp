// FUNC 80018ca4 76 MAIN0
extern int *SCR_220;
extern short SCR_24A;

void ObjListPush_1F800220(int a)
{
    int n = SCR_24A;
    if (n < 0x56) {
        int *p = SCR_220;
        SCR_220 = p - 1;
        p[-1] = a;
        SCR_24A = n + 1;
    }
}
