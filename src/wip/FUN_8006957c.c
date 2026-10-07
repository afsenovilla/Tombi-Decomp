// FUNC 8006957c 44 MAIN0
extern int D[];
extern char A[], B[];

void FUN_8006957c(void)
{
    int *p = &D[1];
    p[0] = (int)A;
    p[1] = (int)B;
    p[-1] = 0;
    p[2] = 0;
}
