// FUNC 80018744 76 MAIN0
extern int *g204;
extern unsigned short g236;
void FUN_80018744(int *o)
{
    unsigned char c = ((unsigned char *)o)[0x1c];
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((unsigned char *)o)[0x1c] = c & 0x7f;
    g236++;
    g204 -= 1;
    g204[0] = (int)o;
}
