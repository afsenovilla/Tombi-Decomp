// FUNC 8001fddc 48 MAIN0
// MATCHING 8001fddc 48
extern short COS_TABLE[];

int MulCos(int a, short b)
{
    return (COS_TABLE[(short)a] * b * 16) >> 16;
}
