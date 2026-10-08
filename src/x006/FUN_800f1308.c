// FUNC 800f1308 368 X006
// MATCHING 800f1308 368
#include "TOBJ.H"
extern unsigned char DAT_8009d00f;
extern TObj *DAT_8009c330;
extern int FUN_8005bdec(void);
extern void FUN_800efc04(TObj *o);
extern void FUN_8001fe94(TObj *o, int a);
extern int FUN_8001fec0(TObj *o);

#define SETANIM(o, n)                                     \
    {                                                     \
        TObj *p = DAT_8009c330;                           \
        p->animTimer = n;                                 \
        if (p->animFrame != n) {                          \
            p->animTimer = n;                             \
            FUN_800efc04(o);                              \
            FUN_8001fe94(o, 0);                           \
            DAT_8009c330->animFrame = DAT_8009c330->animTimer; \
        }                                                 \
    }

void FUN_800f1308(TObj *o)
{
    switch (DAT_8009d00f) {
    case 1:
        if ((FUN_8005bdec() & 0x3f) == 0)
            SETANIM(o, 0x4c);
        if (FUN_8001fec0(o) != 0)
            DAT_8009c330->animFrame = 0xff;
        break;
    case 2:
        SETANIM(o, 0x4d);
        FUN_8001fec0(o);
        break;
    case 3:
        SETANIM(o, 0x4e);
        FUN_8001fec0(o);
        break;
    case 4:
        SETANIM(o, 0x4f);
        FUN_8001fec0(o);
        break;
    case 5:
        SETANIM(o, 0x4b);
        FUN_8001fec0(o);
        break;
    }
}
