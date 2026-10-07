// FUNC 8001fe0c 48 MAIN0
// MATCHING 8001fe0c 48
extern short negsintab[];

int MulNegSin(int a, short b)
{
    return (negsintab[(short)a] * b * 16) >> 16;
}
