// FUNC 8001fe3c 48 MAIN0
// MATCHING 8001fe3c 48
extern short COS_TABLE[];

int MulCosDup(int a, short b)
{
    return (COS_TABLE[(short)a] * b * 16) >> 16;
}
