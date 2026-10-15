// FUNC 8012d200 440 X010
// MATCHING 8012d200 440
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern short D_8012F4EC[];
extern unsigned short Rand(void);
extern void spawnGroundDebrisPiece(TObj *, int, int, int, int);
extern void PoolFree_1F800210(TObj *);

void func_8012D200(TObj *o)
{
    int t;
    short *p;

    switch (o->b04) {
    case 0:
        o->w08 = 0xf0;
        o->b04++;
        break;
    case 1:
        if (GVARS.b3f == 1) break;
        p = &(*(short*)((char*)&SPAD.i168 + 2));
        if (*p >= 0xc8b) break;
        if ((*(short*)((char*)&SPAD.i16c + 2)) < -0x59) break;
        if (--o->w08 != -1) break;
        o->w08 = D_8012F4EC[Rand() & 3];
        t = ((unsigned char*)&GCD.bceaf)[0];
        if (((unsigned char*)&GCD.bd0b4)[0] + t < 0x1e) {
            t = 5 - GCD.bcfc8;
            if (t > 0 && GVARS.b59 < t && (Rand() & 3) == 0)
                spawnGroundDebrisPiece(o, 0, *p, (short)((*(short*)((char*)&SPAD.i16c + 2)) + 0x14), (*(short*)((char*)&SPAD.i170 + 2)));
        } else {
            o->b04++;
        }
        break;
    case 2:
        PoolFree_1F800210(o);
        break;
    }
}
