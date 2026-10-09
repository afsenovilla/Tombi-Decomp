// FUNC 801271d8 404 X010
/* score 12: only v0/v1 swapped in D_800A604C = d34 + (sin << 6/7) (game: d34 in v0, sine in v1, sum in d34 reg).
   Tried: d34-first operand order (sum then sinks below li 3), temps, decl order, inline helper, short/int types,
   volatile/[0] for D_800A604C, do-while barriers, -fno-schedule-insns. */
#include "TOBJ.H"

extern short D_8007A3F0[];
extern int D_800A604C;
extern Fix16 *D_800A6078x[];
#define D_800A6078 D_800A6078x[0]
extern unsigned short D_1F8001F8;
extern void playSFX(int);

void func_801271D8(TObj *o, int n)
{
    if (n >= 2) {
        o->ba7 += 2;
        D_800A604C = (D_8007A3F0[o->ba7] << 6) + o->d34;
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
        D_800A604C = (D_8007A3F0[o->ba7] << 7) + o->d34;
        if (n && !(D_1F8001F8 & 0x1f)) playSFX(0xe);
    }
}
