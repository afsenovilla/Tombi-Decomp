// FUNC 8001fe3c 48 MAIN0
// MATCHING 8001fe3c 48
extern short costab[];

int MulCosDup(int a, short b)
{
    return (costab[(short)a] * b * 16) >> 16;
}
