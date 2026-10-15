// FUNC 800f2dc4 192 I0NFO2
// MATCHING 800f2dc4 192
#include "TOBJ.H"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern unsigned char D_8009CDE1, D_8009CDE2[], D_8009CFAA;

int func_800F2DC4(TObj *o)
{
    Pos6 p;
    p = *(Pos6 *)&o->a;
    if (D_8009CDE2[0] != 0) {
        o->d90 = func_8002DCC8(2, 2, &p);
    } else if (D_8009CDE1 != 0) {
        D_8009CFAA++;
        return 0;
    } else {
        o->d90 = func_8002DCC8(2, 0, &p);
    }
    return 1;
}
