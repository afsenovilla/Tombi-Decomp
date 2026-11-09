// FUNC 80021f5c 80 MAIN0
// MATCHING 80021f5c 80
typedef struct S { int w[8]; } S;
extern S BASE_MATRIX;

void getBaseMatrix(S *o)
{
    *o = BASE_MATRIX;
}
