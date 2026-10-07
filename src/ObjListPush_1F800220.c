// FUNC 80018ca4 76 MAIN0
extern struct {
    char p0[0x220];
    int *list;
    char p1[0x24a - 0x224];
    short cnt;
} SCR;

void ObjListPush_1F800220(int a)
{
    int n = SCR.cnt;
    if (n < 0x56) {
        int *p = SCR.list;
        SCR.list = p - 1;
        p[-1] = a;
        SCR.cnt = n + 1;
    }
}
