// FUNC 8001fdac 48 MAIN0
// MATCHING 8001fdac 48
extern short negsintab[];

int MulNegSinScaled(int a, short b)
{
    return (negsintab[(short)a] * b * 16) >> 16;
}
