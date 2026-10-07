// FUNC 8001703c 76 MAIN0
// MATCHING 8001703c 76
void FUN_8001703c(void)
{
int r = *(int *)0x110; char *p = (char *)0x801fd800; int i = 0; unsigned k = 0x40000404; char *q = (char *)0x801fe400;
for (; i < 3; i++) {
    r += 0xc0;
    *(short *)p = 0;
    *(char **)(p + 8) = q;
    p += 0x70;
    q += 0x800;
    *(int *)(r + 0x94) = k;
}
}
