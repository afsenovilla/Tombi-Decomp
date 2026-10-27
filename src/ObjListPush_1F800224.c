// FUNC 80018cf0 60 MAIN0
// MATCHING 80018cf0 60
extern int *sp224;
extern unsigned short CAT4_OBJECT_COUNT;

void ObjListPush_1F800224(int v)
{
    int *p = sp224 - 1;
    sp224 = p;
    *p = v;
    CAT4_OBJECT_COUNT = CAT4_OBJECT_COUNT + 1;
}
