// FUNC 80057edc 652 MAIN0
// MATCHING 80057edc 652
#include "TOBJ.H"
extern short DAT_1f8001c6;
extern short DAT_1f80025a;
extern TObj **DAT_1f800270;
extern short DAT_1f800240;
extern TObj **DAT_1f80022c;
extern unsigned char DAT_800a603a;
extern unsigned short DAT_8009c960[];
extern void FUN_800544c8(TObj *);
extern void FUN_80058168(TObj *);
extern void FUN_8005840c(TObj *);
extern void FUN_800eb044(TObj *);
extern void FUN_800ec710(TObj *);
extern void FUN_8011e778(TObj *);
extern void FUN_80116fb8(TObj *);

void FUN_80057edc(void)
{
    TObj *o;
    TObj **p;
    int n;

    if (DAT_1f8001c6 != 0) {
        n = DAT_1f80025a;
        p = DAT_1f800270;
        while (n != 0) {
            o = *p++;
            n--;
            switch (o->b0a) {
            case 0:
                FUN_800544c8(o);
                break;
            case 1:
                FUN_80058168(o);
                break;
            case 2:
                FUN_8005840c(o);
                break;
            case 3:
                if (DAT_800a603a & 2)
                    FUN_800ec710(o);
                else
                    FUN_800eb044(o);
                break;
            case 4:
                if (DAT_8009c960[0] == 4)
                    FUN_8011e778(o);
                if (DAT_8009c960[0] == 0xc)
                    FUN_80116fb8(o);
                break;
            }
        }
    } else {
        DAT_1f80025a = DAT_1f800240;
        DAT_1f800270 = DAT_1f80022c;
        while (DAT_1f800240 != 0) {
            o = *DAT_1f80022c++;
            DAT_1f800240--;
            switch (o->b0a) {
            case 0:
                FUN_800544c8(o);
                break;
            case 1:
                FUN_80058168(o);
                break;
            case 2:
                FUN_8005840c(o);
                break;
            case 3:
                if (DAT_800a603a & 2)
                    FUN_800ec710(o);
                else
                    FUN_800eb044(o);
                break;
            case 4:
                if (DAT_8009c960[0] == 4)
                    FUN_8011e778(o);
                if (DAT_8009c960[0] == 0xc)
                    FUN_80116fb8(o);
                break;
            }
        }
    }
}
