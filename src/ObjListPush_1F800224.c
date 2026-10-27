// FUNC 80018cf0 60 MAIN0
// MATCHING 80018cf0 60
extern int *CAT4_OBJECT_LIST;
extern unsigned short CAT4_OBJECT_COUNT;

void ObjListPush_1F800224(int v)
{
    int *p = CAT4_OBJECT_LIST - 1;
    CAT4_OBJECT_LIST = p;
    *p = v;
    CAT4_OBJECT_COUNT = CAT4_OBJECT_COUNT + 1;
}
