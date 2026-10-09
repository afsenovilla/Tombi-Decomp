// FUNC 801271d8 404 X010
// MATCHING 801271d8 404
#include "TOBJ.H"

extern short D_8007A3F0[];
extern int D_800A604C;
extern Fix16 *D_800A6078x[];
#define D_800A6078 D_800A6078x[0]
extern unsigned short D_1F8001F8;
extern void playSFX(int);

void func_801271D8(TObj *o, int n)
{
    int s;

    if (n >= 2) {
        o->ba7 += 2;
        s = D_8007A3F0[o->ba7] << 6;
        D_800A604C = o->d34 + s;
        if (n == 3) {
            D_800A6078->raw += 0x8000;
            if (o->d30 + 0x40 < D_800A6078->p.whole) D_800A6078->p.whole = o->d30 + 0x40;
        } else if (n == 4) {
            D_800A6078->raw += -0x10000;
            if (D_800A6078->p.whole < o->d30 - 0x20) D_800A6078->p.whole = o->d30 - 0x20;
        } else {
            if (D_800A6078->p.whole != o->d30) {
                if (o->d30 < D_800A6078->p.whole) D_800A6078->raw -= 0x8000;
                else D_800A6078->raw += 0x8000;
            }
        }
    } else {
        o->ba7 += 8;
        s = D_8007A3F0[o->ba7] << 7;
        D_800A604C = o->d34 + s;
        if (n && !(D_1F8001F8 & 0x1f)) playSFX(0xe);
    }
}
