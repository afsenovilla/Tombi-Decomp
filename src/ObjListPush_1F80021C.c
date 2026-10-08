// FUNC 80018c68 60 MAIN0
// MATCHING 80018c68 60
extern int **DAT_1f80021c;
extern unsigned short DAT_1f800246;

void ObjListPush_1F80021C(int *p)
{
    *--DAT_1f80021c = p;
    DAT_1f800246++;
}
