// FUNC 80018ca4 76 MAIN0
// MATCHING 80018ca4 76
extern int *SCR_220;
extern short SCR_24A;

void ObjListPush_1F800220(int a)
{
    if (SCR_24A < 0x56) {
        *--SCR_220 = a;
        SCR_24A++;
    }
}
