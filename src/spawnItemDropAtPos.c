// FUNC 8003e3cc 84 MAIN0
// MATCHING 8003e3cc 84
extern void ObjSpawn(int a, short b, short c, int d, short e, short f);

void spawnItemDropAtPos(short a, short b, int c, short d, short e)
{
    ObjSpawn(3, a, b, c, d, e);
}
