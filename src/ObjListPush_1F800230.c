// FUNC 80018d2c 60 MAIN0
// MATCHING 80018d2c 60
extern int *g230;
extern unsigned short CAT8_OBJECT_COUNT;
void ObjListPush_1F800230(int o)
{
    g230 -= 1;
    g230[0] = o;
    CAT8_OBJECT_COUNT = CAT8_OBJECT_COUNT + 1;
}
