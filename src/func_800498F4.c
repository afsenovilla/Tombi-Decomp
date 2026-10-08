// FUNC 800498f4 944 MAIN0
// MATCHING 800498f4 944
typedef struct S { char p[0xc]; unsigned char b0c; } S;
extern unsigned short D_8009C960;
extern unsigned short D_8009C962;
extern void func_80118534(S *), func_8012B454(S *), func_80129BE4(S *), func_80134E60(S *);
extern void func_8012BEDC(S *), func_8012C858(S *), func_8012A890(S *), func_80129A9C(S *);
extern void func_80129524(S *), func_8012FB4C(S *), func_80130320(S *), func_8012A8B8(S *);
extern void func_8012A124(S *), func_801171A4(S *), func_801294B4(S *), func_8012E010(S *);
extern void func_80125C50(S *), func_801270AC(S *), func_80119940(S *), func_80117F60(S *);
extern void func_80118AE8(S *), func_801184FC(S *), func_80118014(S *), func_801170F4(S *);
extern void func_8011A32C(S *);
void func_800498F4(S *o)
{
    switch (D_8009C960) {
    case 2:
        switch (D_8009C962) {
        case 2:
        case 3:
            break;
        case 5:
            func_80118534(o);
            break;
        }
        break;
    case 3:
        switch (D_8009C962) {
        case 0:
        case 4:
            if (o->b0c == 0x28)
                func_8012B454(o);
            else
                func_80129BE4(o);
            break;
        case 1:
        case 5:
            switch (o->b0c) {
            case 9:
                func_80129BE4(o);
                break;
            case 3:
                func_80134E60(o);
                break;
            case 0x28:
                func_8012BEDC(o);
                break;
            case 0x29:
                func_8012C858(o);
                break;
            }
            break;
        case 2:
            func_80129BE4(o);
            break;
        case 3:
            func_8012A890(o);
            break;
        }
        break;
    case 4:
    case 12:
        switch (D_8009C962) {
        case 7: func_80129A9C(o); break;
        case 8: func_80129524(o); break;
        case 9: func_8012FB4C(o); break;
        case 12: func_80130320(o); break;
        case 17: func_8012A8B8(o); break;
        case 18: func_8012A124(o); break;
        }
        break;
    case 5:
        func_801171A4(o);
        break;
    case 9:
        func_801294B4(o);
        break;
    case 10:
        switch (D_8009C962) {
        case 0:
        case 4:
            switch (o->b0c) {
            case 0: func_8012E010(o); break;
            case 0x28: func_80125C50(o); break;
            }
            break;
        case 8:
            func_801270AC(o);
            break;
        case 1:
            break;
        case 2:
            break;
        }
        break;
    case 11:
        switch (D_8009C962) {
        case 1:
        case 3:
            func_80119940(o);
            break;
        case 4:
            break;
        case 0:
            break;
        case 2:
            break;
        }
        break;
    case 16:
        switch (D_8009C962) {
        case 5: func_80117F60(o); break;
        case 6: func_80118AE8(o); break;
        }
        break;
    case 17:
        switch (D_8009C962) {
        case 0: case 1: case 3: case 4: case 5: case 10:
            func_801184FC(o);
            break;
        case 2: case 7:
            func_80118014(o);
            break;
        }
        break;
    case 18:
        switch (D_8009C962) {
        case 1: func_801170F4(o); break;
        case 2: func_8011A32C(o); break;
        case -1: break;
        }
        break;
    }
}
