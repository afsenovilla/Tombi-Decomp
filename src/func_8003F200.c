// FUNC 8003f200 804 MAIN0
// MATCHING 8003f200 804
typedef struct { short x, y; } XY;
extern short D_800A457C, D_800A457E;
extern int D_8009C960;
extern unsigned short D_8009C960h;
extern unsigned short D_8009C962;
extern short *D_800A607C;
extern char D_8007B5DC[];
extern XY *D_8007B460[];
extern XY D_8007B450[];
extern XY D_8007B44C[];
extern XY *D_8007B46C;
extern unsigned char *D_8007B55C[];
extern char *D_1F800308[];
extern char *D_1F80030C;
extern unsigned short D_1F8001C8;
#define M960 D_8009C960h

char *func_8003F200(short x, short y)
{
    XY *t;
    char *b;
    short r, c;
    unsigned char lim;

    if (x < D_800A457C - 0xaa || D_800A457E + 0xaa < x || (D_8009C960 != 0xe && x < 0))
        return D_8007B5DC;
    if (M960 == 0 && D_8009C962 != 3) {
        if (y >= 0x10f) {
            b = D_1F800308[0];
            y -= 0x10e;
        } else {
            b = D_1F80030C;
        }
        t = D_8007B460[M960];
        t += D_8009C962;
    } else if ((M960 == 4 || M960 == 12) && D_8009C962 < 4) {
        t = &D_8007B450[D_8009C962];
        b = D_1F800308[D_1F8001C8 & 1];
    } else if (M960 == 3 && (D_8009C962 == 1 || D_8009C962 == 5)) {
        if (D_800A607C[1] < 0) {
            b = D_1F80030C;
            t = D_8007B44C;
            y += 0x5a;
        } else {
            t = D_8007B46C;
            b = D_1F800308[0];
            t += D_8009C962;
        }
    } else {
        t = D_8007B460[M960];
        b = D_1F800308[0];
        t += D_8009C962;
    }
    if (D_1F8001C8 & 1)
        r = (t->y - y) / 90;
    else
        r = (y - t->y) / 90;
    { unsigned char *q = D_8007B55C[M960]; lim = q[D_8009C962]; }
    if (lim < r) r = lim;
    if (r < 0) r = 0;
    b += *(unsigned short *)(b + 8 + r * 2);
    c = (x - t->x) / 8;
    if (c < 0) c = 0;
    return b + *(unsigned short *)(b + (c << 1));
}
