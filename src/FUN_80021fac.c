// FUNC 80021fac 152 MAIN0
// MATCHING 80021fac 152
#include "TOBJ.H"
typedef struct B32 { int w[8]; } B32;
extern B32 DAT_1f8000f8;
extern void FUN_80022044(void);

void FUN_80021fac(TObj *o)
{
    *(short *)&o->d = 0x638;
    *((short *)&o->d + 1) = 0x800;
    o->w48 = 0;
    *(B32 *)&o->timer = DAT_1f8000f8;
    o->timer = 0xd00;
    *((short *)&o->anim + 1) = 0xd00;
    o->animTimer = 0xd00;
    *(char *)&o->h = 0xc0;
    *((char *)&o->h + 1) = 0xc0;
    *((char *)&o->h + 2) = 0xc0;
    FUN_80022044();
}
