// FUNC 80055174 1188 MAIN0
// MATCHING 80055174 1188
#include "TOBJ.H"

extern short DAT_1f8001c6;
extern short DAT_1f800252, DAT_1f80024a;
extern TObj **DAT_1f800220, **DAT_1f800264;
extern unsigned char DAT_800a603a;

int FUN_8004fed4(TObj *o);
void FUN_8005296c(TObj *o);
void FUN_800532fc(TObj *o);
void FUN_80055618(TObj *o);
void FUN_80123e04(TObj *o);
void FUN_80054c38(TObj *o);
void FUN_80055cb8(TObj *o);
void FUN_80052190(TObj *o);
void FUN_80052db8(TObj *o);
void FUN_80055fac(TObj *o);
void FUN_80053b5c(TObj *o);
void FUN_800563ac(TObj *o);
void FUN_80054868(TObj *o);
void FUN_800eab58(TObj *o);
void FUN_800ec148(TObj *o);
void FUN_800eb77c(TObj *o);
void FUN_80056e4c(TObj *o);
void FUN_800544c8(TObj *o);
void FUN_80124780(TObj *o);
void FUN_8011c00c(TObj *o);
void FUN_800ecd88(TObj *o);
void FUN_801189ec(TObj *o);
void FUN_80118ddc(TObj *o);
void FUN_80057470(TObj *o);

void FUN_80055174(void)
{
    int n;
    TObj **p;
    TObj *o;

    if (DAT_1f8001c6 != 0) {
        n = DAT_1f800252;
        p = DAT_1f800264;
        while (n != 0) {
            o = *p++;
            n--;
            switch (FUN_8004fed4(o)) {
            case 0: FUN_8005296c(o); break;
            case 2: FUN_800532fc(o); break;
            case 4: FUN_80055618(o); break;
            case 5: FUN_80123e04(o); break;
            case 6: FUN_80054c38(o); break;
            case 7: FUN_80055cb8(o); break;
            case 8: FUN_80052190(o); break;
            case 1: case 9: FUN_80052db8(o); break;
            case 10: FUN_80055fac(o); break;
            case 3: case 11: FUN_80053b5c(o); break;
            case 12: FUN_800563ac(o); break;
            case 13:
                switch (DAT_800a603a) {
                case 0: FUN_80054868(o); break;
                case 1: FUN_800eab58(o); break;
                case 2: FUN_800ec148(o); break;
                case 3: FUN_800eb77c(o); break;
                }
            case 14: FUN_80056e4c(o); break;
            case 16: FUN_800544c8(o); break;
            case 17: FUN_80124780(o); break;
            case 18: FUN_8011c00c(o); break;
            case 19: FUN_800ecd88(o); break;
            case 20: FUN_801189ec(o); break;
            case 21: FUN_80118ddc(o); break;
            case 22: FUN_80057470(o); break;
            }
        }
    } else {
        DAT_1f800252 = DAT_1f80024a;
        DAT_1f800264 = DAT_1f800220;
        while (DAT_1f80024a != 0) {
            o = *DAT_1f800220++;
            DAT_1f80024a--;
            if (o->visible) {
                switch (FUN_8004fed4(o)) {
                case 0: FUN_8005296c(o); break;
                case 2: FUN_800532fc(o); break;
                case 4: FUN_80055618(o); break;
                case 5: FUN_80123e04(o); break;
                case 6: FUN_80054c38(o); break;
                case 7: FUN_80055cb8(o); break;
                case 8: FUN_80052190(o); break;
                case 1: case 9: FUN_80052db8(o); break;
                case 10: FUN_80055fac(o); break;
                case 3: case 11: FUN_80053b5c(o); break;
                case 12: FUN_800563ac(o); break;
                case 13:
                    switch (DAT_800a603a) {
                    case 0: FUN_80054868(o); break;
                    case 1: FUN_800eab58(o); break;
                    case 2: FUN_800ec148(o); break;
                    case 3: FUN_800eb77c(o); break;
                    }
                    break;
                case 14: FUN_80056e4c(o); break;
                case 16: FUN_800544c8(o); break;
                case 17: FUN_80124780(o); break;
                case 18: FUN_8011c00c(o); break;
                case 19: FUN_800ecd88(o); break;
                case 20: FUN_801189ec(o); break;
                case 21: FUN_80118ddc(o); break;
                case 22: FUN_80057470(o); break;
                }
            }
        }
    }
}
