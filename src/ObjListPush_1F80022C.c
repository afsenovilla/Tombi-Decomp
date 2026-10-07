// FUNC 80018da4 60 MAIN0
extern struct {
    char p0[0x22c];
    int *list;
    char p1[0x240 - 0x230];
    unsigned short cnt;
} SCR;

void ObjListPush_1F80022C(int a)
{
    int *p = SCR.list;
    SCR.list = p - 1;
    p[-1] = a;
    SCR.cnt++;
}
