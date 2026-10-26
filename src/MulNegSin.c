// FUNC 8001fe0c 48 MAIN0
// MATCHING 8001fe0c 48
extern short NEG_SIN_TABLE[];

int MulNegSin(int a, short b)
{
    return (NEG_SIN_TABLE[(short)a] * b * 16) >> 16;
}
