// FUNC 801194ac 612 X002
// MATCHING 801194ac 612
#include "TOBJ.H"

extern unsigned short D_8009C960, D_8009C962;
extern unsigned char *D_8007C178[];
extern unsigned char D_8009C975;
extern unsigned char D_8009C976;
extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
extern unsigned char D_8009CE57;

void func_801194AC(TObj *o)
{
    unsigned char *p;
    unsigned char r, g, b;
    unsigned char *k;
    unsigned char n;

    switch (o->b04) {
    case 0:
        p = D_8007C178[D_8009C960] + D_8009C962 * 4;
        r = *p++;
        g = p[0];
        b = p[1];
        D_8009C975 = 1;
        D_8009C976 = 0;
        D_8009E375 = r;
        D_8009E376 = g;
        D_8009E377 = b;
        D_8009F085 = r;
        D_8009F086 = g;
        D_8009F087 = b;
        o->b04++;
        switch (D_8009CE57) {
        case 1: case 2: case 3:
            D_8009C975 = 0x10;
            D_8009C976 = 0xc0;
            o->b04 = 5;
            break;
        }
        break;
    case 1:
        k = &D_8009C975;
        n = D_8009C976 + 8;
        *k = 2;
        D_8009C976 = n;
        if (n == 0) {
            *k = 0;
            o->b04++;
        }
        break;
    case 2:
        k = &D_8009C975;
        if (*k == 3) {
            { int m = -D_8009C976;
            *k = 2;
            D_8009C976 = m; }
            o->b04++;
        }
        break;
    case 3:
        k = &D_8009C975;
        n = D_8009C976 - 8;
        *k = 2;
        D_8009C976 = n;
        if (n == 0) {
            *k = 1;
            o->b04++;
            D_8009E375 = 0;
            D_8009E376 = 0;
            D_8009E377 = 0;
            D_8009F085 = 0;
            D_8009F086 = 0;
            D_8009F087 = 0;
        }
        break;
    case 4:
        if (D_8009C975 == 4) o->b04 = 0;
        break;
    case 5:
        k = &D_8009C975;
        if (*k == 3) {
            n = -D_8009C976;
            *k = 2;
            D_8009C976 = n;
            o->b04 = 3;
        } else if (D_8009CE57 == 0xff) {
            o->b04 = 1;
        }
        break;
    }
}
