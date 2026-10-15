// FUNC 801166e0 24 X013
// MATCHING 801166e0 24
typedef struct { unsigned char b0, b1, b2, b3; int d4; } S;

void linkAreaModelBlock(S *a, S *b)
{
    a->d4 = (int)b + b->d4;
    a->b3 = 1;
}
