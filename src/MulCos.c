// FUNC 8001fddc 48 MAIN0
// MATCHING 8001fddc 48
extern short costab[];

int MulCos(int a, short b)
{
    return (costab[(short)a] * b * 16) >> 16;
}
