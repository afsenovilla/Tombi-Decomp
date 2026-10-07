// FUNC 80018d68 60 MAIN0
// MATCHING 80018d68 60
// FLAGS -O1 -G0
extern int *DAT_1f800228;
extern unsigned short DAT_1f800248;

void ObjListPush_1F800228(int v)
{
    int *p = DAT_1f800228;
    DAT_1f800228 = p - 1;
    p[-1] = v;
    DAT_1f800248 = DAT_1f800248 + 1;
}
