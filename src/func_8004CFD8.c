// FUNC 8004cfd8 564 MAIN0
// MATCHING 8004cfd8 564
#include "TOBJ.H"
typedef struct {
    unsigned short c960; unsigned short c962; char p964[0x15 - 4];
    unsigned char c975; unsigned char c976; char p977[0x963 - 0x17]; unsigned char d2c3;
} G;
extern G D_8009C960;
extern unsigned char *D_8007C178[];
extern unsigned char *D_8007C1A8;
extern unsigned char D_1F8003D1;
extern unsigned char D_8009E375, D_8009E376, D_8009E377;
extern unsigned char D_8009F085, D_8009F086, D_8009F087;
void func_8004CFD8(TObj *o)
{
    unsigned char *p;
    int r, g, b;
    switch (o->b04) {
    case 0:
        D_8009C960.c975 = 1;
        D_8009C960.c976 = 0;
        p = D_8007C178[D_8009C960.c960];
        if (D_8009C960.c960 == 4 && (D_8009C960.d2c3 & 8)) p = D_8007C1A8;
        p += D_8009C960.c962 * 4;
        r = *p++;
        g = *p;
        b = p[1];
        if (D_8009C960.c960 == 3 && (D_8009C960.d2c3 & 2) && D_8009C960.c962 == D_8009C960.c960) {
            r = 0x60;
            g = 0x97;
            b = 0xff;
        }
        D_8009E375 = r;
        D_8009E376 = g;
        D_8009E377 = b;
        D_8009F085 = r;
        D_8009F086 = g;
        D_8009F087 = b;
        o->b04++;
        break;
    case 1:
        D_8009C960.c975 = 2;
        D_8009C960.c976 += 8;
        if (D_8009C960.c976 != 0) break;
        if (D_1F8003D1 == 1) D_1F8003D1 = 0;
        D_8009C960.c975 = 0;
        o->b04++;
        break;
    case 2:
        if (D_8009C960.c975 == 3) o->b04++;
        break;
    case 3:
        D_8009C960.c975 = 2;
        D_8009C960.c976 -= 8;
        if (D_8009C960.c976 != 0) break;
        D_8009C960.c975 = 1;
        o->b04++;
        D_8009E375 = 0;
        D_8009E376 = 0;
        D_8009E377 = 0;
        D_8009F085 = 0;
        D_8009F086 = 0;
        D_8009F087 = 0;
        break;
    case 4:
        if (D_8009C960.c975 == 4) o->b04 = 0;
        break;
    }
}
