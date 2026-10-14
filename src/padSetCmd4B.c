// FUNC 8006ae3c 20 MAIN0
// MATCHING 8006ae3c 20
void padSetCmd4B(char *p)
{
    p[0x37] = 0x4b;
    *(int *)(p + 0x2c) = 0;
    p[0x36] = 0;
}
