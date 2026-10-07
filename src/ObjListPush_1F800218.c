// FUNC 80018c2c 60 MAIN0
// MATCHING 80018c2c 60
extern int **DAT_1f800218;
extern unsigned short DAT_1f800244;

void ObjListPush_1F800218(int *p)
{
    *--DAT_1f800218 = p;
    DAT_1f800244++;
}
