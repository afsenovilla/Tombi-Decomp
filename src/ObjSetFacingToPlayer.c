// FUNC 8001f8e4 60 MAIN0
// MATCHING 8001f8e4 60
void ObjSetFacingToPlayer(char *o)
{
    char *q = o;
    if (*(short *)(*(int *)(o + 0x40) + 2) > *(short *)0x1f80016a) *(short *)(o + 0x2e) = 1; else *(short *)(q + 0x2e) = 0;
}
