// FUNC 80018c68 60 MAIN0
// MATCHING 80018c68 60
extern int **CAT2_OBJECT_LIST;
extern unsigned short CAT2_OBJECT_COUNT;

void ObjListPush_1F80021C(int *p)
{
    *--CAT2_OBJECT_LIST = p;
    CAT2_OBJECT_COUNT++;
}
