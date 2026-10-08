// FUNC 800500c4 872 MAIN0
// MATCHING 800500c4 872
#include "TOBJ.H"
extern int D_8009C960;
extern unsigned short D_8009C960s;
extern unsigned short D_8009C962;
extern unsigned char D_8009C964, D_8009C93A, D_800A603A;
int func_80045D0C(TObj *o);
void func_8011CEF0(TObj *o);
void func_8011D1BC(TObj *o);
void func_8011D4E0(TObj *o);
void func_8011DBF4(TObj *o);
void func_8005296C(TObj *o);
void func_80052DB8(TObj *o);
void func_800532FC(TObj *o);
void func_80053B5C(TObj *o);
void func_80054C38(TObj *o);
void func_800EAB58(TObj *o);
void func_800EC148(TObj *o);
void func_800EB77C(TObj *o);
void func_80123FD0(TObj *o);
void func_8011EBC0(TObj *o);
void func_8011F240(TObj *o);
void func_8011B830(TObj *o);
void func_8011C00C(TObj *o);
void func_80054868(TObj *o);
void FUN_80057368(TObj *o);
void FUN_80057470(TObj *o);
void FUN_80057610(TObj *o);
void FUN_800577b8(TObj *o);
void FUN_800579a8(TObj *o);
void FUN_80057ae4(TObj *o);
void FUN_80057bf0(TObj *o);

void func_800500C4(TObj *o)
{
    if (o->visible == 0) return;
    if ((o->b0a & 0x10) && o->da0 == 0) return;
    if (D_8009C960 == 9 && D_8009C964 != 0x20 && D_8009C93A == 1 &&
        o->b.p.whole < 0xb6d && (o->category & 0x7f) != 8) return;
    if (D_8009C960s == 10 && (D_8009C962 == 0 || D_8009C962 == 4) && D_8009C964 != 0 &&
        (unsigned short)(o->b.p.whole + 9) < 0x13 && (o->category & 0x7f) != 8) return;
    if (D_8009C960 == 6) {
        switch (o->b0a) {
        case 0: func_8011CEF0(o); break;
        case 1: func_8011D1BC(o); break;
        case 2: func_8011D4E0(o); break;
        case 3: func_8011DBF4(o); break;
        case 0x10: goto c10;
        case 0x11: goto c11;
        case 0x12: goto c12;
        case 0x13: goto c13;
        case 0x14: goto c14;
        case 0x15: goto c15;
        case 0x16: goto c16;
        }
        return;
    }
    switch (func_80045D0C(o)) {
    case 0: func_8005296C(o); break;
    case 1: func_80052DB8(o); break;
    case 2: func_800532FC(o); break;
    case 3: func_80053B5C(o); break;
    case 4: func_80054C38(o); break;
    case 5:
        switch (D_800A603A) {
        case 1: func_800EAB58(o); break;
        case 2: func_800EC148(o); break;
        case 3: func_800EB77C(o); break;
        }
        break;
    case 6: func_80123FD0(o); break;
    case 7: func_8011EBC0(o); break;
    case 8: func_8011F240(o); break;
    case 9: func_8011B830(o); break;
    case 10: func_8011C00C(o); break;
    case 13: func_80054868(o); break;
    case 0x10: c10: FUN_80057368(o); break;
    case 0x11: c11: FUN_80057470(o); break;
    case 0x12: c12: FUN_80057610(o); break;
    case 0x13: c13: FUN_800577b8(o); break;
    case 0x14: c14: FUN_800579a8(o); break;
    case 0x15: c15: FUN_80057ae4(o); break;
    case 0x16: c16: FUN_80057bf0(o); break;
    }
}
