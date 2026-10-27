// FUNC 80018d68 60 MAIN0
// MATCHING 80018d68 60
// FLAGS -O1 -G0
extern int *CAT5_OBJECT_LIST;
extern unsigned short CAT5_OBJECT_COUNT;

void ObjListPush_1F800228(int v)
{
    int *p = CAT5_OBJECT_LIST;
    CAT5_OBJECT_LIST = p - 1;
    p[-1] = v;
    CAT5_OBJECT_COUNT = CAT5_OBJECT_COUNT + 1;
}
