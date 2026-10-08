// FUNC 80054f54 544 MAIN0
// MATCHING 80054f54 544
#include "TOBJ.H"
extern short DAT_1f8001c6;
extern short DAT_1f800250;
extern TObj **DAT_1f800260;
extern unsigned short DAT_1f800246;
extern short DAT_1f800246s;
extern TObj **DAT_1f80021c;
extern unsigned char DAT_800a603a;
extern void FUN_80054868(TObj *);
extern void FUN_800eab58(TObj *);
extern void FUN_800ec148(TObj *);
extern void FUN_800eb77c(TObj *);
extern void FUN_800500c4(TObj *);

void FUN_80054f54(void)
{
    int n;
    TObj **p;
    TObj *o;
    if (DAT_1f8001c6 != 0) {
        n = DAT_1f800250;
        p = DAT_1f800260;
        while (n != 0) {
            o = *p++;
            n--;
            if (o->b0a == 0xd) {
                switch (DAT_800a603a) {
                case 0: FUN_80054868(o); break;
                case 1: FUN_800eab58(o); break;
                case 2: FUN_800ec148(o); break;
                case 3: FUN_800eb77c(o); break;
                }
            } else {
                FUN_800500c4(o);
            }
        }
    } else {
        DAT_1f800250 = DAT_1f800246;
        DAT_1f800260 = DAT_1f80021c;
        if (DAT_1f800246 == 0)
            return;
        do {
            o = *DAT_1f80021c++;
            DAT_1f800246--;
            if (o->visible) {
                if (o->b0a == 0xd) {
                    switch (DAT_800a603a) {
                    case 0: FUN_80054868(o); break;
                    case 1: FUN_800eab58(o); break;
                    case 2: FUN_800ec148(o); break;
                    case 3: FUN_800eb77c(o); break;
                    }
                } else {
                    FUN_800500c4(o);
                }
            }
        } while (DAT_1f800246s != 0);
    }
}
