// FUNC 8006acf4 84 MAIN0
// MATCHING 8006acf4 84
typedef struct { char p[0x2c]; unsigned char *ptr; char q[6]; unsigned char b36; unsigned char b37; char r[0x46-0x38]; unsigned char k; } O;
void func_8006ACF4(O *o)
{
    int k;
    k = o->k;
    switch (k) {
    case 2:
        o->b37 = 0x44;
        o->ptr = (unsigned char *)o + 0x51;
        o->b36 = k;
        break;
    case 3:
        o->b37 = 0x4d;
        o->ptr = (unsigned char *)o + 0x5d;
        o->b36 = 6;
        break;
    }
}
