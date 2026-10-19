// FUNC 80120404 52 X003
// MATCHING 80120404 52
extern short pushObjectOutOfObjectBoxB();

void pushCat2ObjectOutOfCat4Type0A(void)
{
    if (pushObjectOutOfObjectBoxB() != 0)
        *(short *)0x1f80019e = 0;
}
