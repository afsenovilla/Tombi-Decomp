// FUNC 80018d2c 60 MAIN0
// MATCHING 80018d2c 60
extern int *g230;
extern unsigned short g242;
void ObjListPush_1F800230(int o)
{
    g230 -= 1;
    g230[0] = o;
    g242 = g242 + 1;
}
