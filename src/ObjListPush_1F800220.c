// FUNC 80018ca4 76 MAIN0
// MATCHING 80018ca4 76
extern int *CAT3_OBJECT_LIST;
extern short CAT3_OBJECT_COUNT;

void ObjListPush_1F800220(int a)
{
    if (CAT3_OBJECT_COUNT < 0x56) {
        *--CAT3_OBJECT_LIST = a;
        CAT3_OBJECT_COUNT++;
    }
}
