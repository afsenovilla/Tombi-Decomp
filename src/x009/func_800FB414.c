// FUNC 800fb414 12 X009
// MATCHING 800fb414 12
void copyD88ToD8C(unsigned char *o)
{
    *(int *)(o + 0x8c) = o[0x88];
}
