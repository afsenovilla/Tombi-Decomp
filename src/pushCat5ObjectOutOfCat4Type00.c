// FUNC 80126d04 68 X000
// MATCHING 80126d04 68
extern short pushObjectOutOfObjectBoxB();

void pushCat5ObjectOutOfCat4Type00(int a, char *p)
{
    if (*(int *)(p + 0x94) == 0 && pushObjectOutOfObjectBoxB() != 0)
        *(short *)0x1f80019e = 0;
}
