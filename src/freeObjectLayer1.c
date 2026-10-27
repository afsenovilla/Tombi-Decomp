// FUNC 80018744 76 MAIN0
// MATCHING 80018744 76
extern int *g204;
extern unsigned short FREE_OBJECT_COUNT_LAYER1;
void freeObjectLayer1(int *o)
{
    unsigned char c = ((unsigned char *)o)[0x1c];
    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    o[3] = 0;
    ((unsigned char *)o)[0x1c] = c & 0x7f;
    FREE_OBJECT_COUNT_LAYER1++;
    g204 -= 1;
    g204[0] = (int)o;
}
